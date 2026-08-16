#include "OverlayManager.h"
#include "RaceMenuIntegration.h"

namespace BHS
{
    namespace
    {
        class StringCaptureVariant final : public SKEE::IOverrideInterface::GetVariant
        {
        public:
            void Int(SKEE::i32) override {}
            void Float(float) override {}
            void String(const char* value) override
            {
                if (value) {
                    value_ = value;
                    hasValue_ = true;
                }
            }
            void Bool(bool) override {}
            void TextureSet(const RE::BGSTextureSet*) override {}

            [[nodiscard]] bool HasValue() const { return hasValue_; }
            [[nodiscard]] const std::string& Value() const { return value_; }

        private:
            std::string value_;
            bool hasValue_{ false };
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

        std::string NormalizeTexturePath(std::string path)
        {
            std::replace(path.begin(), path.end(), '/', '\\');
            std::transform(path.begin(), path.end(), path.begin(), [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            });
            if (path.starts_with("data\\")) {
                path.erase(0, 5);
            }
            return path;
        }

        bool MatchesKnownTexture(std::string_view candidate, const OverlayStyle& style)
        {
            const auto normalizedCandidate = NormalizeTexturePath(std::string(candidate));
            for (const auto* texture : { &style.texture, &style.textureDark, &style.textureFair }) {
                if (!texture->empty() && NormalizeTexturePath(*texture) == normalizedCandidate) {
                    return true;
                }
            }
            return false;
        }

        std::optional<std::string> ReadNodeTexture(
            SKEE::IOverrideInterface* overrideInterface,
            RE::Actor* actor,
            bool isFemale,
            const std::string& nodeName)
        {
            StringCaptureVariant capture;
            if (overrideInterface->GetNodeOverride(
                    actor,
                    isFemale,
                    nodeName.c_str(),
                    SKEE::kShaderTexture,
                    0,
                    capture) &&
                capture.HasValue()) {
                return capture.Value();
            }
            return std::nullopt;
        }
    }

    std::int32_t OverlayManager::DetectStyleIndex(RE::Actor* actor, std::string_view region, bool isFemale) const
    {
        if (!actor) {
            return 0;
        }

        auto& integration = RaceMenuIntegration::GetSingleton();
        auto* overlay = integration.Overlay();
        auto* overrideInterface = integration.Override();
        if (!overlay || !overrideInterface) {
            return 0;
        }

        const auto sex = isFemale ? "female"sv : "male"sv;
        const auto styles = Settings::GetSingleton().GetStylesForRegion(region, sex);

        for (std::size_t styleIndex = 0; styleIndex < styles.size(); ++styleIndex) {
            const auto* style = styles[styleIndex];
            if (!style) {
                continue;
            }

            const auto location = ToOverlayLocation(style->location);
            const auto count = overlay->GetOverlayCount(
                SKEE::IOverlayInterface::OverlayType::Normal,
                location);
            const auto* format = overlay->GetOverlayFormat(
                SKEE::IOverlayInterface::OverlayType::Normal,
                location);

            for (std::uint32_t slot = 0; slot < count; ++slot) {
                const auto nodeName = MakeNodeName(format, slot);
                if (!overrideInterface->HasNodeOverride(
                        actor,
                        isFemale,
                        nodeName.c_str(),
                        SKEE::kShaderTexture,
                        0)) {
                    continue;
                }

                const auto texture = ReadNodeTexture(overrideInterface, actor, isFemale, nodeName);
                if (texture && MatchesKnownTexture(*texture, *style)) {
                    const auto result = static_cast<std::int32_t>(styleIndex + 1);
                    SKSE::log::info(
                        "Detected existing body-hair overlay region='{}' style={} provider={} slot={} texture={}",
                        region,
                        result,
                        style->provider,
                        slot,
                        *texture);
                    return result;
                }
            }
        }

        return 0;
    }
}
