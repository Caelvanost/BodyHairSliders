# BodyHairSliders

BodyHairSliders is a Skyrim SE/AE SKSE plugin that exposes compatible third-party body-hair overlays through RaceMenu's native **Hair** category.

> **Compatibility test branch:** `compat/skyrim-1.6.640-racemenu-0.4.19.14`
>
> **v0.8.2 targets Skyrim SE/AE 1.6.640 with RaceMenu 0.4.19.14.** The DLL deliberately refuses to load on another Skyrim runtime so compatibility reports remain isolated.

BodyHairSliders does **not** redistribute provider textures or meshes.

## v0.8.2 performance fix

The SKEE v1 / `legacy-papyrus` backend now caches the active BodyHairSliders style for each body-hair region while RaceMenu is open.

Previously, changing `Body Hair Color` rescanned Body, Hands and Feet overlay slots separately for every possible region and could call `NiOverride.ApplyNodeOverrides()` repeatedly. On Skyrim 1.6.640 + RaceMenu 0.4.19.14 this could cause multi-second stalls while moving the color slider.

v0.8.2 now:

- remembers the active style index for each BodyHairSliders region;
- recolors only regions that currently have an active BodyHairSliders overlay;
- skips full overlay rescans during color changes;
- batches all tint/texture updates;
- calls `NiOverride.ApplyNodeOverrides()` only once after the batch.

Normal style changes still refresh immediately.

## v0.8.1 compatibility fix

A real Skyrim 1.6.640 + RaceMenu 0.4.19.14 installation reported:

```text
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=1
```

v0.8.0 incorrectly assumed RaceMenu 0.4.19.14 would expose the wrapper-generation `Overlay v2` / `Override v2` interfaces and disabled overlay application when v1 was detected.

v0.8.1+ queries the ABI-stable SKEE base interface first and selects a backend from the actual interface versions:

```text
Overlay v1 + Override v1
  -> legacy-papyrus backend using RaceMenu NiOverride natives

Overlay v2+ + Override v2+
  -> modern C++ SKEE wrapper backend
```

SKEE v1 objects are never cast to the modern wrapper interfaces.

Expected log on the reported 1.6.640 configuration:

```text
BodyHairSliders v0.8.2 loading - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST
Compatibility target: Skyrim 1-6-640-0 / RaceMenu 0.4.19.14
Detected Skyrim runtime: 1-6-640-0
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=1
Selected SKEE backend: legacy-papyrus (Overlay v1 Override v1 ActorUpdate v1)
```

If a compatible installation exposes v2+ wrapper interfaces, the modern backend remains available.

## RaceMenu controls

Only regions supplied by detected providers are shown:

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

Each region uses `0 = None / Shaved`. `Body Hair Color` includes `Match Hair` and built-in presets.

## Supported providers

The v0.8.2 test FOMOD includes:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility
- More Pubes for SlaveTats
- Natural Pubic Hairstyles — standard 2K/4K
- Natural Pubic Hairstyles - UBE — UBE 2K/4K

## Legacy-Papyrus backend

For SKEE v1, BodyHairSliders uses RaceMenu's `NiOverride` Papyrus natives for:

- Body / Hands / Feet overlay counts;
- current texture detection;
- free slot reuse;
- texture, tint and alpha application;
- `None / Shaved` clearing;
- color reapplication.

`compiler_stubs/NiOverride.psc` exists **only for compilation** and is not packaged as a replacement for RaceMenu's own script.

## Modern backend

For `Overlay >= 2` and `Override >= 2`, BodyHairSliders uses the native C++ SKEE wrapper interfaces for overlay discovery, texture/tint/alpha overrides and refresh work.

## Requirements

- Skyrim **1.6.640**
- matching SKSE64
- Address Library for 1.6.640
- RaceMenu **0.4.19.14**
- one or more supported body-hair provider mods

## Installation

1. Install SKSE64, Address Library and RaceMenu versions matching Skyrim 1.6.640.
2. Install one or more supported body-hair provider mods.
3. Install `BodyHairSliders-v0.8.2-SE-1.6.640-RM-0.4.19.14-TEST-FOMOD.zip`.
4. Select only providers actually installed.
5. Optionally enable Extended Overlay Slots.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and run `showracemenu`.
8. Test Hair sliders, `None / Shaved`, repeated color changes, and reopening RaceMenu.

## Diagnostics

Log path:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

When reporting an issue, include the full log and exact Skyrim, SKSE and RaceMenu versions.

## Build

```bat
build_release.bat
```

The version comes from `VERSION`. The build compiles C++, compiles both Papyrus scripts, verifies their `.pex` outputs, validates the FOMOD and produces:

```text
dist/BodyHairSliders-v0.8.2-SE-1.6.640-RM-0.4.19.14-TEST-FOMOD.zip
```

## Changelog

### v0.8.2

- Optimized Body Hair Color changes on the SKEE v1 / legacy-papyrus backend.
- Added per-region active-style caching while RaceMenu is open.
- Removed repeated Body/Hands/Feet rescans during color changes.
- Recolors only active BodyHairSliders overlays.
- Batches legacy recoloring into a single `NiOverride.ApplyNodeOverrides()` refresh.

### v0.8.1

- Fixed Skyrim 1.6.640 + RaceMenu 0.4.19.14 installations exposing SKEE `Overlay v1 / Override v1`.
- Added safe automatic backend selection between SKEE v1 and v2+.
- Added NiOverride Papyrus fallback for v1 without calling incompatible C++ vtables.
- Preserved strict Skyrim 1.6.640 runtime locking for this test branch.

### v0.8.0

- Added dedicated Skyrim 1.6.640 / RaceMenu 0.4.19.14 compatibility target.
- Added exact runtime validation and SKEE interface diagnostics.
