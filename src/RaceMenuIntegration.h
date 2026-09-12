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

        // Initializes the SKEE interface exchange once, on demand. This must not be
        // called during normal save loading; RaceMenu/BodyHairSliders Papyrus calls
        // trigger it when the RaceMenu sliders are actually opened.
        bool EnsureInitialized();
        bool IsInitialized() const noexcept { return initialized_; }

        // Backend queries are intentionally lazy. The first RaceMenu/Papyrus use
        // performs the interface exchange; normal save loading does not touch SKEE.
        bool IsAvailable() { return EnsureInitialized() && backend_ == Backend::Modern; }
        bool IsModern() { return EnsureInitialized() && backend_ == Backend::Modern; }
        bool IsLegacy() { return EnsureInitialized() && backend_ == Backend::LegacyPapyrus; }
        Backend GetBackend() const noexcept { return backend_; }

        SKEE::IOverlayInterface* Overlay() const noexcept { return overlay_; }
        SKEE::IOverrideInterface* Override() const noexcept { return override_; }
        SKEE::IActorUpdateManager* ActorUpdate() const noexcept { return actorUpdate_; }

    private:
        bool initialized_{ false };
        Backend backend_{ Backend::Unavailable };
        SKEE::IOverlayInterface* overlay_{ nullptr };
        SKEE::IOverrideInterface* override_{ nullptr };
        SKEE::IActorUpdateManager* actorUpdate_{ nullptr };
    };
}
