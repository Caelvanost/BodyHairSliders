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

        overlay_ = static_cast<SKEE::IOverlayInterface*>(exchange.interfaceMap->QueryInterface("Overlay"));
        override_ = static_cast<SKEE::IOverrideInterface*>(exchange.interfaceMap->QueryInterface("Override"));
        actorUpdate_ = static_cast<SKEE::IActorUpdateManager*>(exchange.interfaceMap->QueryInterface("ActorUpdateManager"));

        if (!overlay_ || !override_) {
            SKSE::log::error("Required SKEE interfaces missing (Overlay={} Override={})",
                static_cast<const void*>(overlay_), static_cast<const void*>(override_));
            return false;
        }

        SKSE::log::info("SKEE acquired: Overlay v{}, Override v{}, ActorUpdateManager={}",
            overlay_->GetVersion(), override_->GetVersion(), static_cast<const void*>(actorUpdate_));

        available_ = true;
        return true;
    }
}
