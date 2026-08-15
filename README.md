# NordicBodyHairSliders

Early development draft of a Skyrim SE/AE SKSE plugin that aims to expose **Nordic Warmaiden Body Hair** choices as convenient controls in **RaceMenu > Body**.

## Target UX

- Body Hair - Pubic
- Body Hair - Armpits
- Body Hair - Butt
- Body Hair - Color
- `Match Hair` as the default color mode

## Design goals

- Do **not** redistribute Nordic Warmaiden Body Hair textures.
- Treat Nordic Warmaiden Body Hair as an external dependency.
- Use dedicated RaceMenu/NiOverride overlay keys so unrelated body paints are not overwritten.
- Keep style mappings in JSON instead of hard-coding texture paths.
- Keep selection state separate from rendering so it can later be synchronized by MorphSyncTogether.

## Current state: v0.1 draft

This first draft provides:

- CommonLibSSE-NG plugin skeleton.
- DataLoaded initialization and logging.
- JSON config loader.
- Hair-color extraction from the actor's base hair color.
- A preset hair-color palette.
- Overlay manager API.
- RaceMenu integration boundary.
- Vortex-style `package/` directory and `build_release.bat` creating the ZIP under `dist/`.

### Not implemented yet

The actual SKEE/RaceMenu overlay calls and Body-category slider registration are intentionally **TODO** in this draft. We first need the real Nordic Warmaiden archive/file layout and the exact RaceMenu/SKEE interface we will target; placeholder texture paths are marked `TODO/...` in `config.json`.

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
- Nordic Warmaiden Body Hair

## Build

```bat
build_release.bat
```

The script builds the DLL, copies it into:

```text
package/SKSE/Plugins/NordicBodyHairSliders.dll
```

and creates:

```text
dist/NordicBodyHairSliders-v0.1.0.zip
```

## Next development step

1. Inspect the Nordic Warmaiden archive and replace `TODO/...` texture paths with the real paths.
2. Wire the SKEE/NiOverride body-overlay API.
3. Register the four controls in RaceMenu > Body.
4. Persist selected values and reapply on appearance/NiNode refresh.
5. Test coexistence with existing RaceMenu Body Paints and Skyrim Together.
