# BodyHairSliders

BodyHairSliders is a Skyrim SE/AE SKSE plugin that exposes compatible third-party body-hair overlays through a unified RaceMenu frontend.

The mod does **not** redistribute body-hair textures or meshes. Supported body-hair mods remain external asset providers.

## RaceMenu controls

Starting with **v0.5.0**, BodyHairSliders integrates directly into RaceMenu's native **Hair** category instead of creating a separate tab.

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

Each region uses `0 = None / Shaved`, followed by the detected styles.

`Body Hair Color` provides `Match Hair` plus built-in color presets.

Existing supported overlays already present on the player are detected from RaceMenu/SKEE and reflected by the corresponding sliders when RaceMenu opens.

The UI location is independent from the SKEE render location: HIMBO arms/back/belly/chest/legs use Body overlay slots, hands use Hand overlay slots, and feet use Feet overlay slots while all controls remain together under **Hair**.

## Supported providers

The FOMOD currently supports:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility

Select only the providers that are actually installed in the current Skyrim setup.

Provider definitions are installed under:

```text
SKSE/Plugins/BodyHairSliders/providers/*.json
```

`config.json` contains global BodyHairSliders settings. Provider files are loaded deterministically and provider/style identifiers are normalized so RaceMenu lookups are case-insensitive.

## Provider details

### Nordic Warmaiden Body Hair

Scanned from:

```text
Data/Textures/Actors/Nordic Warmaiden Hair/
```

Supported groups include pubic hair, armpits, stomach/navel, butt/crack and back hair. Paired `Dark_` and `Fair_` textures are merged into one logical style.

### Pubes Forever Female / Pubic Hairstyles All In One CBBE

Female pubic-hair overlays are scanned from:

```text
Data/textures/actors/character/ak_rm_pubic_hair_all_in_one/
```

### Pubes Forever for Males

Male pubic-hair overlays use the corresponding male textures from the provider pack.

### HIMBO V3 Bodyhair Overlays for Racemenu

Expected controls:

- Arms: Light / Medium / Heavy
- Hands: Light / Medium / Heavy
- Armpit: one style
- Butt: Light / Medium / Heavy
- Back: Light / Medium / Heavy
- Stomach/Belly: Light / Medium / Heavy
- Chest: Light / Medium / Heavy
- Legs: Light / Medium / Heavy
- Feet: one style

Hands render through RaceMenu Hand overlays and feet through Feet overlays.

### OPubes NG

OPubes NG is treated as an integration provider. Its assets remain external.

## SKEE / NiOverride integration

BodyHairSliders acquires RaceMenu's SKEE interfaces through SKSE interface exchange.

The runtime uses:

- `Overlay` to discover overlay counts and node formats for Body, Hand and Feet locations;
- `Override` to read/write diffuse texture, tint color and alpha;
- `ActorUpdateManager` to rebuild overlays and node overrides.

BodyHairSliders looks for existing supported overlay textures before reserving a new slot. This allows saved RaceMenu overlays to be detected and reclaimed instead of duplicated.

The FOMOD can optionally install a `skee64_custom.ini` profile with expanded Body/Hand/Feet overlay capacity.

## RaceMenu frontend host

BodyHairSliders ships its own `BodyHairSliders.esp`. The plugin contains the start-game-enabled `BHS_RaceMenuQuest`, with:

- `BodyHairSlidersRaceMenu` attached to the quest;
- a `Player` alias forced to `PlayerRef`;
- RaceMenu's `RaceMenuLoad` attached to the Player alias.

As of v0.5.0, the frontend uses RaceMenu's native Hair category:

```text
CATEGORY_HAIR = 256
AddSlider(..., CATEGORY_HAIR, ...)
```

No third-party body-hair mod is required as a UI host.

## Requirements

Runtime:

- Skyrim Special Edition / Anniversary Edition
- SKSE64 matching the installed game version
- Address Library for SKSE Plugins
- RaceMenu / SKEE
- one or more supported body-hair provider mods selected in the FOMOD

Development/build:

- Visual Studio with C++ desktop workload
- CMake 3.21+
- vcpkg (`VCPKG_ROOT`)
- CommonLibSSE-NG
- Skyrim Creation Kit Papyrus compiler and extracted vanilla/RaceMenu script sources

## Installation

1. Install SKSE64, Address Library and RaceMenu.
2. Install one or more supported body-hair provider mods.
3. Install `BodyHairSliders-v0.5.0-FOMOD.zip` with Vortex or another FOMOD-capable mod manager.
4. Select the body-hair providers actually installed in your setup.
5. Select the optional extended overlay-slot configuration if your current RaceMenu setup does not already provide enough Body/Hands/Feet overlay slots.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and open RaceMenu (`showracemenu`).
8. Open the native **Hair** category. BodyHairSliders controls appear alongside the normal hair and facial-hair controls.

Only regions supplied by the selected and detected providers are shown.

## Upgrading from older development builds

If an old manually compiled BodyHairSliders frontend was previously placed directly in `Data`, remove loose copies of:

```text
Data/BodyHairSliders.esp
Data/Scripts/BodyHairSliders.pex
Data/Scripts/BodyHairSlidersRaceMenu.pex
```

then redeploy the mod manager installation.

Do not remove source `.psc` files from `Data/Source/Scripts`; they are not loaded by the game.

## Diagnostics

Runtime diagnostics are written to:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

The log reports detected providers, style counts per body region, RaceMenu style queries and detected existing overlays.

## Build

```bat
build_release.bat
```

The version is read from the root `VERSION` file by CMake and the packaging scripts.

The build cleans previous build/staging output, compiles the C++ plugin and Papyrus scripts, stages the FOMOD, validates its XML/archive contents and produces:

```text
package/BodyHairSliders.esp
package/SKSE/Plugins/BodyHairSliders.dll
package/Scripts/BodyHairSliders.pex
package/Scripts/BodyHairSlidersRaceMenu.pex
dist/BodyHairSliders-v0.5.0-FOMOD.zip
```

## Current state: v0.5.0

Implemented:

- CommonLibSSE-NG SKSE plugin;
- dedicated `BodyHairSliders.esp` RaceMenu host;
- native integration into RaceMenu's **Hair** category;
- modular FOMOD provider selection;
- Nordic Warmaiden, HIMBO, Pubes Forever Female/Male and OPubes NG provider support;
- deterministic provider/style ordering;
- case-insensitive region/sex/location lookup;
- SKEE Body, Hand and Feet overlay rendering;
- free overlay-slot reservation and saved-slot reclamation;
- detection of already-applied supported overlays;
- texture/tint/alpha overrides and refresh;
- native Papyrus API for style discovery/application;
- `None / Shaved` for every region;
- `Match Hair` plus color presets;
- optional expanded RaceMenu overlay capacity;
- verified FOMOD archive packaging.

Planned:

- additional persistence/reapply hardening for unusual appearance refresh cases;
- stable synchronization API for MorphSyncTogether or other multiplayer integrations.
