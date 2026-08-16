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
        struct ReservedSlot
        {
            std::uint32_t index{ 0 };
            std::string location{ "body" };
        };

        std::optional<std::uint32_t> FindOrReserveSlot(
            RE::Actor* actor,
            std::string_view region,
            std::string_view location,
            bool isFemale);

        std::unordered_map<std::uint32_t, std::unordered_map<std::string, ReservedSlot>> slots_;
    };
}
