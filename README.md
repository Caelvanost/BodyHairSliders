# BodyHairSliders

BodyHairSliders is a Skyrim SE/AE SKSE plugin that exposes compatible third-party body-hair overlays through a unified RaceMenu frontend.

The mod does **not** redistribute body-hair textures or meshes. Supported body-hair mods remain external asset providers.

## Current version

**v0.9.0 — unified runtime support**

One archive and one DLL now support both the validated legacy RaceMenu/SKEE v1 path and the modern SKEE v2+ path.

### Runtime compatibility

Validated / intended targets:

- **Skyrim SE 1.5.97** — legacy RaceMenu/SKEE v1 backend through RaceMenu `NiOverride` Papyrus natives.
- **Skyrim SE/AE 1.6.1170** — modern RaceMenu/SKEE v2+ C++ backend.
- **Skyrim 1.6.640** — planned for validation; the unified architecture already supports SKEE v2+ and the dedicated compatibility branch remains available for testing.

BodyHairSliders does not select behavior from a hard-coded Skyrim version table. It queries RaceMenu/SKEE at runtime and selects the safe backend from the interface ABI:

```text
SKEE Overlay v1 + Override v1
  -> legacy-papyrus backend

SKEE Overlay v2+ + Override v2+
  -> modern C++ backend
```

This prevents legacy SKEE v1 objects from being called through the incompatible modern wrapper vtable.

## RaceMenu controls

BodyHairSliders integrates directly into RaceMenu's native **Hair** category and dynamically exposes only regions for which compatible styles are detected:

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

The v0.9.0 FOMOD supports:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility
- More Pubes for SlaveTats
- Natural Pubic Hairstyles — standard 2K/4K variants
- Natural Pubic Hairstyles - UBE — UBE 2K/4K variants

Select only the providers actually installed in the current Skyrim setup.

Provider definitions are installed under:

```text
SKSE/Plugins/BodyHairSliders/providers/*.json
```

## Backend details

### Legacy SKEE v1 / Skyrim 1.5.97

The original SKEE v1 `Overlay` and `Override` interfaces are not ABI-compatible with the modern wrapper API. BodyHairSliders therefore queries only the stable base `IPluginInterface` first, reads `GetVersion()`, and never casts v1 objects to the modern wrappers.

For v1/v1, the RaceMenu frontend uses RaceMenu's `NiOverride` Papyrus natives for:

- Body/Hands/Feet overlay counts;
- current texture detection;
- overlay slot reuse;
- texture/tint/alpha application;
- `None / Shaved` clearing;
- color reapplication.

This path was field-tested successfully by Skyrim 1.5.97 users in the v0.7.2 compatibility build and is carried forward into v0.9.0.

Expected log lines include:

```text
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=0
Selected SKEE backend: legacy-papyrus (...)
```

### Modern SKEE v2+

For `Overlay >= 2` and `Override >= 2`, BodyHairSliders uses the native C++ backend:

- `Overlay` for Body/Hand/Feet overlay counts and node formats;
- `Override` for diffuse texture, tint and alpha;
- `ActorUpdateManager` when available for refresh/update work.

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

For Skyrim 1.5.97, use the matching 1.5.97-era SKSE, Address Library and RaceMenu build. For modern Skyrim, use the corresponding modern versions.

## Installation

1. Install SKSE64, Address Library and RaceMenu versions matching your Skyrim runtime.
2. Install one or more supported body-hair provider mods.
3. Install `BodyHairSliders-v0.9.0-FOMOD.zip` with Vortex or another FOMOD-capable mod manager.
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

The first line identifies the exact BodyHairSliders build. The log also reports provider detection, style counts, SKEE interface versions and the selected backend.

When reporting compatibility problems, include the full `BodyHairSliders.log`, exact Skyrim version, SKSE version and RaceMenu version.

## Build

```bat
build_release.bat
```

The version is read from the root `VERSION` file. The build cleans previous outputs, compiles the C++ DLL and Papyrus frontend, verifies both `.pex` files were generated, stages and validates the FOMOD, then produces:

```text
package/BodyHairSliders.esp
package/SKSE/Plugins/BodyHairSliders.dll
package/Scripts/BodyHairSliders.pex
package/Scripts/BodyHairSlidersRaceMenu.pex
dist/BodyHairSliders-v0.9.0-FOMOD.zip
```

`compiler_stubs/NiOverride.psc` is compile-time only and is **not** packaged as a RaceMenu replacement.

## v0.9.0

- Unified the validated Skyrim 1.5.97 legacy backend with the existing modern backend into one DLL.
- Added runtime SKEE ABI detection before any interface cast.
- Selects `legacy-papyrus` for SKEE v1/v1 and `modern` for SKEE v2+.
- Carries forward the field-tested v0.7.2 NiOverride compatibility implementation.
- Keeps the modern overlay renderer unchanged for current SKEE wrapper APIs.
- Hardened the release build so missing Papyrus outputs abort packaging.
- Restored a single universal FOMOD archive name.

## Supported provider notes

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
