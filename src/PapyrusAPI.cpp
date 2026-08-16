#include "PapyrusAPI.h"

#include "HairColor.h"
#include "OverlayManager.h"
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

        RGBA CurrentColor(RE::Actor* actor)
        {
            if (g_currentColorIndex <= 0) {
                return HairColor::FromActor(actor);
            }
            return HairColor::Preset(static_cast<std::size_t>(g_currentColorIndex - 1));
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

            const auto styles = Settings::GetSingleton().GetStylesForRegion(ToString(region), SexName(female));
            const auto zeroBased = static_cast<std::size_t>(index - 1);
            if (zeroBased >= styles.size()) {
                return RE::BSFixedString("Invalid");
            }

            const auto* style = styles[zeroBased];
            const auto label = style->provider + " - " + style->label;
            return RE::BSFixedString(label.c_str());
        }

        std::int32_t GetCurrentStyleIndex(RE::StaticFunctionTag*, RE::BSFixedString region, bool female)
        {
            const auto regionName = ToString(region);
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
            ReapplyPlayerSelections();
            return true;
        }
    }

    void ReapplyPlayerSelections()
    {
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
        vm->RegisterFunction("GetColorCount", script, GetColorCount);
        vm->RegisterFunction("GetColorName", script, GetColorName);
        vm->RegisterFunction("GetCurrentColorIndex", script, GetCurrentColorIndex);
        vm->RegisterFunction("SetColorIndex", script, SetColorIndex);

        SKSE::log::info("Registered BodyHairSliders Papyrus API");
        return true;
    }
}
