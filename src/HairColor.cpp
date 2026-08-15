#include "HairColor.h"

namespace BHS
{
    namespace
    {
        constexpr std::array<RGBA, 10> kPresets{{
            {0.06F, 0.04F, 0.03F, 1.0F}, // Black
            {0.14F, 0.07F, 0.03F, 1.0F}, // Dark brown
            {0.28F, 0.13F, 0.05F, 1.0F}, // Brown
            {0.43F, 0.25F, 0.10F, 1.0F}, // Light brown
            {0.42F, 0.12F, 0.04F, 1.0F}, // Auburn
            {0.55F, 0.10F, 0.03F, 1.0F}, // Red
            {0.70F, 0.48F, 0.18F, 1.0F}, // Dark blonde
            {0.88F, 0.72F, 0.35F, 1.0F}, // Blonde
            {0.93F, 0.86F, 0.65F, 1.0F}, // Platinum
            {0.45F, 0.45F, 0.45F, 1.0F}  // Grey
        }};
    }

    RGBA HairColor::FromActor(const RE::Actor* actor)
    {
        if (!actor) {
            return {};
        }

        const auto* base = actor->GetActorBase();
        if (!base || !base->headRelatedData || !base->headRelatedData->hairColor) {
            return {};
        }

        const auto& color = base->headRelatedData->hairColor->color;
        return {
            static_cast<float>(color.red) / 255.0F,
            static_cast<float>(color.green) / 255.0F,
            static_cast<float>(color.blue) / 255.0F,
            1.0F
        };
    }

    RGBA HairColor::Preset(std::size_t index)
    {
        return index < kPresets.size() ? kPresets[index] : RGBA{};
    }

    std::size_t HairColor::PresetCount()
    {
        return kPresets.size();
    }
}
