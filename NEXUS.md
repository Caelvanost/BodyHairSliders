# Nexus Mods publication pack — BodyHairSliders v0.10.2

## Mod name

**Body Hair Sliders - RaceMenu Body Hair Integration**

## One-line summary

Adds unified RaceMenu sliders for compatible male and female body-hair overlay packs, with automatic legacy/modern RaceMenu backend selection.

## Short description

Body Hair Sliders provides a unified RaceMenu frontend for compatible third-party body-hair overlays. v0.10.2 keeps the unified legacy/modern backend and provider support while replacing broad modern SKEE refreshes with targeted Body/Hands/Feet node updates to address crash reports and cumulative RaceMenu face distortion.

## Compatibility

### Supported / intended

- **Skyrim SE 1.5.97** — legacy SKEE v1 path using RaceMenu's `NiOverride` Papyrus API; field-tested successfully by users.
- **Skyrim SE/AE 1.6.1170** — modern SKEE v2+ C++ wrapper backend.
- **Skyrim 1.6.640** — dedicated compatibility testing has shown RaceMenu 0.4.19.14 may expose `Overlay v1 / Override v1`; the unified architecture routes that ABI through the legacy NiOverride backend.

The user does not select a Skyrim version in the FOMOD. BodyHairSliders detects the RaceMenu/SKEE interface ABI at runtime:

```text
Overlay v1 + Override v1 -> legacy-papyrus
Overlay v2+ + Override v2+ -> modern
```

## Main description

**Body Hair Sliders** integrates compatible third-party body-hair overlays directly into RaceMenu's native **Hair** category.

Depending on installed providers it can expose:

- Full Body Hair
- Pubic Hair
- Armpit Hair
- Chest Hair
- Stomach Hair
- Back Hair
- Arm Hair
- Hand Hair
- Leg Hair
- Foot Hair
- Butt Hair
- Body Hair Color

Every region includes **None / Shaved** as position 0. `Body Hair Color` includes **Match Hair** plus built-in presets.

Existing supported overlays are detected when RaceMenu opens so the sliders reflect the currently applied style instead of resetting to zero.

## Supported providers

The v0.10.2 FOMOD supports:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility
- More Pubes for SlaveTats
- Natural Pubic Hairstyles — standard 2K/4K
- Natural Pubic Hairstyles - UBE — UBE 2K/4K
- **Body Hair Overlays for Male and Female — BH extra variants**

BodyHairSliders does **not** redistribute provider textures or meshes. Install the original provider mods and select only the providers present in your setup.

### Body Hair Overlays for Male and Female

The **BH extra variants** archive is detected through `BH_Body.esp` and supplies six external DDS files:

- Female Full Body
- Female Arms
- Female Legs
- Male Full Body
- Male Arms
- Male Legs

BodyHairSliders exposes the full-body variants through **Full Body Hair** and maps the regional variants to **Arm Hair** and **Leg Hair**.

## Legacy color performance

v0.10.1 optimized `Body Hair Color` for the `legacy-papyrus` backend. BodyHairSliders caches active legacy region selections when RaceMenu opens and whenever a body-hair slider changes. Color changes update only active overlays and perform one final `NiOverride.ApplyNodeOverrides()` refresh instead of rescanning every Body/Hands/Feet slot for every region and refreshing repeatedly.

## Modern SKEE stability fix — v0.10.2

Crash reports on the modern SKEE v2+ backend showed RaceMenu/SKEE inside overlay installation/update work, including Hand overlay rebuilds on unrelated actors. Another report described cumulative face distortion after repeatedly changing BodyHairSliders sliders, which disappeared after reopening RaceMenu.

Previous modern builds used broad actor refresh operations after each BodyHairSliders change:

```text
SetNodeProperties(actor, true)
AddOverlayUpdate(actor)
AddNodeOverrideUpdate(actor)
Flush()
```

Those APIs can rebuild unrelated RaceMenu node state and `Flush()` can process work already queued by other plugins or actors.

v0.10.2 removes that global refresh path for normal BodyHairSliders changes. The modern backend now:

- persists only the BodyHairSliders-owned overlay node overrides;
- applies texture, tint and alpha through targeted `SetNodeProperty()` calls;
- no longer calls `SetNodeProperties()` after each slider change;
- no longer calls `AddNodeOverrideUpdate()` or `ActorUpdateManager::Flush()` for normal style changes;
- no longer calls `AddOverlays()` on every slider movement;
- clears `None / Shaved` by hiding only the affected Body/Hands/Feet overlay node.

Modern apply log entries now end with:

```text
refresh=targeted
```

This change is specifically intended to prevent BodyHairSliders from forcing full actor-node rebuilds or flushing unrelated SKEE work.

