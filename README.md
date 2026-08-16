# BodyHairSliders

Skyrim SE/AE SKSE plugin that exposes body-hair assets from multiple compatible mods through a unified **RaceMenu > Body** frontend.

## RaceMenu controls

The first functional frontend dynamically exposes only regions for which compatible styles are detected:

- Pubic Hair
- Armpit Hair
- Chest Hair
- Stomach Hair
- Back Hair
- Arm Hair
- Leg Hair
- Butt Hair
- Body Hair Color

Each region uses `0 = None / Shaved`, followed by the detected styles. `Body Hair Color` provides `Match Hair` plus the built-in color presets.

## Provider model

BodyHairSliders acts as a frontend and renderer while supported body-hair mods remain external asset providers. Providers are configured in `config.json` and may expose explicit styles or directory scan rules.

Initial supported providers:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubic Hairstyles All In One CBBE / Pubes Forever female assets
- Pubes Forever for Males
- OPubes NG compatibility

## Confirmed archive inventory

### Nordic Warmaiden Body Hair

Assets are scanned from:

```text
Data/Textures/Actors/Nordic Warmaiden Hair/
```

Confirmed groups:

- `dePog - Pubes - ...` -> pubic
- `dePog - Pits - ...` -> armpits
- `dePog - Navel - ...` -> stomach
- `dePog - Crack - ...` -> butt
- `dePog - Beast - ...` -> back

Paired `Dark_` and `Fair_` textures are merged into one logical style.

### Pubic Hairstyles All In One CBBE / Pubes Forever female

Female pubic-hair overlays are scanned from:

```text
Data/textures/actors/character/ak_rm_pubic_hair_all_in_one/
```

### Pubes Forever for Males

Male textures use the same directory and end in `M.dds`. They are filtered into a separate male provider.

### HIMBO V3 Bodyhair Overlays for Racemenu

Body paints are scanned from:

```text
Data/Textures/actors/character/character assets/overlays/
```

Supported Body regions currently include arms, armpits, butt, back, stomach/belly, chest and legs. Separate Hand Paint and Feet Paint assets are intentionally left for a later dedicated integration.

### OPubes NG

OPubes NG primarily supplies scripts/configuration and compatibility data rather than the underlying 2D textures, so it is treated as an integration bridge.

## SKEE / NiOverride integration

BodyHairSliders acquires RaceMenu's SKEE interfaces through SKSE interface exchange.

The runtime uses:

- `Overlay` to discover Body overlay slots and node names;
- `Override` to set diffuse texture, tint color and alpha;
- `ActorUpdateManager` to rebuild overlays/node overrides.

A Body slot is selected from the highest available RaceMenu slot downward. Existing texture overrides are treated as occupied, so BodyHairSliders does not intentionally replace unrelated Body Paints.

The SKEE rendering backend has been validated in game with a male Pubes Forever style.

## RaceMenu frontend host

The first frontend deliberately reuses the existing `AK_All_In_One_Script` quest supplied by **Pubic Hairstyles All In One / Pubes Forever**. BodyHairSliders replaces that quest's script implementation with a dynamic frontend backed by the SKSE DLL.

Consequences for this first version:

- keep the underlying Pubes Forever / Pubic Hairstyles provider and its plugin installed;
- BodyHairSliders must win the file conflict for `Scripts/ak_all_in_one_script.pex`;
- do not install another selector override that wins that same script conflict;
- the provider textures themselves are never redistributed by BodyHairSliders.

A later version can move the frontend to a dedicated ESP/quest so the UI host no longer depends on that provider.

## Proof-of-concept

The old automatic renderer test remains available in `config.json` but should now stay disabled during normal UI testing:

```json
"proofOfConcept": {
  "enabled": false,
  "region": "pubic",
  "sex": "male",
  "styleIndex": 0
}
```

## Current state: v0.1 draft

Implemented:

- CommonLibSSE-NG SKSE plugin;
- provider-based JSON configuration and runtime asset discovery;
- Nordic Warmaiden, HIMBO and male/female Pubes Forever scan rules;
- Dark/Fair pairing and sex filtering;
- SKEE interface acquisition and Body-overlay rendering;
- free Body-overlay slot reservation;
- texture/tint/alpha overrides and refresh;
- native Papyrus API for style discovery/application;
- dynamic `RaceMenu > Body` sliders;
- `None / Shaved` for every region;
- `Match Hair` plus color presets;
- reapplication of selections while the current Skyrim process is running;
- Vortex-ready `package/` staging and `dist/` archive output.

Still planned:

- durable per-save semantic selection persistence across game restarts;
- automatic reapply hooks for additional NiNode/appearance refresh cases;
- dedicated BodyHairSliders ESP/quest instead of reusing `AK_All_In_One_Script`;
- optional HIMBO Hand/Feet support;
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
- Pubic Hairstyles All In One / Pubes Forever quest host for this first frontend version
- one or more supported body-hair asset providers

## Build

```bat
build_release.bat
```

If Skyrim is not at the default development path, set `SKYRIM_DIR` before building, for example:

```bat
set SKYRIM_DIR=D:\Games\Skyrim Special Edition
build_release.bat
```

The build compiles both C++ and Papyrus and produces:

```text
package/SKSE/Plugins/BodyHairSliders.dll
package/Scripts/BodyHairSliders.pex
package/Scripts/ak_all_in_one_script.pex
dist/BodyHairSliders-v0.1.0.zip
```

Compiler-only stubs are never copied into the package.

## First UI test

1. Keep the source body-hair provider mods installed.
2. Leave `proofOfConcept.enabled` set to `false`.
3. If the previous OPubes RaceMenu Selector is installed, first select `Shaved`, close RaceMenu and save.
4. Disable the old selector override package, but keep Pubes Forever / Pubic Hairstyles itself installed.
5. Install `dist/BodyHairSliders-v0.1.0.zip` with Vortex.
6. Make BodyHairSliders win the conflict for `Scripts/ak_all_in_one_script.pex`.
7. Launch Skyrim and open `showracemenu` / RaceMenu > Body.
8. Test region sliders, `None / Shaved`, and `Body Hair Color`.

Runtime diagnostics are written to:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```
