#pragma once
#include "HairColor.h"
#include "Settings.h"

namespace BHS
{
    class OverlayManager
    {
    public:
        static OverlayManager& GetSingleton();

        // Draft API. The RaceMenu/SKEE bridge will be wired into these methods
        // once the exact interface and provider asset paths are confirmed.
        bool Apply(RE::Actor* actor, const OverlayStyle* style, const RGBA& color);
        bool Clear(RE::Actor* actor, std::string_view region);
        void Refresh(RE::Actor* actor);
    };
}