## Requirements

- SKSE64 matching your Skyrim runtime
- Address Library for SKSE Plugins matching your runtime
- RaceMenu matching your runtime
- at least one supported body-hair provider for styles to appear

## Installation

1. Install SKSE64, Address Library and RaceMenu for your Skyrim runtime.
2. Install one or more supported body-hair provider mods.
3. Install `BodyHairSliders-v0.10.2-FOMOD.zip` with Vortex or another FOMOD-capable manager.
4. Select the provider packs actually installed.
5. Optionally select Extended Overlay Slots if your RaceMenu configuration needs more Body/Hands/Feet slots.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and open RaceMenu with `showracemenu`.
8. Open **Hair**.

## Diagnostics

Log path:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

A v0.10.2 log should begin with lines similar to:

```text
BodyHairSliders v0.10.2 loading - unified runtime build
Detected Skyrim runtime: ...
```

Legacy RaceMenu/SKEE:

```text
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=...
Selected SKEE backend: legacy-papyrus (...)
```

Modern RaceMenu/SKEE:

```text
SKEE interface versions: Overlay=2 Override=2 ActorUpdate=2
Selected SKEE backend: modern (...)
Applied provider=... refresh=targeted
```

When reporting an issue, include the full log plus exact Skyrim, SKSE and RaceMenu versions. For modern-backend reports, please also state whether the issue happens when applying a style, changing Body Hair Color, selecting None / Shaved, or closing/reopening RaceMenu.

## Version 0.10.2 changelog

- Reworked the modern SKEE v2+ refresh path after crash and face-distortion reports.
- Removed global `SetNodeProperties()` calls from normal modern style application.
- Removed `ActorUpdateManager::AddNodeOverrideUpdate()` and `Flush()` from normal modern BodyHairSliders changes.
- Stopped calling `AddOverlays()` on every modern slider movement.
- Uses targeted `SetNodeProperty()` updates for the exact BodyHairSliders overlay node only.
- `None / Shaved` now hides only the affected modern overlay node instead of rebuilding all actor node overrides.
- Prevents BodyHairSliders from flushing unrelated RaceMenu/SKEE updates queued by other mods or actors.
- Added `refresh=targeted` diagnostics to modern overlay operations.

## Version 0.10.1 changelog

- Optimized `Body Hair Color` updates on the legacy NiOverride backend.
- Cached active legacy selections to avoid repeated Body/Hands/Feet overlay scans.
- Recolors only active BodyHairSliders regions.
- Batches legacy recoloring into a single `NiOverride.ApplyNodeOverrides()` refresh.

## Version 0.10.0 changelog

- Added **Body Hair Overlays for Male and Female — BH extra variants** provider support.
- Added new **Full Body Hair** RaceMenu slider.
- Added male and female full-body mappings.
- Added male and female Arm Hair and Leg Hair mappings for the BH pack.
- Added automatic FOMOD recommendation when `BH_Body.esp` is active.

## Recommended Nexus metadata

**Game:** Skyrim Special Edition

**Category:** Body, Face, and Hair

Suggested tags:

- RaceMenu
- Character Appearance
- Body
- Hair
- SKSE

**Version:** 0.10.2

**Main file name:**

`BodyHairSliders-v0.10.2-FOMOD.zip`

**Main file label:**

`Body Hair Sliders v0.10.2 - Universal FOMOD`

**Main file description:**

Unified installer with automatic SKEE backend selection. v0.10.2 replaces broad modern RaceMenu/SKEE actor refreshes with targeted BodyHairSliders overlay-node updates to improve stability and prevent cumulative face distortion.

## Publication checklist

- [ ] Build from `dev/unified-runtime-support` with `VERSION = 0.10.2`.
- [ ] Run `build_release.bat`.
- [ ] Verify `dist/BodyHairSliders-v0.10.2-FOMOD.zip` installs cleanly.
- [ ] Confirm FOMOD detects/recommends **Body Hair Overlays for Male and Female** when `BH_Body.esp` is active.
- [ ] On modern SKEE v2+, repeatedly drag several BodyHairSliders style sliders and confirm the face does not progressively deform.
- [ ] Test Body, Hands and Feet providers on the modern backend.
- [ ] Test `None / Shaved` and confirm the affected overlay disappears immediately.
- [ ] Test `Body Hair Color`, especially HIMBO styles.
- [ ] Confirm modern log entries report `refresh=targeted`.
- [ ] Play/load cells with NPCs using RaceMenu/TNG overlays and verify no SKEE overlay-install crash recurs.
- [ ] Re-test legacy SKEE v1 to confirm the v0.10.1 recoloring optimization still behaves normally.
