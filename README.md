# BodyHairSliders

BodyHairSliders is a Skyrim SE/AE SKSE plugin that exposes compatible third-party body-hair overlays through a unified RaceMenu frontend.

> **Compatibility test branch:** `compat/skyrim-1.6.640-racemenu-0.4.19.14`
>
> **v0.8.0 targets Skyrim SE/AE 1.6.640 with RaceMenu 0.4.19.14.** This branch deliberately refuses to load on another Skyrim runtime so test results cannot be mixed with other compatibility targets.

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

The **v0.8.0** FOMOD supports:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility
- More Pubes for SlaveTats
- Natural Pubic Hairstyles — standard 2K/4K variants
- Natural Pubic Hairstyles - UBE — UBE 2K/4K variants

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

### More Pubes for SlaveTats

v0.6.0 added the **48 female pubic-hair textures** from More Pubes for SlaveTats as normal BodyHairSliders `Pubic Hair` choices.

The provider reads the original texture pack from:

```text
Data/textures/actors/character/slavetats/ZckeZckTPubicHair/
```

The original SlaveTats JSON names are preserved as slider style labels, including the CBBE/UNP-specific variants.

**SlaveTats itself is not required by BodyHairSliders.** BodyHairSliders uses the installed DDS files directly through RaceMenu/SKEE. The More Pubes for SlaveTats texture pack itself must remain installed.

### Natural Pubic Hairstyles

The standard Natural Pubic Hairstyles provider supports both **2K and 4K** variants. It scans the 65 female styles from:

```text
Data/Textures/Actors/Character/PubicHairStyles/
```

using the `PubicHairStyles-*.dds` files. `NaturalPubicHairstyles.esp` is used as the provider detection plugin.

### Natural Pubic Hairstyles - UBE

v0.7.0 added support for the **UBE 2K/4K** Natural Pubic Hairstyles variants. They expose the same 65 logical styles but use a separate UBE texture directory:

```text
Data/Textures/Actors/Character/UBE_PubicHairStyles/
```

The provider scans the same `PubicHairStyles-*.dds` naming scheme and uses `UBENaturalPubicHairstyles.esp` for detection.

The standard and UBE providers are separate FOMOD choices so users should select only the variant family actually installed for their body setup.

## Skyrim 1.6.640 / RaceMenu 0.4.19.14 compatibility

v0.8.0 is a dedicated compatibility test build for:

```text
Skyrim SE/AE 1.6.640
RaceMenu 0.4.19.14
SKSE64 matching Skyrim 1.6.640
Address Library matching Skyrim 1.6.640
```

RaceMenu 0.4.19.14 uses the wrapper-generation SKEE API. The official SKEE interfaces expose version 2 for `Overlay`, `Override` and `ActorUpdateManager`.

BodyHairSliders now queries every SKEE object through the ABI-stable `IPluginInterface` base first and reads `GetVersion()` before casting it to the wrapper interface. The test backend activates only when `Overlay v2` and `Override v2` are present.

Expected log lines:

```text
BodyHairSliders v0.8.0 loading - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST
Compatibility target: Skyrim 1-6-640-0 / RaceMenu 0.4.19.14
Detected Skyrim runtime: 1-6-640-0
SKEE interface versions: Overlay=2 Override=2 ActorUpdate=2
SKEE acquired: Overlay v2 Override v2 ActorUpdate v2 backend=rm-0.4.19.14-wrapper
```

If another Skyrim executable version is detected, this compatibility build returns `false` from plugin load rather than running against an unintended runtime.

## SKEE integration

The runtime uses:

- `Overlay` to discover overlay counts and node formats for Body, Hand and Feet locations;
- `Override` to read/write diffuse texture, tint color and alpha;
- `ActorUpdateManager` to rebuild overlays and node overrides when the matching v2 interface is available.

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

Runtime for this branch:

- Skyrim **1.6.640**
- SKSE64 matching Skyrim 1.6.640
- Address Library for Skyrim 1.6.640
- RaceMenu **0.4.19.14** / SKEE v2
- one or more supported body-hair provider mods selected in the FOMOD

