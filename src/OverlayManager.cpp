#include "OverlayManager.h"

namespace BHS
{
    OverlayManager& OverlayManager::GetSingleton()
    {
        static OverlayManager singleton;
        return singleton;
    }

    bool OverlayManager::Apply(RE::Actor* actor, const OverlayStyle* style, const RGBA& color)
    {
        if (!actor || !style) {
            return false;
        }

        // TODO(v0.2): call the SKEE/RaceMenu NiOverride overlay interface here.
        // Important design constraint: reserve our own overlay keys and never
        // overwrite unrelated RaceMenu body paints.
        SKSE::log::info(
            "Apply draft: actor={:X}, provider={}, region={}, style={}, texture={}, rgba=({:.3f},{:.3f},{:.3f},{:.3f})",
            actor->GetFormID(), style->provider, style->region, style->id, style->texture,
            color.r, color.g, color.b, color.a);
        return true;
    }

    bool OverlayManager::Clear(RE::Actor* actor, std::string_view region)
    {
        if (!actor) {
            return false;
        }
        SKSE::log::info("Clear draft: actor={:X}, region={}", actor->GetFormID(), region);
        return true;
    }

    void OverlayManager::Refresh(RE::Actor* actor)
    {
        if (!actor) {
            return;
        }
        SKSE::log::debug("Refresh draft for actor {:X}", actor->GetFormID());
    }
}
