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
        backend_ = Backend::Unavailable;
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

        // Query through the ABI-stable IPluginInterface base first. GetVersion() has
        // occupied the same base vtable slot since the legacy 1.5.97-era SKEE API.
        // Never cast a v1 object to the modern wrapper interfaces: their vtables are
        // incompatible and doing so can dispatch into unrelated functions and CTD.
        auto* overlayBase = exchange.interfaceMap->QueryInterface("Overlay");
        auto* overrideBase = exchange.interfaceMap->QueryInterface("Override");
        auto* actorUpdateBase = exchange.interfaceMap->QueryInterface("ActorUpdateManager");

        if (!overlayBase || !overrideBase) {
            SKSE::log::error("Required SKEE interfaces missing (Overlay={} Override={})",
                static_cast<const void*>(overlayBase), static_cast<const void*>(overrideBase));
            return false;
        }

        const auto overlayVersion = overlayBase->GetVersion();
        const auto overrideVersion = overrideBase->GetVersion();
        const auto actorUpdateVersion = actorUpdateBase ? actorUpdateBase->GetVersion() : 0U;

        SKSE::log::info("SKEE interface versions: Overlay={} Override={} ActorUpdate={}",
            overlayVersion, overrideVersion, actorUpdateVersion);

        if (overlayVersion >= 2 && overrideVersion >= 2) {
            overlay_ = static_cast<SKEE::IOverlayInterface*>(overlayBase);
            override_ = static_cast<SKEE::IOverrideInterface*>(overrideBase);
            if (actorUpdateBase && actorUpdateVersion >= 2) {
                actorUpdate_ = static_cast<SKEE::IActorUpdateManager*>(actorUpdateBase);
            }
            backend_ = Backend::Modern;

            SKSE::log::info(
                "Selected SKEE backend: modern (Overlay v{} Override v{} ActorUpdate v{})",
                overlayVersion, overrideVersion, actorUpdateVersion);
            return true;
        }

        if (overlayVersion == 1 && overrideVersion == 1) {
            // RaceMenu 0.4.16 / Skyrim 1.5.97 uses the original SKEE C++ ABI.
            // BodyHairSliders deliberately does not call through those old C++
            // vtables. The RaceMenu frontend uses the stable legacy NiOverride
            // Papyrus natives instead.
            backend_ = Backend::LegacyPapyrus;
            SKSE::log::info(
                "Selected SKEE backend: legacy-papyrus (Overlay v{} Override v{} ActorUpdate v{})",
                overlayVersion, overrideVersion, actorUpdateVersion);
            return true;
        }

        SKSE::log::error(
            "Unsupported SKEE interface combination: Overlay v{} Override v{} ActorUpdate v{}",
            overlayVersion, overrideVersion, actorUpdateVersion);
        return false;
    }
}
