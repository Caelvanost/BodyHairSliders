#pragma once
#include "HairColor.h"
#include "Settings.h"

namespace BHS
{
    class OverlayManager
    {
    public:
        static OverlayManager& GetSingleton();

        bool Apply(RE::Actor* actor, const OverlayStyle* style, const RGBA& color);
        bool Clear(RE::Actor* actor, std::string_view region);
        void Refresh(RE::Actor* actor);

    private:
        std::optional<std::uint32_t> FindOrReserveBodySlot(RE::Actor* actor, std::string_view region, bool isFemale);
        std::unordered_map<std::uint32_t, std::unordered_map<std::string, std::uint32_t>> slots_;
    };
}
