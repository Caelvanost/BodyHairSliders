#include "OverlayManager.h"
#include "RaceMenuIntegration.h"

namespace BHS
{
    namespace
    {
        constexpr SKEE::u8 kUnindexedProperty = 0xFF;

        class StringVariant final : public SKEE::IOverrideInterface::SetVariant
        {
        public:
            explicit StringVariant(std::string value) : value_(std::move(value)) {}
            Type GetType() override { return Type::String; }
            const char* String() override { return value_.c_str(); }
        private:
            std::string value_;
        };

        class IntVariant final : public SKEE::IOverrideInterface::SetVariant
        {
        public:
            explicit IntVariant(SKEE::i32 value) : value_(value) {}
            Type GetType() override { return Type::Int; }
            SKEE::i32 Int() override { return value_; }
        private:
            SKEE::i32 value_;
        };

        class FloatVariant final : public SKEE::IOverrideInterface::SetVariant
        {
        public:
            explicit FloatVariant(float value) : value_(value) {}
            Type GetType() override { return Type::Float; }
            float Float() override { return value_; }
        private:
            float value_;
        };

        SKEE::IOverlayInterface::OverlayLocation ToOverlayLocation(std::string_view location)
        {
            if (location == "hand" || location == "hands") {
                return SKEE::IOverlayInterface::OverlayLocation::Hand;
            }
            if (location == "feet" || location == "foot") {
                return SKEE::IOverlayInterface::OverlayLocation::Feet;
            }
            if (location == "face") {
                return SKEE::IOverlayInterface::OverlayLocation::Face;
            }
            return SKEE::IOverlayInterface::OverlayLocation::Body;
        }

        std::string MakeNodeName(const char* format, std::uint32_t index)
        {
            std::string result = format ? format : "Body [Ovl{}]";
            if (const auto pos = result.find("{}"); pos != std::string::npos) {
                result.replace(pos, 2, std::to_string(index));
            }
            return result;
        }

        std::string ToSKEETexturePath(std::string path)
        {
            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.starts_with("Data\\")) {
                path.erase(0, 5);
            } else if (path.starts_with("data\\")) {
                path.erase(0, 5);
            }
            return path;
        }

        std::uint32_t PackRGB(const RGBA& color)
        {
            const auto byte = [](float value) {
                return static_cast<std::uint32_t>(std::clamp(value, 0.0F, 1.0F) * 255.0F + 0.5F);
            };
            return (byte(color.r) << 16) | (byte(color.g) << 8) | byte(color.b);
        }

