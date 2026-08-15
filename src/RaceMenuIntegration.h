#pragma once
#include "SKEEInterfaces.h"

namespace BHS
{
    class RaceMenuIntegration
    {
    public:
        static RaceMenuIntegration& GetSingleton();
        bool Initialize();
        bool IsAvailable() const noexcept { return available_; }

        SKEE::IOverlayInterface* Overlay() const noexcept { return overlay_; }
        SKEE::IOverrideInterface* Override() const noexcept { return override_; }
        SKEE::IActorUpdateManager* ActorUpdate() const noexcept { return actorUpdate_; }

    private:
        bool available_{ false };
        SKEE::IOverlayInterface* overlay_{ nullptr };
        SKEE::IOverrideInterface* override_{ nullptr };
        SKEE::IActorUpdateManager* actorUpdate_{ nullptr };
    };
}