Development/build:

- Visual Studio with C++ desktop workload
- CMake 3.21+
- vcpkg (`VCPKG_ROOT`)
- CommonLibSSE-NG
- Skyrim Creation Kit Papyrus compiler and extracted vanilla/RaceMenu script sources

## Installation

1. Confirm the game executable is Skyrim **1.6.640**.
2. Install the matching SKSE64 and Address Library.
3. Install RaceMenu **0.4.19.14**.
4. Install one or more supported body-hair provider mods.
5. Install `BodyHairSliders-v0.8.0-SE-1.6.640-RM-0.4.19.14-TEST-FOMOD.zip` with Vortex or another FOMOD-capable mod manager.
6. Select the body-hair providers actually installed in your setup.
7. Select the optional extended overlay-slot configuration if your current RaceMenu setup does not already provide enough Body/Hands/Feet overlay slots.
8. Enable `BodyHairSliders.esp`.
9. Launch Skyrim and open RaceMenu (`showracemenu`).
10. Open the native **Hair** category. BodyHairSliders controls appear alongside the normal hair and facial-hair controls.

Only regions supplied by the selected and detected providers are shown.

## Upgrading from older development builds

If an old manually compiled BodyHairSliders frontend was previously placed directly in `Data`, remove loose copies of:

```text
Data/BodyHairSliders.esp
Data/Scripts/BodyHairSliders.pex
Data/Scripts/BodyHairSlidersRaceMenu.pex
Data/SKSE/Plugins/BodyHairSliders.dll
```

then redeploy the mod manager installation. Do not keep another BodyHairSliders build enabled at the same time during compatibility testing.

Do not remove source `.psc` files from `Data/Source/Scripts`; they are not loaded by the game.

## Diagnostics

Runtime diagnostics are written to:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

For compatibility reports, include that full log plus any CrashLogger/NetScriptFramework crash log.

## Build

```bat
build_release.bat
```

The version is read from the root `VERSION` file by CMake and the packaging scripts.

The build cleans previous build/staging output, compiles the C++ plugin and Papyrus scripts, verifies both `.pex` outputs, stages the FOMOD, validates its XML/archive contents and produces:

```text
package/BodyHairSliders.esp
package/SKSE/Plugins/BodyHairSliders.dll
package/Scripts/BodyHairSliders.pex
package/Scripts/BodyHairSlidersRaceMenu.pex
dist/BodyHairSliders-v0.8.0-SE-1.6.640-RM-0.4.19.14-TEST-FOMOD.zip
```

## Current state: v0.8.0

Implemented:

- CommonLibSSE-NG SKSE plugin;
- dedicated `BodyHairSliders.esp` RaceMenu host;
- native integration into RaceMenu's **Hair** category;
- modular FOMOD provider selection;
- Nordic Warmaiden, HIMBO, Pubes Forever Female/Male and OPubes NG provider support;
- More Pubes for SlaveTats support with 48 named female styles;
- Natural Pubic Hairstyles standard 2K/4K support with 65 female styles;
- Natural Pubic Hairstyles UBE 2K/4K support with 65 female styles;
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
- verified FOMOD archive packaging;
- exact Skyrim 1.6.640 runtime guard for this test branch;
- SKEE v2 ABI validation before wrapper casts;
- dedicated RaceMenu 0.4.19.14 compatibility diagnostics.

### v0.8.0

- Added a dedicated Skyrim 1.6.640 / RaceMenu 0.4.19.14 compatibility target.
- Validates the Skyrim executable version before registering runtime functionality.
- Validates SKEE `Overlay`, `Override` and `ActorUpdateManager` versions before wrapper casts.
- Produces a separately named test FOMOD archive to prevent accidental mixing with other runtime builds.

Planned:

- field validation on a real Skyrim 1.6.640 + RaceMenu 0.4.19.14 setup;
- additional persistence/reapply hardening for unusual appearance refresh cases;
- stable synchronization API for MorphSyncTogether or other multiplayer integrations.
