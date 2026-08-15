# BodyHairSliders

Early development draft of a Skyrim SE/AE SKSE plugin that exposes body-hair assets from multiple compatible mods through a unified RaceMenu-oriented frontend.

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

Assets are scanned from:

```text
Data/Textures/Actors/Nordic Warmaiden Hair/
```

Confirmed groups:

- `dePog - Pubes - ...` → pubic
- `dePog - Pits - ...` → armpits
- `dePog - Navel - ...` → stomach
- `dePog - Crack - ...` → butt
- `dePog - Beast - ...` → back

Paired `Dark_` and `Fair_` textures are merged into one logical style. The renderer chooses the appropriate source texture and applies the requested tint.

### Pubic Hairstyles All In One CBBE / Pubes Forever female

The supplied archive contains 20 female pubic-hair overlays under:

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

Confirmed Body regions:

- Arms: Light / Medium / Heavy
- Armpit
- Ass: Light / Medium / Heavy
- Back: Light / Medium / Heavy
- Belly: Light / Medium / Heavy
- Chest: Light / Medium / Heavy
- Legs: Light / Medium / Heavy

Separate Hand Paint and Feet Paint assets exist but are not yet routed through those RaceMenu locations.

### OPubes NG

OPubes NG primarily supplies scripts/configuration and compatibility data rather than the underlying 2D textures, so it is treated as an integration bridge.

## SKEE / NiOverride integration

BodyHairSliders now acquires RaceMenu's public SKEE interfaces through the SKSE interface-exchange message instead of linking against RaceMenu internals.

The runtime uses:

- `Overlay` to discover the configured Body overlay count and node format.
- `Override` to set the overlay diffuse texture, tint color and alpha.
- `ActorUpdateManager` to rebuild overlays and node overrides.

A Body slot is selected from the highest available RaceMenu slot downward. A slot with an existing texture override is treated as occupied so BodyHairSliders does not intentionally overwrite an existing Body Paint.

SKEE shader-property indices used by the renderer are:

- texture key `9`, texture index `0`
- tint key `7`, unindexed sentinel `255`
- alpha key `8`, unindexed sentinel `255`

## Proof-of-concept test

The package contains an opt-in end-to-end test in:

```text
Data/SKSE/Plugins/BodyHairSliders/config.json
```

It is disabled by default:

```json
"proofOfConcept": {
  "enabled": false,
  "region": "pubic",
  "sex": "female",
  "styleIndex": 0
}
```

For a test build, set `enabled` to `true`. After loading a save, BodyHairSliders will:

1. find the installed styles matching `region` and `sex`;
2. select `styleIndex`;
3. read the player's hair color;
4. reserve a free RaceMenu Body overlay slot;
5. apply the texture, hair-color tint and alpha;
6. request a SKEE overlay/node refresh.

The log is written to:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

Useful success entries include `SKEE acquired`, `Reserved RaceMenu body overlay slot`, and `Applied provider=...`.

If the log reports no free Body overlay slot, increase RaceMenu's `[Overlays/Body] iNumOverlays` in `skee64.ini` or `skee64_custom.ini`.

## Design goals

- Do **not** redistribute third-party body-hair assets without permission.
- Treat supported packs as external/optional providers.
- Avoid intentionally overwriting unrelated RaceMenu Body Paints.
- Discover provider styles dynamically where practical.
- Support arbitrary body regions rather than a fixed pubic/armpit/butt list.
- Keep male and female styles distinct.
- Keep selection state separate from rendering so MorphSyncTogether can synchronize it later.
- Leave room for non-overlay providers such as 3D pubic-hair meshes.

## Current state: v0.1 draft

Implemented:

- CommonLibSSE-NG plugin skeleton.
- Generic provider-based JSON config and runtime asset discovery.
- Confirmed provider scan rules for Nordic Warmaiden, HIMBO, female CBBE pubic styles and Pubes Forever male.
- Dark/Fair pairing and male/female filtering.
- Hair-color extraction and color presets.
- Dynamic SKEE interface acquisition.
- Free Body-overlay slot reservation.
- Texture/tint/alpha node overrides.
- SKEE overlay refresh.
- Disabled-by-default end-to-end player PoC.
- Vortex-style `package/` and `dist/` packaging.

Still to implement after the PoC is validated in game:

- actual custom selectors/sliders in `RaceMenu > Body`;
- persistent BodyHairSliders selection state;
- automatic reapplication after relevant appearance/NiNode refreshes;
- optional HIMBO Hand/Feet support;
- MorphSyncTogether synchronization API.

## Requirements

Development/build:

- Visual Studio 2022 or newer with C++ desktop workload
- CMake 3.21+
- vcpkg (`VCPKG_ROOT` environment variable)
- CommonLibSSE-NG

Runtime:

- SKSE64
- Address Library for SKSE Plugins
- RaceMenu / SKEE
- one or more supported body-hair providers

## Build

```bat
build_release.bat
```

Output DLL:

```text
package/SKSE/Plugins/BodyHairSliders.dll
```

Vortex archive:

```text
dist/BodyHairSliders-v0.1.0.zip
```

## Next development steps

1. Compile against the user's current CommonLibSSE-NG/vcpkg environment.
2. Run the disabled-by-default PoC with one known provider and inspect `BodyHairSliders.log`.
3. Correct any ABI/runtime differences exposed by the installed RaceMenu build.
4. Register dynamic controls in `RaceMenu > Body`.
5. Persist selections and reapply after appearance/NiNode refreshes.
6. Test coexistence with existing Body Paints, OStim and Skyrim Together.
7. Expose stable state for MorphSyncTogether synchronization.
