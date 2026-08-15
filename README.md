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

BodyHairSliders is intended to act as a frontend and renderer, while supported body-hair mods remain asset providers.

Initial target providers:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever
- OPubes

Providers are represented in `config.json`. A style declares its provider, region, display label, rendering type and texture path. This keeps asset mappings out of the C++ code and makes additional packs easy to add later.

The provider mods should remain optional: users should not need to install all supported packs at once.

## Design goals

- Do **not** redistribute third-party body-hair assets without permission.
- Treat supported body-hair mods as external/optional providers.
- Use dedicated RaceMenu/NiOverride overlay keys so unrelated body paints are not overwritten.
- Keep style mappings in JSON instead of hard-coding texture paths.
- Support arbitrary body regions rather than a fixed pubic/armpit/butt list.
- Keep selection state separate from rendering so it can later be synchronized by MorphSyncTogether.
- Allow future support for non-overlay providers, such as 3D mesh-based pubic hair, without forcing them through the overlay backend.

## Current state: v0.1 draft

The current draft provides:

- CommonLibSSE-NG plugin skeleton.
- `BodyHairSliders.dll` project identity.
- DataLoaded initialization and logging to `BodyHairSliders.log`.
- Generic provider-based JSON config loader.
- Region-based style lookup.
- Hair-color extraction from the actor's base hair color.
- A preset hair-color palette.
- Generic overlay manager API.
- RaceMenu integration boundary.
- Vortex-style `package/` directory and `build_release.bat` creating the ZIP under `dist/`.

### Not implemented yet

The actual SKEE/RaceMenu overlay calls and Body-category slider registration are intentionally **TODO** in this draft.

The exact texture paths and style inventories for the supported providers also still need to be mapped from their real archives. Placeholder Nordic Warmaiden paths remain in the config until that archive is inspected; HIMBO, Pubes Forever and OPubes are currently registered as empty providers.

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

1. Inventory the real Nordic Warmaiden, HIMBO, Pubes Forever and OPubes assets.
2. Fill the provider config with exact texture/mesh paths and region mappings.
3. Add provider availability detection.
4. Wire the SKEE/NiOverride body-overlay API.
5. Register dynamic controls in RaceMenu > Body.
6. Persist selected values and reapply on appearance/NiNode refresh.
7. Test coexistence with existing RaceMenu Body Paints and Skyrim Together.
8. Expose a stable selection state that MorphSyncTogether can synchronize later.
