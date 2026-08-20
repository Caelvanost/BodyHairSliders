# BodyHairSliders

BodyHairSliders is a Skyrim SE/AE SKSE plugin that exposes compatible third-party body-hair overlays through a unified RaceMenu frontend.

The mod does **not** redistribute body-hair textures or meshes. Supported body-hair mods remain external asset providers.

## Current version

**v0.10.1 — unified runtime support + optimized legacy recoloring**

One archive and one DLL support both the legacy RaceMenu/SKEE v1 path and the modern SKEE v2+ path.

### Runtime compatibility

Validated / intended targets:

- **Skyrim SE 1.5.97** — legacy RaceMenu/SKEE v1 backend through RaceMenu `NiOverride` Papyrus natives.
- **Skyrim SE/AE 1.6.1170** — modern RaceMenu/SKEE v2+ C++ backend.
- **Skyrim 1.6.640** — dedicated compatibility testing has shown RaceMenu 0.4.19.14 may expose `Overlay v1 / Override v1`; the unified backend architecture is designed to route that ABI safely through the same legacy NiOverride path.

BodyHairSliders queries RaceMenu/SKEE at runtime and selects the safe backend from the interface ABI:

```text
SKEE Overlay v1 + Override v1
  -> legacy-papyrus backend

SKEE Overlay v2+ + Override v2+
  -> modern C++ backend
```

## RaceMenu controls

BodyHairSliders integrates directly into RaceMenu's native **Hair** category and dynamically exposes only regions for which compatible styles are detected:

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

Each region uses `0 = None / Shaved`, followed by the detected styles. `Body Hair Color` provides `Match Hair` plus built-in color presets.

Existing supported overlays already present on the player are detected and reflected by the corresponding sliders when RaceMenu opens.

## Supported providers

The v0.10.1 FOMOD supports:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility
- More Pubes for SlaveTats
- Natural Pubic Hairstyles — standard 2K/4K variants
- Natural Pubic Hairstyles - UBE — UBE 2K/4K variants
- **Body Hair Overlays for Male and Female — BH extra variants**

Select only the providers actually installed in the current Skyrim setup.

Provider definitions are installed under:

```text
SKSE/Plugins/BodyHairSliders/providers/*.json
```

## Body Hair Overlays for Male and Female

v0.10.0 added support for the **BH extra variants** archive.

The original provider contains:

```text
BH_Body.esp
Data/textures/actors/character/Overlays/Body Hair Overlays/BH_F.dds
Data/textures/actors/character/Overlays/Body Hair Overlays/BH_F_Arms.dds
Data/textures/actors/character/Overlays/Body Hair Overlays/BH_F_Legs.dds
Data/textures/actors/character/Overlays/Body Hair Overlays/BH_M.dds
Data/textures/actors/character/Overlays/Body Hair Overlays/BH_M_Arms.dds
Data/textures/actors/character/Overlays/Body Hair Overlays/BH_M_Legs.dds
```

BodyHairSliders maps these as:

- `BH_F.dds` / `BH_M.dds` -> **Full Body Hair**
- `BH_F_Arms.dds` / `BH_M_Arms.dds` -> **Arm Hair**
- `BH_F_Legs.dds` / `BH_M_Legs.dds` -> **Leg Hair**

Male and female styles are exposed independently. The original provider mod must remain installed; BodyHairSliders only references its DDS files.

## Backend details

### Legacy SKEE v1

The original SKEE v1 `Overlay` and `Override` interfaces are not ABI-compatible with the modern wrapper API. BodyHairSliders queries only the stable base `IPluginInterface` first, reads `GetVersion()`, and never casts v1 objects to the modern wrappers.

For v1/v1, the RaceMenu frontend uses RaceMenu's `NiOverride` Papyrus natives for overlay counts, current texture detection, slot reuse, texture/tint/alpha application, clearing and recoloring.

The legacy recoloring path was optimized in v0.10.1. Active BodyHairSliders regions are cached when RaceMenu builds the sliders and whenever a body-hair slider changes. Changing `Body Hair Color` therefore updates only active cached overlays instead of rescanning every Body/Hands/Feet slot for every region, and performs a single `NiOverride.ApplyNodeOverrides()` refresh after the batch.

This improves responsiveness particularly on SKEE v1 configurations while leaving the modern C++ backend unchanged.

Expected log lines include:

```text
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=...
Selected SKEE backend: legacy-papyrus (...)
```

### Modern SKEE v2+

For `Overlay >= 2` and `Override >= 2`, BodyHairSliders uses the native C++ backend.

Expected log lines include:

```text
SKEE interface versions: Overlay=2 Override=2 ActorUpdate=2
Selected SKEE backend: modern (...)
```

## Requirements

- Skyrim Special Edition / Anniversary Edition on a supported runtime
- SKSE64 matching the installed game version
- Address Library for SKSE Plugins matching the runtime
- RaceMenu / SKEE matching the runtime
- one or more supported body-hair provider mods selected in the FOMOD

## Installation

1. Install SKSE64, Address Library and RaceMenu versions matching your Skyrim runtime.
2. Install one or more supported body-hair provider mods.
3. Install `BodyHairSliders-v0.10.1-FOMOD.zip` with Vortex or another FOMOD-capable mod manager.
4. Select the provider packs actually installed in your setup.
5. Optionally install the extended RaceMenu Body/Hands/Feet overlay-slot configuration if needed.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and open RaceMenu (`showracemenu`).
8. Open the native **Hair** category.

## Diagnostics

Runtime diagnostics are written to:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

The first lines identify the exact BodyHairSliders build and detected Skyrim runtime. The log also reports provider detection, style counts, SKEE interface versions and the selected backend.

## Build

```bat
build_release.bat
```

The version is read from the root `VERSION` file. The build verifies the DLL, both Papyrus `.pex` files, all selected provider definitions and the final FOMOD archive.

Expected archive:

```text
dist/BodyHairSliders-v0.10.1-FOMOD.zip
```

`compiler_stubs/NiOverride.psc` is compile-time only and is **not** packaged as a RaceMenu replacement.

## v0.10.1

- Optimized `Body Hair Color` updates on the legacy `NiOverride` backend.
- Caches active legacy body-hair selections instead of rescanning every overlay location on each color change.
- Recolors only active BodyHairSliders regions.
- Batches legacy recoloring into a single `NiOverride.ApplyNodeOverrides()` refresh.
- Preserves the modern SKEE v2+ C++ backend unchanged.

## v0.10.0

- Added **Body Hair Overlays for Male and Female — BH extra variants** provider support.
- Added a new native RaceMenu **Full Body Hair** slider.
- Added male and female full-body overlay mappings.
- Added male and female Arm Hair and Leg Hair mappings for the BH pack.
- Added automatic FOMOD recommendation when `BH_Body.esp` is active.
- Preserved the unified SKEE v1 legacy and SKEE v2+ modern runtime architecture introduced in v0.9.0.

## Existing provider notes

### HIMBO

HIMBO exposes arms, hands, armpits, butt, back, stomach/belly, chest, legs and feet. Hands render through Hand overlay slots and feet through Feet overlay slots while controls remain under Hair.

### More Pubes for SlaveTats

BodyHairSliders reads the original provider DDS files directly. SlaveTats itself is not required, but the original texture pack must remain installed.

### Natural Pubic Hairstyles

Standard variants use:

```text
Data/Textures/Actors/Character/PubicHairStyles/
```

UBE variants use:

```text
Data/Textures/Actors/Character/UBE_PubicHairStyles/
```

The standard and UBE providers remain separate FOMOD choices.
