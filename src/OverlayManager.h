#pragma once
#include "HairColor.h"
#include "Settings.h"

namespace NBHS
{
    enum class Region
    {
        kPubic,
        kArmpits,
        kButt
    };

    class OverlayManager
    {
    public:
        static OverlayManager& GetSingleton();

        // Draft API. The RaceMenu/SKEE bridge will be wired into these methods
        // once the exact interface and Nordic Warmaiden texture names are confirmed.
        bool Apply(RE::Actor* actor, Region region, const OverlayStyle* style, const RGBA& color);
        bool Clear(RE::Actor* actor, Region region);
        void Refresh(RE::Actor* actor);
    };
}
