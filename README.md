# BodyHairSliders

Early development draft of a Skyrim SE/AE SKSE plugin that aims to expose body-hair assets from multiple compatible mods as convenient controls in **RaceMenu > Body**.

## Target UX

Planned controls include:

- Pubic Hair
- Armpit Hair
- Chest Hair
- Stomach Hair
- Back Hair
- Arm Hair
- Leg Hair
- Butt Hair
- Body Hair Color
- `Match Hair` as the default color mode

Only categories supplied by installed/configured providers need to be shown.

## Provider model

BodyHairSliders acts as a frontend and renderer while supported body-hair mods remain optional asset providers. Providers are configured in `config.json` and may expose explicit styles or directory scan rules.

The scanner discovers compatible `.dds` files at runtime, avoiding hard-coded style inventories and making supported pack updates easier to absorb.

Initial target providers:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubic Hairstyles All In One CBBE / Pubes Forever female assets
- Pubes Forever for Males
- OPubes NG compatibility

## Confirmed archive inventory

### Nordic Warmaiden Body Hair

The supplied archive contains RaceMenu/body-paint assets in:

```text
Data/Textures/Actors/Nordic Warmaiden Hair/
```

Confirmed body-hair groups include:

- `dePog - Pubes - ...` → pubic
- `dePog - Pits - ...` → armpits
- `dePog - Navel - ...` → stomach
- `dePog - Crack - ...` → butt / intergluteal hair
- `dePog - Beast - ...` → back

Most styles have paired `Dark_` and `Fair_` textures. BodyHairSliders merges those pairs into one logical slider entry and retains both source textures for later color handling.

### Pubic Hairstyles All In One CBBE / Pubes Forever female

The supplied archive contains 20 female pubic-hair overlays under:

```text
Data/textures/actors/character/ak_rm_pubic_hair_all_in_one/
```

The original RaceMenu script registers `AK_01_Trimmed01` through `AK_20_Superhairy05`. BodyHairSliders scans these files dynamically.

### Pubes Forever for Males

The supplied archive uses the same directory as the female pack. Male textures end in `M.dds`; BodyHairSliders filters these into a separate male provider so they do not appear in the female selector.

### HIMBO V3 Bodyhair Overlays for Racemenu

The supplied archive registers body paints from:

```text
Data/Textures/actors/character/character assets/overlays/
```

Confirmed body-region overlays are:

- Arms: Light / Medium / Heavy
- Armpit
- Ass: Light / Medium / Heavy
- Back: Light / Medium / Heavy
- Belly: Light / Medium / Heavy
- Chest: Light / Medium / Heavy
- Legs: Light / Medium / Heavy

The archive also contains separate **Hand Paint** textures (`HIMBO_BodyHair_HandsArm*.dds`) and a **Feet Paint** texture (`HIMBO_BodyHair_BodyFeet.dds`). Those are intentionally not mapped into the Body overlay backend yet because RaceMenu handles hand and feet paints through separate APIs.

### OPubes NG

OPubes NG primarily supplies scripts/configuration rather than the pubic-hair textures. Its included compatibility data confirms the expected Pubes Forever texture layout. BodyHairSliders therefore treats OPubes as a compatibility/integration provider rather than as an asset pack.

## Design goals

- Do **not** redistribute third-party body-hair assets without permission.
- Treat supported body-hair mods as external/optional providers.
- Use dedicated RaceMenu/NiOverride overlay keys so unrelated body paints are not overwritten.
- Discover provider styles dynamically where practical instead of hard-coding huge inventories.
- Support arbitrary body regions rather than a fixed pubic/armpit/butt list.
- Keep male and female provider styles distinct.
- Keep selection state separate from rendering so it can later be synchronized by MorphSyncTogether.
- Allow future support for non-overlay providers, such as 3D mesh-based pubic hair, without forcing them through the overlay backend.

## Current state: v0.1 draft

The current draft provides:

- CommonLibSSE-NG plugin skeleton.
- `BodyHairSliders.dll` project identity.
- DataLoaded initialization and logging to `BodyHairSliders.log`.
- Generic provider-based JSON config loader.
- Runtime directory-based asset discovery.
- Confirmed scan rules for Nordic Warmaiden, HIMBO, female CBBE pubic styles and Pubes Forever male.
- Dark/Fair pairing for Nordic Warmaiden styles.
- Male/female filtering for Pubes Forever-style assets.
- Region-based style lookup.
- Hair-color extraction from the actor's base hair color.
- A preset hair-color palette.
- Generic overlay manager API.
- RaceMenu integration boundary.
- Vortex-style `package/` directory and `build_release.bat` creating the ZIP under `dist/`.

### Not implemented yet

The actual SKEE/RaceMenu overlay calls and Body-category slider registration are still TODO. The next implementation milestone is to render a selected discovered style on the player and then expose the dynamic selector through RaceMenu.

## Requirements

Development/build:

- Visual Studio 2022 or newer with C++ desktop workload
- CMake 3.21+
- vcpkg (`VCPKG_ROOT` environment variable)
- CommonLibSSE-NG

Runtime target:

- SKSE64
- Address Library for SKSE Plugins
- RaceMenu
- One or more supported body-hair asset providers

## Build

```bat
build_release.bat
```

The script builds the DLL, copies it into:

```text
package/SKSE/Plugins/BodyHairSliders.dll
```

and creates:

```text
dist/BodyHairSliders-v0.1.0.zip
```

The runtime configuration is installed at:

```text
Data/SKSE/Plugins/BodyHairSliders/config.json
```

## Next development steps

1. Wire the SKEE/NiOverride body-overlay API.
2. Render and recolor one discovered overlay as an end-to-end proof.
3. Register dynamic controls in RaceMenu > Body.
4. Persist selected values and reapply on appearance/NiNode refresh.
5. Add optional Hand Paint / Feet Paint support for HIMBO if desired.
6. Test coexistence with existing RaceMenu Body Paints and Skyrim Together.
7. Expose a stable selection state that MorphSyncTogether can synchronize later.
