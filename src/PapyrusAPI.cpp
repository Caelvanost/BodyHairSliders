#include "PapyrusAPI.h"

#include "HairColor.h"
#include "OverlayManager.h"
#include "RaceMenuIntegration.h"
#include "Settings.h"

namespace BHS::PapyrusAPI
{
    namespace
    {
        constexpr std::array<std::string_view, 10> kColorNames{
            "Black", "Dark Brown", "Brown", "Light Brown", "Auburn",
            "Red", "Dark Blonde", "Blonde", "Platinum", "Grey"
        };

        std::unordered_map<std::string, std::int32_t> g_currentSelections;
        std::int32_t g_currentColorIndex{ 0 };
        bool g_currentFemale{ false };

        std::string ToString(const RE::BSFixedString& value)
        {
            return value.c_str() ? value.c_str() : "";
        }

        std::string SexName(bool female)
        {
            return female ? "female" : "male";
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

        std::string ToSKEETexturePath(std::string path)
        {
            std::replace(path.begin(), path.end(), '/', '\\');
            if (path.starts_with("Data\\") || path.starts_with("data\\")) {
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

        RGBA CurrentColor(RE::Actor* actor)
        {
            if (g_currentColorIndex <= 0) {
                return HairColor::FromActor(actor);
            }
            return HairColor::Preset(static_cast<std::size_t>(g_currentColorIndex - 1));
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

        const OverlayStyle* GetStyle(std::string_view region, std::int32_t index, bool female)
        {
            if (index <= 0) {
                return nullptr;
            }
            const auto styles = Settings::GetSingleton().GetStylesForRegion(region, SexName(female));
            const auto zeroBased = static_cast<std::size_t>(index - 1);
            return zeroBased < styles.size() ? styles[zeroBased] : nullptr;
        }

        std::int32_t GetStyleCount(RE::StaticFunctionTag*, RE::BSFixedString region, bool female)
        {
            const auto regionName = ToString(region);
            const auto sex = SexName(female);
            const auto styles = Settings::GetSingleton().GetStylesForRegion(regionName, sex);
            SKSE::log::info("RaceMenu style query region='{}' sex={} count={}", regionName, sex, styles.size());
            return static_cast<std::int32_t>(styles.size());
        }

        RE::BSFixedString GetStyleName(RE::StaticFunctionTag*, RE::BSFixedString region, std::int32_t index, bool female)
        {
            if (index <= 0) {
                return RE::BSFixedString("None / Shaved");
            }

            const auto* style = GetStyle(ToString(region), index, female);
            if (!style) {
                return RE::BSFixedString("Invalid");
            }

            const auto label = style->provider + " - " + style->label;
            return RE::BSFixedString(label.c_str());
        }

        bool IsLegacySKEE(RE::StaticFunctionTag*)
        {
            return RaceMenuIntegration::GetSingleton().IsLegacy();
        }

        RE::BSFixedString GetStyleTexture(
            RE::StaticFunctionTag*,
            RE::BSFixedString region,
            std::int32_t index,
            bool female)
        {
            const auto* style = GetStyle(ToString(region), index, female);
            auto* player = RE::PlayerCharacter::GetSingleton();
            if (!style || !player) {
                return RE::BSFixedString("");
            }

            const auto texture = ToSKEETexturePath(SelectTexture(*style, CurrentColor(player)));
            return RE::BSFixedString(texture.c_str());
        }

        RE::BSFixedString GetStyleLocation(
            RE::StaticFunctionTag*,
            RE::BSFixedString region,
            std::int32_t index,
            bool female)
        {
            const auto* style = GetStyle(ToString(region), index, female);
            return style ? RE::BSFixedString(style->location.c_str()) : RE::BSFixedString("");
        }

        std::int32_t FindStyleIndexByTexture(
            RE::StaticFunctionTag*,
            RE::BSFixedString region,
            RE::BSFixedString texture,
            bool female)
        {
            const auto textureName = ToString(texture);
            if (textureName.empty()) {
                return 0;
            }

            const auto styles = Settings::GetSingleton().GetStylesForRegion(ToString(region), SexName(female));
            for (std::size_t i = 0; i < styles.size(); ++i) {
                if (styles[i] && MatchesKnownTexture(textureName, *styles[i])) {
                    return static_cast<std::int32_t>(i + 1);
                }
            }
            return 0;
        }

        std::int32_t GetCurrentColorRGB(RE::StaticFunctionTag*)
        {
            auto* player = RE::PlayerCharacter::GetSingleton();
            return player ? static_cast<std::int32_t>(PackRGB(CurrentColor(player))) : 0;
        }

        std::int32_t GetCurrentStyleIndex(RE::StaticFunctionTag*, RE::BSFixedString region, bool female)
        {
            const auto regionName = ToString(region);
            if (RaceMenuIntegration::GetSingleton().IsLegacy()) {
                // Legacy RaceMenu is inspected safely by BodyHairSlidersRaceMenu.psc
                // through the old NiOverride Papyrus API. Never enter OverlayManager,
                // which deliberately targets only the modern SKEE C++ wrapper ABI.
                const auto it = g_currentSelections.find(regionName);
                return it != g_currentSelections.end() ? it->second : 0;
            }

            auto* player = RE::PlayerCharacter::GetSingleton();
            if (!player) {
                return 0;
            }

            const auto liveIndex = OverlayManager::GetSingleton().DetectStyleIndex(player, regionName, female);
            g_currentSelections[regionName] = liveIndex;
            g_currentFemale = female;
            return liveIndex;
        }

        bool ApplyStyleInternal(std::string_view region, std::int32_t index, bool female)
        {
            if (RaceMenuIntegration::GetSingleton().IsLegacy()) {
                SKSE::log::warn("Native ApplyStyle ignored on legacy SKEE; RaceMenu frontend must use NiOverride fallback");
                return false;
            }

            auto* player = RE::PlayerCharacter::GetSingleton();
            if (!player) {
                return false;
            }

            const auto regionString = std::string(region);
            g_currentFemale = female;

            if (index <= 0) {
                g_currentSelections[regionString] = 0;
                return OverlayManager::GetSingleton().Clear(player, region);
            }

            const auto styles = Settings::GetSingleton().GetStylesForRegion(region, SexName(female));
            const auto zeroBased = static_cast<std::size_t>(index - 1);
            if (zeroBased >= styles.size()) {
                SKSE::log::warn("Papyrus ApplyStyle rejected region='{}' index={} count={}", region, index, styles.size());
                return false;
            }

            const auto* style = styles[zeroBased];
            if (!OverlayManager::GetSingleton().Apply(player, style, CurrentColor(player))) {
                return false;
            }

            g_currentSelections[regionString] = index;
            return true;
        }

        bool ApplyStyle(RE::StaticFunctionTag*, RE::BSFixedString region, std::int32_t index, bool female)
        {
            return ApplyStyleInternal(ToString(region), index, female);
        }

        std::int32_t GetColorCount(RE::StaticFunctionTag*)
        {
            return static_cast<std::int32_t>(HairColor::PresetCount() + 1);
        }

        RE::BSFixedString GetColorName(RE::StaticFunctionTag*, std::int32_t index)
        {
            if (index <= 0) {
                return RE::BSFixedString("Match Hair");
            }

            const auto zeroBased = static_cast<std::size_t>(index - 1);
            if (zeroBased >= kColorNames.size()) {
                return RE::BSFixedString("Invalid");
            }
            return RE::BSFixedString(kColorNames[zeroBased].data());
        }

        std::int32_t GetCurrentColorIndex(RE::StaticFunctionTag*)
        {
            return g_currentColorIndex;
        }

        bool SetColorIndex(RE::StaticFunctionTag*, std::int32_t index)
        {
            const auto maxIndex = static_cast<std::int32_t>(HairColor::PresetCount());
            if (index < 0 || index > maxIndex) {
                return false;
            }

            g_currentColorIndex = index;
            if (!RaceMenuIntegration::GetSingleton().IsLegacy()) {
                ReapplyPlayerSelections();
            }
            return true;
        }
    }

    void ReapplyPlayerSelections()
    {
        if (RaceMenuIntegration::GetSingleton().IsLegacy()) {
            return;
        }

        const auto selections = g_currentSelections;
        for (const auto& [region, index] : selections) {
            if (index > 0) {
                ApplyStyleInternal(region, index, g_currentFemale);
            }
        }
    }

    bool Register(RE::BSScript::IVirtualMachine* vm)
    {
        if (!vm) {
            return false;
        }

        constexpr auto script = "BodyHairSliders"sv;
        vm->RegisterFunction("GetStyleCount", script, GetStyleCount);
        vm->RegisterFunction("GetStyleName", script, GetStyleName);
        vm->RegisterFunction("GetCurrentStyleIndex", script, GetCurrentStyleIndex);
        vm->RegisterFunction("ApplyStyle", script, ApplyStyle);
        vm->RegisterFunction("IsLegacySKEE", script, IsLegacySKEE);
        vm->RegisterFunction("GetStyleTexture", script, GetStyleTexture);
        vm->RegisterFunction("GetStyleLocation", script, GetStyleLocation);
        vm->RegisterFunction("FindStyleIndexByTexture", script, FindStyleIndexByTexture);
        vm->RegisterFunction("GetCurrentColorRGB", script, GetCurrentColorRGB);
        vm->RegisterFunction("GetColorCount", script, GetColorCount);
        vm->RegisterFunction("GetColorName", script, GetColorName);
        vm->RegisterFunction("GetCurrentColorIndex", script, GetCurrentColorIndex);
        vm->RegisterFunction("SetColorIndex", script, SetColorIndex);

        SKSE::log::info("Registered BodyHairSliders Papyrus API");
        return true;
    }
}