        std::string SelectTexture(const OverlayStyle& style, const RGBA& color)
        {
            if (!style.texture.empty()) {
                return style.texture;
            }

            const float luminance = 0.2126F * color.r + 0.7152F * color.g + 0.0722F * color.b;
            if (luminance >= 0.55F && !style.textureFair.empty()) {
                return style.textureFair;
            }
            if (!style.textureDark.empty()) {
                return style.textureDark;
            }
            return style.textureFair;
        }
    }

    OverlayManager& OverlayManager::GetSingleton()
    {
        static OverlayManager singleton;
        return singleton;
    }

    std::optional<std::uint32_t> OverlayManager::FindOrReserveSlot(
        RE::Actor* actor,
        std::string_view region,
        std::string_view location,
        bool isFemale)
    {
        auto& integration = RaceMenuIntegration::GetSingleton();
        auto* overlay = integration.Overlay();
        auto* overrideInterface = integration.Override();
        if (!actor || !overlay || !overrideInterface) {
            return std::nullopt;
        }

        auto& actorSlots = slots_[actor->GetFormID()];
        if (const auto it = actorSlots.find(std::string(region)); it != actorSlots.end()) {
            return it->second.index;
        }

        const auto overlayLocation = ToOverlayLocation(location);
        const auto count = overlay->GetOverlayCount(
            SKEE::IOverlayInterface::OverlayType::Normal,
            overlayLocation);
        const auto* format = overlay->GetOverlayFormat(
            SKEE::IOverlayInterface::OverlayType::Normal,
            overlayLocation);

        for (std::uint32_t i = count; i-- > 0;) {
            const auto nodeName = MakeNodeName(format, i);
            if (!overrideInterface->HasNodeOverride(actor, isFemale, nodeName.c_str(), SKEE::kShaderTexture, 0)) {
                actorSlots.emplace(std::string(region), ReservedSlot{ i, std::string(location) });
                SKSE::log::info("Reserved RaceMenu {} overlay slot {} ({}) for region '{}'",
                    location, i, nodeName, region);
                return i;
            }
        }

        SKSE::log::error("No free RaceMenu {} overlay slot for region '{}'; increase the matching overlay count in skee64.ini",
            location, region);
        return std::nullopt;
    }

    bool OverlayManager::Apply(RE::Actor* actor, const OverlayStyle* style, const RGBA& color)
    {
        if (!actor || !style || style->type != "overlay") {
            return false;
        }

        auto& integration = RaceMenuIntegration::GetSingleton();
        if (!integration.IsAvailable()) {
            SKSE::log::error("Cannot apply '{}': SKEE is unavailable", style->id);
            return false;
        }

        const bool isFemale = style->sex == "female";
        const auto slot = FindOrReserveSlot(actor, style->region, style->location, isFemale);
        if (!slot) {
            return false;
        }

        auto* overlay = integration.Overlay();
        auto* overrideInterface = integration.Override();
        const auto overlayLocation = ToOverlayLocation(style->location);
        const auto* format = overlay->GetOverlayFormat(
            SKEE::IOverlayInterface::OverlayType::Normal,
            overlayLocation);
        const auto nodeName = MakeNodeName(format, *slot);
        const auto texture = ToSKEETexturePath(SelectTexture(*style, color));
        if (texture.empty()) {
            SKSE::log::error("Style '{}' has no usable texture", style->id);
            return false;
        }

        overlay->AddOverlays(actor, true);

        StringVariant textureValue(texture);
        IntVariant tintValue(static_cast<SKEE::i32>(PackRGB(color)));
        FloatVariant alphaValue(std::clamp(color.a, 0.0F, 1.0F));

        overrideInterface->AddNodeOverride(actor, isFemale, nodeName.c_str(), SKEE::kShaderTexture, 0, textureValue);
        overrideInterface->AddNodeOverride(actor, isFemale, nodeName.c_str(), SKEE::kShaderTintColor, kUnindexedProperty, tintValue);
        overrideInterface->AddNodeOverride(actor, isFemale, nodeName.c_str(), SKEE::kShaderAlpha, kUnindexedProperty, alphaValue);
        overrideInterface->SetNodeProperties(actor, true);

        if (auto* update = integration.ActorUpdate()) {
            update->AddOverlayUpdate(actor->GetFormID());
            update->AddNodeOverrideUpdate(actor->GetFormID());
            update->Flush();
        }

        SKSE::log::info("Applied provider={} region={} location={} style={} node={} texture={} rgb=#{:06X} alpha={:.2f}",
            style->provider, style->region, style->location, style->id, nodeName, texture, PackRGB(color), color.a);
        return true;
    }

    bool OverlayManager::Clear(RE::Actor* actor, std::string_view region)
    {
        if (!actor) {
            return false;
        }

        auto& integration = RaceMenuIntegration::GetSingleton();
        auto* overlay = integration.Overlay();
        auto* overrideInterface = integration.Override();
        if (!overlay || !overrideInterface) {
            return false;
        }

        auto actorIt = slots_.find(actor->GetFormID());
        if (actorIt == slots_.end()) {
            return false;
        }
        auto slotIt = actorIt->second.find(std::string(region));
        if (slotIt == actorIt->second.end()) {
            return false;
        }

        const auto overlayLocation = ToOverlayLocation(slotIt->second.location);
        const auto* format = overlay->GetOverlayFormat(
            SKEE::IOverlayInterface::OverlayType::Normal,
            overlayLocation);
        const auto nodeName = MakeNodeName(format, slotIt->second.index);

        // Remove for both sexes so a character sex/style change cannot leave stale data.
        for (bool female : { false, true }) {
            overrideInterface->RemoveNodeOverride(actor, female, nodeName.c_str(), SKEE::kShaderTexture, 0);
            overrideInterface->RemoveNodeOverride(actor, female, nodeName.c_str(), SKEE::kShaderTintColor, kUnindexedProperty);
            overrideInterface->RemoveNodeOverride(actor, female, nodeName.c_str(), SKEE::kShaderAlpha, kUnindexedProperty);
        }
        overrideInterface->SetNodeProperties(actor, true);
        actorIt->second.erase(slotIt);

        if (auto* update = integration.ActorUpdate()) {
            update->AddOverlayUpdate(actor->GetFormID());
            update->AddNodeOverrideUpdate(actor->GetFormID());
            update->Flush();
        }
        return true;
    }

    void OverlayManager::Refresh(RE::Actor* actor)
    {
        if (!actor) {
            return;
        }
        if (auto* update = RaceMenuIntegration::GetSingleton().ActorUpdate()) {
            update->AddOverlayUpdate(actor->GetFormID());
            update->AddNodeOverrideUpdate(actor->GetFormID());
            update->Flush();
        }
    }
}
