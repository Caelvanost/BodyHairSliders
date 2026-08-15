#pragma once

namespace BHS
{
    struct RGBA
    {
        float r{ 1.0F };
        float g{ 1.0F };
        float b{ 1.0F };
        float a{ 1.0F };
    };

    class HairColor
    {
    public:
        static RGBA FromActor(const RE::Actor* actor);
        static RGBA Preset(std::size_t index);
        static std::size_t PresetCount();
    };
}
