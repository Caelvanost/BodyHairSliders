# Nexus Mods publication pack — BodyHairSliders v0.9.0

## Mod name

**Body Hair Sliders - RaceMenu Body Hair Integration**

## One-line summary

Adds unified RaceMenu sliders for compatible male and female body-hair overlay packs, with automatic legacy/modern RaceMenu backend selection.

## Short description

Body Hair Sliders provides a unified RaceMenu frontend for compatible third-party body-hair overlays. v0.9.0 ships one FOMOD and one DLL for Skyrim 1.5.97 and modern Skyrim, automatically selecting the safe RaceMenu/SKEE backend at runtime.

## Compatibility

### Supported in v0.9.0

- **Skyrim SE 1.5.97** — legacy SKEE v1 path using RaceMenu's `NiOverride` Papyrus API. This path was field-tested successfully by users in v0.7.2.
- **Skyrim SE/AE 1.6.1170** — modern SKEE v2+ C++ wrapper backend.

### Planned validation

- **Skyrim 1.6.640** — dedicated compatibility work exists and will be folded into the supported matrix after field validation.

The user does not select a Skyrim version in the FOMOD. BodyHairSliders detects the RaceMenu/SKEE interface ABI at runtime:

```text
Overlay v1 + Override v1 -> legacy-papyrus
Overlay v2+ + Override v2+ -> modern
```

This avoids calling legacy SKEE objects through the incompatible modern vtable.

## Main description

**Body Hair Sliders** integrates compatible third-party body-hair overlays directly into RaceMenu's native **Hair** category.

Depending on installed providers it can expose:

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

The v0.9.0 FOMOD supports:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility
- More Pubes for SlaveTats
- Natural Pubic Hairstyles — standard 2K/4K
- Natural Pubic Hairstyles - UBE — UBE 2K/4K

BodyHairSliders does **not** redistribute provider textures or meshes. Install the original provider mods and select only the providers present in your setup.

## Requirements

- SKSE64 matching your Skyrim runtime
- Address Library for SKSE Plugins matching your runtime
- RaceMenu matching your runtime
- at least one supported body-hair provider for styles to appear

For Skyrim 1.5.97, use the appropriate 1.5.97-era SKSE, Address Library and RaceMenu versions. For modern Skyrim, use the corresponding modern versions.

## Installation

1. Install SKSE64, Address Library and RaceMenu for your Skyrim runtime.
2. Install one or more supported body-hair provider mods.
3. Install `BodyHairSliders-v0.9.0-FOMOD.zip` with Vortex or another FOMOD-capable manager.
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

A v0.9.0 log should begin with lines similar to:

```text
BodyHairSliders v0.9.0 loading - unified runtime build
Detected Skyrim runtime: ...
```

Legacy RaceMenu/SKEE:

```text
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=0
Selected SKEE backend: legacy-papyrus (...)
```

Modern RaceMenu/SKEE:

```text
SKEE interface versions: Overlay=2 Override=2 ActorUpdate=2
Selected SKEE backend: modern (...)
```

When reporting an issue, include the full log plus exact Skyrim, SKSE and RaceMenu versions.

## Version 0.9.0 changelog

- Unified Skyrim 1.5.97 legacy support and the modern RaceMenu/SKEE backend into one DLL and one FOMOD archive.
- Added safe SKEE ABI detection before interface casting.
- Selects the field-tested `legacy-papyrus` renderer for SKEE v1/v1.
- Selects the existing native C++ renderer for SKEE v2+.
- Added detected Skyrim runtime and selected backend to diagnostics.
- Hardened Papyrus build verification before FOMOD packaging.
- Preserved all existing provider support and the native RaceMenu Hair-tab UI.

## Recommended Nexus metadata

**Game:** Skyrim Special Edition

**Category:** Body, Face, and Hair

Suggested tags:

- RaceMenu
- Character Appearance
- Body
- Hair
- SKSE

**Version:** 0.9.0

**Main file name:**

`BodyHairSliders-v0.9.0-FOMOD.zip`

**Main file label:**

`Body Hair Sliders v0.9.0 - Universal FOMOD`

**Main file description:**

Unified installer for Skyrim SE 1.5.97 and modern Skyrim. Automatically selects the compatible RaceMenu/SKEE backend at runtime. Select only the body-hair provider packs installed in your setup.

## Publication checklist

- [ ] Build from `dev/unified-runtime-support` with `VERSION = 0.9.0`.
- [ ] Run `build_release.bat`.
- [ ] Verify `dist/BodyHairSliders-v0.9.0-FOMOD.zip` installs cleanly.
- [ ] Confirm log reports `v0.9.0` and the detected Skyrim runtime.
- [ ] Re-test on Skyrim 1.6.1170.
- [ ] Have a 1.5.97 tester confirm `Selected SKEE backend: legacy-papyrus` and normal slider behavior.
- [ ] Validate 1.6.640 separately before adding it to the officially supported runtime list.
- [ ] Test reopening RaceMenu preserves/detects existing slider positions.
- [ ] Test `None / Shaved`, color changes and representative Body/Hands/Feet overlays.
