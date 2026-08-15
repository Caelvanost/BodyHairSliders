#include "OverlayManager.h"
#include "RaceMenuIntegration.h"

namespace BHS
{
    namespace
    {
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

    std::optional<std::uint32_t> OverlayManager::FindOrReserveBodySlot(RE::Actor* actor, std::string_view region, bool isFemale)
    {
        auto& integration = RaceMenuIntegration::GetSingleton();
        auto* overlay = integration.Overlay();
        auto* overrideInterface = integration.Override();
        if (!actor || !overlay || !overrideInterface) {
            return std::nullopt;
        }

        auto& actorSlots = slots_[actor->GetFormID()];
        if (const auto it = actorSlots.find(std::string(region)); it != actorSlots.end()) {
            return it->second;
        }

        const auto count = overlay->GetOverlayCount(
            SKEE::IOverlayInterface::OverlayType::Normal,
            SKEE::IOverlayInterface::OverlayLocation::Body);
        const auto* format = overlay->GetOverlayFormat(
            SKEE::IOverlayInterface::OverlayType::Normal,
            SKEE::IOverlayInterface::OverlayLocation::Body);

        for (std::uint32_t i = count; i-- > 0;) {
            const auto nodeName = MakeNodeName(format, i);
            if (!overrideInterface->HasNodeOverride(actor, isFemale, nodeName.c_str(), SKEE::kShaderTexture, 0)) {
                actorSlots.emplace(std::string(region), i);
                SKSE::log::info("Reserved RaceMenu body overlay slot {} ({}) for region '{}'", i, nodeName, region);
                return i;
            }
        }

        SKSE::log::error("No free RaceMenu Body overlay slot for region '{}'; increase [Overlays/Body] iNumOverlays in skee64.ini", region);
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
        const auto slot = FindOrReserveBodySlot(actor, style->region, isFemale);
        if (!slot) {
            return false;
        }

        auto* overlay = integration.Overlay();
        auto* overrideInterface = integration.Override();
        const auto* format = overlay->GetOverlayFormat(
            SKEE::IOverlayInterface::OverlayType::Normal,
            SKEE::IOverlayInterface::OverlayLocation::Body);
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
        overrideInterface->AddNodeOverride(actor, isFemale, nodeName.c_str(), SKEE::kShaderTintColor, 0, tintValue);
        overrideInterface->AddNodeOverride(actor, isFemale, nodeName.c_str(), SKEE::kShaderAlpha, 0, alphaValue);
        overrideInterface->SetNodeProperties(actor, true);

        if (auto* update = integration.ActorUpdate()) {
            update->AddOverlayUpdate(actor->GetFormID());
            update->AddNodeOverrideUpdate(actor->GetFormID());
            update->Flush();
        }

        SKSE::log::info("Applied provider={} region={} style={} node={} texture={} rgb=#{:06X} alpha={:.2f}",
            style->provider, style->region, style->id, nodeName, texture, PackRGB(color), color.a);
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

        const auto* format = overlay->GetOverlayFormat(
            SKEE::IOverlayInterface::OverlayType::Normal,
            SKEE::IOverlayInterface::OverlayLocation::Body);
        const auto nodeName = MakeNodeName(format, slotIt->second);

        // Remove for both sexes so a character sex/style change cannot leave stale data.
        for (bool female : { false, true }) {
            overrideInterface->RemoveNodeOverride(actor, female, nodeName.c_str(), SKEE::kShaderTexture, 0);
            overrideInterface->RemoveNodeOverride(actor, female, nodeName.c_str(), SKEE::kShaderTintColor, 0);
            overrideInterface->RemoveNodeOverride(actor, female, nodeName.c_str(), SKEE::kShaderAlpha, 0);
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
