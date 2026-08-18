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
        available_ = false;
        overlay_ = nullptr;
        override_ = nullptr;
        actorUpdate_ = nullptr;

        const auto* raceMenu = RE::TESDataHandler::GetSingleton()->LookupModByName("RaceMenu.esp");
        if (!raceMenu) {
            SKSE::log::warn("RaceMenu.esp not detected");
            return false;
        }

        const auto messaging = SKSE::GetMessagingInterface();
        if (!messaging) {
            SKSE::log::error("SKSE messaging interface unavailable");
            return false;
        }

        SKEE::InterfaceExchangeMessage exchange{};
        messaging->Dispatch(
            SKEE::InterfaceExchangeMessage::kMessage_ExchangeInterface,
            &exchange,
            sizeof(exchange),
            nullptr);

        if (!exchange.interfaceMap) {
            SKSE::log::error("SKEE interface exchange failed");
            return false;
        }

        // RaceMenu 0.4.19.14 exposes the wrapper-generation SKEE interfaces.
        // Query through the stable base interface first so we never dispatch through
        // an incompatible vtable if a user accidentally installs another RaceMenu build.
        auto* overlayBase = exchange.interfaceMap->QueryInterface("Overlay");
        auto* overrideBase = exchange.interfaceMap->QueryInterface("Override");
        auto* actorUpdateBase = exchange.interfaceMap->QueryInterface("ActorUpdateManager");

        if (!overlayBase || !overrideBase) {
            SKSE::log::error(
                "Required SKEE interfaces missing (Overlay={} Override={})",
                static_cast<const void*>(overlayBase),
                static_cast<const void*>(overrideBase));
            return false;
        }

        const auto overlayVersion = overlayBase->GetVersion();
        const auto overrideVersion = overrideBase->GetVersion();
        const auto actorUpdateVersion = actorUpdateBase ? actorUpdateBase->GetVersion() : 0U;

        SKSE::log::info(
            "SKEE interface versions: Overlay={} Override={} ActorUpdate={}",
            overlayVersion,
            overrideVersion,
            actorUpdateVersion);

        constexpr std::uint32_t kExpectedWrapperVersion = 2;
        if (overlayVersion != kExpectedWrapperVersion || overrideVersion != kExpectedWrapperVersion) {
            SKSE::log::error(
                "Unsupported RaceMenu/SKEE ABI for the Skyrim 1.6.640 test build: Overlay v{} Override v{} (expected v2/v2 from RaceMenu 0.4.19.14)",
                overlayVersion,
                overrideVersion);
            return false;
        }

        overlay_ = static_cast<SKEE::IOverlayInterface*>(overlayBase);
        override_ = static_cast<SKEE::IOverrideInterface*>(overrideBase);

        if (actorUpdateBase && actorUpdateVersion == kExpectedWrapperVersion) {
            actorUpdate_ = static_cast<SKEE::IActorUpdateManager*>(actorUpdateBase);
        } else if (actorUpdateBase) {
            SKSE::log::warn(
                "ActorUpdateManager v{} does not match expected v2; refresh calls will use available fallback paths",
                actorUpdateVersion);
        }

        available_ = true;
        SKSE::log::info(
            "SKEE acquired: Overlay v{} Override v{} ActorUpdate v{} backend=rm-0.4.19.14-wrapper",
            overlayVersion,
            overrideVersion,
            actorUpdateVersion);
        return true;
    }
}
