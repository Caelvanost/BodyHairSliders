#include "RaceMenuIntegration.h"

namespace BHS
{
    RaceMenuIntegration& RaceMenuIntegration::GetSingleton()
    {
        static RaceMenuIntegration singleton;
        return singleton;
    }

    bool RaceMenuIntegration::Initialize()
    {
        // We deliberately keep the first draft buildable without bundling
        // RaceMenu SDK headers. v0.2 will acquire the SKEE interface through
        // SKSE messaging and register the Body-category controls.
        const auto* raceMenu = RE::TESDataHandler::GetSingleton()->LookupModByName("RaceMenu.esp");
        available_ = raceMenu != nullptr;

        if (available_) {
            SKSE::log::info("RaceMenu.esp detected; SKEE bridge is ready to be implemented");
        } else {
            SKSE::log::warn("RaceMenu.esp not detected");
        }
        return available_;
    }
}
