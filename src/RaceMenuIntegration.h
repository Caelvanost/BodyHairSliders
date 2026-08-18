#pragma once
#include "SKEEInterfaces.h"

namespace BHS
{
    class RaceMenuIntegration
    {
    public:
        enum class Backend
        {
            Unavailable,
            LegacyPapyrus,
            Modern
        };

        static RaceMenuIntegration& GetSingleton();
        bool Initialize();

        // The C++ overlay renderer may only use the modern wrapper ABI.
        bool IsAvailable() const noexcept { return backend_ == Backend::Modern; }
        bool IsModern() const noexcept { return backend_ == Backend::Modern; }
        bool IsLegacy() const noexcept { return backend_ == Backend::LegacyPapyrus; }
        Backend GetBackend() const noexcept { return backend_; }

        SKEE::IOverlayInterface* Overlay() const noexcept { return overlay_; }
        SKEE::IOverrideInterface* Override() const noexcept { return override_; }
        SKEE::IActorUpdateManager* ActorUpdate() const noexcept { return actorUpdate_; }

    private:
        Backend backend_{ Backend::Unavailable };
        SKEE::IOverlayInterface* overlay_{ nullptr };
        SKEE::IOverrideInterface* override_{ nullptr };
        SKEE::IActorUpdateManager* actorUpdate_{ nullptr };
    };
}
