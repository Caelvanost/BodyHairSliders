# BodyHairSliders

BodyHairSliders is a Skyrim SE/AE SKSE plugin that exposes compatible third-party body-hair overlays through a unified RaceMenu frontend.

The mod does **not** redistribute provider textures or meshes. Supported body-hair mods remain external dependencies.

## Current version

**v0.10.4 — lazy SKEE initialization + targeted modern overlay updates**

One archive and one DLL support both the legacy RaceMenu/SKEE v1 path and the modern SKEE v2+ path.

## Runtime compatibility

Validated / intended targets:

- **Skyrim SE 1.5.97** — legacy RaceMenu/SKEE v1 through RaceMenu `NiOverride` Papyrus natives.
- **Skyrim 1.6.640** — RaceMenu 0.4.19.14 may expose SKEE Overlay/Override v1 and is routed through the same safe legacy backend.
- **Skyrim SE/AE 1.6.1170** — modern RaceMenu/SKEE v2+ C++ backend.

Backend selection:

```text
Overlay v1 + Override v1
  -> legacy-papyrus

Overlay v2+ + Override v2+
  -> modern
```

Since v0.10.3, BodyHairSliders does **not** exchange SKEE interfaces during normal save loading. SKEE initialization is deferred until RaceMenu actually requests BodyHairSliders controls. This avoids the save-load CTD observed on modern SKEE while RaceMenu was processing unrelated overlay installation tasks.

## RaceMenu controls

Depending on installed providers, BodyHairSliders exposes the following controls in RaceMenu's native **Hair** category:

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

Each region uses `0 = None / Shaved`, followed by the detected styles. Existing supported overlays are detected when RaceMenu opens and reflected by the sliders.

`Body Hair Color` provides `Match Hair` plus built-in presets.

## Supported providers

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever Female / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG compatibility
- More Pubes for SlaveTats
- Natural Pubic Hairstyles — standard variants
- Natural Pubic Hairstyles - UBE
- Body Hair Overlays for Male and Female — BH extra variants

Provider definitions are installed under:

```text
SKSE/Plugins/BodyHairSliders/providers/*.json
```

### Body Hair Overlays for Male and Female

The BH extra variants provider is mapped as follows:

- `BH_F.dds` / `BH_M.dds` -> **Full Body Hair**
- `BH_F_Arms.dds` / `BH_M_Arms.dds` -> **Arm Hair**
- `BH_F_Legs.dds` / `BH_M_Legs.dds` -> **Leg Hair**

The original provider mod must remain installed.

## Modern SKEE v2+ backend

v0.10.4 completes the local SKEE v2 `IOverrideInterface` wrapper with RaceMenu's official targeted node-property methods:

```text
SetNodeProperty(...)
GetNodeProperty(...)
```

Normal BodyHairSliders changes now use this sequence:

```text
persist BodyHairSliders node overrides
-> update only the affected live Body/Hands/Feet overlay node
-> no global SetNodeProperties()
-> no AddNodeOverrideUpdate()
-> no ActorUpdateManager::Flush()
```

Texture, tint and alpha are applied directly to the exact overlay geometry owned by the selected BodyHairSliders region. `None / Shaved` likewise hides only the affected live overlay node after removing its persistent overrides.

Expected modern apply log entries end with:

```text
refresh=targeted-node
```

This is intended to fix modern-backend cases where:

- a newly selected Full Body Hair texture was stored but not visibly updated;
- Body Hair Color values were stored for all active regions but only some overlays visibly recolored;
- broad RaceMenu node refreshes caused cumulative face distortion or interacted badly with unrelated SKEE overlay work.

## Legacy SKEE v1 backend

Legacy SKEE v1 is never cast to the modern wrapper ABI. BodyHairSliders uses RaceMenu's stable `NiOverride` Papyrus natives for overlay detection and application.

The legacy recoloring path caches active BodyHairSliders regions and batches recoloring into a single `NiOverride.ApplyNodeOverrides()` refresh.

## Extended overlay slots

BodyHairSliders optionally provides:

```ini
[Overlays/Body]
iNumOverlays=20

[Overlays/Hands]
iNumOverlays=10

[Overlays/Feet]
iNumOverlays=10
```

This is useful when the normal RaceMenu overlay slots are already occupied by other overlays. A tested modern setup required the extended counts because the default `6 / 3 / 3` slots were already full.

## Requirements

- Skyrim Special Edition / Anniversary Edition on a supported runtime
- matching SKSE64
- Address Library matching the runtime
- RaceMenu matching the runtime
- one or more supported body-hair provider mods

## Installation

1. Install SKSE64, Address Library and RaceMenu for the current Skyrim runtime.
2. Install one or more supported body-hair providers.
3. Install `BodyHairSliders-v0.10.4-FOMOD.zip`.
4. Select only the providers actually installed.
5. Enable Extended Overlay Slots if the normal RaceMenu slots are already occupied.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and load the save normally.
8. Open RaceMenu with `showracemenu` and use the **Hair** category.

## Diagnostics

Log path:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

On modern SKEE, initialization should not occur during save loading. The first RaceMenu use should produce lines similar to:

```text
Initializing RaceMenu/SKEE integration on demand
SKEE interface versions: Overlay=2 Override=2 ActorUpdate=2
Selected SKEE backend: modern (...)
```

Modern style updates should then report:

```text
refresh=targeted-node
```

## Build

```bat
build_release.bat
```

Expected archive:

```text
dist/BodyHairSliders-v0.10.4-FOMOD.zip
```

`compiler_stubs/NiOverride.psc` is compile-time only and is not packaged as a RaceMenu replacement.

## Changelog

### v0.10.4

- Completed the modern SKEE v2 node-property wrapper using the official RaceMenu interface layout.
- Added targeted `SetNodeProperty()` live updates for overlay texture, tint and alpha.
- Removed global `SetNodeProperties()` from normal modern style/color changes.
- Added targeted live clearing for `None / Shaved`.
- Targets the Full Body Hair live-update issue and modern Body Hair Color updates that were only visibly affecting some overlays.
- Keeps ActorUpdateManager global refresh/flush operations disabled.

### v0.10.3

- Deferred RaceMenu/SKEE interface exchange until BodyHairSliders is actually used in RaceMenu.
- Removed normal post-load SKEE interaction.
- Fixed the reproducible save-load CTD triggered by early SKEE initialization on the tested Skyrim 1.6.1170 setup.

### v0.10.2

- Removed `AddNodeOverrideUpdate()` and `ActorUpdateManager::Flush()` from normal modern BodyHairSliders operations.
- Stopped calling `AddOverlays()` on every slider movement.

### v0.10.1

- Optimized legacy Body Hair Color updates by caching active regions and batching the final NiOverride refresh.

### v0.10.0

- Added Body Hair Overlays for Male and Female support.
- Added Full Body Hair.
