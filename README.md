# BodyHairSliders

Skyrim SE/AE SKSE plugin that exposes body-hair assets from multiple compatible mods through a unified **RaceMenu > Body** frontend.

## RaceMenu controls

The frontend dynamically exposes only regions for which compatible styles are detected:

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

HIMBO arm/leg textures are rendered as Body Paints, hand textures as Hand Paints, and foot textures as Feet Paints.

## FOMOD provider selection

Starting with **v0.3.0**, provider definitions are modular and selected during installation instead of all being embedded in the core config.

The FOMOD currently offers:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever Male
- OPubes NG compatibility

When the corresponding ESP is active, the FOMOD marks that option as recommended. Select the same providers that are actually installed in the Skyrim setup.

The selected definitions are installed under:

```text
SKSE/Plugins/BodyHairSliders/providers/*.json
```

`config.json` now contains only global BodyHairSliders settings. The runtime still accepts the old pre-v0.3 `providers` array inside `config.json` for upgrade compatibility; a modular provider file with the same provider ID replaces the legacy definition.

Provider JSON files are loaded in deterministic filename order, and matching texture files are sorted before styles are generated. This gives stable slider ordering for future persistence and synchronization.

## Provider model

BodyHairSliders is a frontend and renderer. It does **not** redistribute third-party body-hair textures or meshes. Supported mods remain external asset providers.

### Nordic Warmaiden Body Hair

Scanned from:

```text
Data/Textures/Actors/Nordic Warmaiden Hair/
```

Supported groups:

- `dePog - Pubes - ...` -> pubic
- `dePog - Pits - ...` -> armpits
- `dePog - Navel - ...` -> stomach
- `dePog - Crack - ...` -> butt
- `dePog - Beast - ...` -> back

Paired `Dark_` and `Fair_` textures are merged into one logical style.

### Pubes Forever Female / Pubic Hairstyles All In One CBBE

Female pubic-hair overlays are scanned from:

```text
Data/textures/actors/character/ak_rm_pubic_hair_all_in_one/
```

### Pubes Forever Male

Male textures use the same directory and end in `M.dds`.

### HIMBO V3 Bodyhair Overlays for Racemenu

Scanned from:

```text
Data/Textures/actors/character/character assets/overlays/
```

Supported regions include arms, hands, armpits, butt, back, stomach/belly, chest, legs and feet.

### OPubes NG

OPubes NG is treated as an integration provider. Its assets remain external.

## SKEE / NiOverride integration

BodyHairSliders acquires RaceMenu's SKEE interfaces through SKSE interface exchange.

The runtime uses:

- `Overlay` to discover overlay counts and node formats for Body, Hand and Feet locations;
- `Override` to set diffuse texture, tint color and alpha;
- `ActorUpdateManager` to rebuild overlays/node overrides.

A free slot is selected independently per RaceMenu overlay location. Existing texture overrides are treated as occupied so BodyHairSliders does not intentionally replace unrelated paints.

The SKEE rendering backend has been validated in game with a male Pubes Forever style.

## RaceMenu frontend host

The current frontend still reuses the existing `AK_All_In_One_Script` quest supplied by **Pubic Hairstyles All In One / Pubes Forever**. BodyHairSliders replaces that quest's script implementation with a dynamic frontend backed by the SKSE DLL.

For the current version:

- keep the underlying Pubes Forever / Pubic Hairstyles quest host installed;
- BodyHairSliders must win the conflict for `Scripts/ak_all_in_one_script.pex`;
- do not install another selector override that wins that same script conflict;
- provider textures themselves are never redistributed by BodyHairSliders.

A future version can move the frontend to a dedicated BodyHairSliders ESP/quest.

## Proof-of-concept

The old automatic renderer test remains available in `config.json` but should stay disabled during normal use:

```json
"proofOfConcept": {
  "enabled": false,
  "region": "pubic",
  "sex": "male",
  "styleIndex": 0
}
```

## Current state: v0.3.0

Implemented:

- CommonLibSSE-NG SKSE plugin;
- modular provider JSON configuration;
- FOMOD provider selection with ESP-based recommendations;
- Nordic Warmaiden, HIMBO and male/female Pubes Forever scanning;
- deterministic provider/style ordering;
- Dark/Fair pairing and sex filtering;
- SKEE Body, Hand and Feet overlay rendering;
- free overlay-slot reservation;
- texture/tint/alpha overrides and refresh;
- native Papyrus API for style discovery/application;
- dynamic `RaceMenu > Body` sliders;
- `None / Shaved` for every region;
- `Match Hair` plus color presets;
- no slider-change `Debug.Notification` spam;
- reapplication of selections while the current Skyrim process is running;
- verified FOMOD archive packaging.

Still planned:

- durable per-save semantic selection persistence across game restarts;
- automatic reapply hooks for additional NiNode/appearance refresh cases;
- dedicated BodyHairSliders ESP/quest instead of reusing `AK_All_In_One_Script`;
- stable MorphSyncTogether synchronization API.

## Requirements

Development/build:

- Visual Studio with C++ desktop workload
- CMake 3.21+
- vcpkg (`VCPKG_ROOT`)
- CommonLibSSE-NG
- Skyrim Creation Kit Papyrus compiler and extracted vanilla script sources

Runtime:

- SKSE64
- Address Library for SKSE Plugins
- RaceMenu / SKEE
- Pubic Hairstyles All In One / Pubes Forever quest host for the current frontend
- one or more supported body-hair asset providers selected in the FOMOD

## Build

```bat
build_release.bat
```

If Skyrim is not at the default development path, set `SKYRIM_DIR` first, for example:

```bat
set SKYRIM_DIR=D:\Games\Skyrim Special Edition
build_release.bat
```

The version is read from the root `VERSION` file by CMake and the packaging scripts.

The build compiles C++, compiles the two Papyrus scripts, stages the core and optional provider packages, validates the FOMOD XML and archive contents, and produces:

```text
package/SKSE/Plugins/BodyHairSliders.dll
package/Scripts/BodyHairSliders.pex
package/Scripts/ak_all_in_one_script.pex
dist/BodyHairSliders-v0.3.0-FOMOD.zip
```

The source `providers/*.json` files are **not** copied into the Core package. Only the providers selected in the FOMOD are installed.

## Installation / test

1. Keep the source body-hair provider mods installed.
2. Leave `proofOfConcept.enabled` set to `false`.
3. If an older OPubes RaceMenu Selector override is installed, select `Shaved`, close RaceMenu and save before disabling that selector override.
4. Install `dist/BodyHairSliders-v0.3.0-FOMOD.zip` with Vortex.
5. In the FOMOD, select the body-hair packs installed in that Skyrim setup.
6. Make BodyHairSliders win the conflict for `Scripts/ak_all_in_one_script.pex`.
7. Deploy, launch Skyrim, then open `showracemenu` and go to **Body**.
8. Confirm that only regions supplied by the selected/installed providers appear.

Runtime diagnostics are written to:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```
