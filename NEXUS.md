# Nexus Mods publication pack — BodyHairSliders v0.10.4

## Mod name

**Body Hair Sliders - RaceMenu Body Hair Integration**

## One-line summary

Adds unified RaceMenu body-hair controls for compatible male and female overlay packs, with automatic legacy/modern RaceMenu backend selection.

## Short description

Body Hair Sliders integrates compatible third-party body-hair overlays directly into RaceMenu's native Hair category. v0.10.4 keeps the v0.10.3 lazy SKEE initialization fix and completes targeted modern SKEE node updates for texture, tint and alpha.

## Compatibility

- **Skyrim SE 1.5.97** — legacy SKEE v1 / NiOverride backend.
- **Skyrim 1.6.640** — RaceMenu 0.4.19.14 may expose SKEE v1 and is routed through the legacy backend.
- **Skyrim SE/AE 1.6.1170** — modern SKEE v2+ C++ backend.

Backend selection is automatic:

```text
Overlay v1 + Override v1 -> legacy-papyrus
Overlay v2+ + Override v2+ -> modern
```

Since v0.10.3, SKEE interface exchange is deferred until BodyHairSliders is actually used in RaceMenu. The plugin does not initialize SKEE during normal save loading.

## RaceMenu controls

Depending on installed providers, Body Hair Sliders can expose:

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

Each region includes **None / Shaved** as position 0. Existing supported overlays are detected when RaceMenu opens.

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

BodyHairSliders does not redistribute provider textures or meshes.

### Body Hair Overlays for Male and Female

The BH extra variants provider is mapped as:

- `BH_F.dds` / `BH_M.dds` -> **Full Body Hair**
- `BH_F_Arms.dds` / `BH_M_Arms.dds` -> **Arm Hair**
- `BH_F_Legs.dds` / `BH_M_Legs.dds` -> **Leg Hair**

## Modern SKEE stability and live-update fixes

### v0.10.3 — save-load stability

A reproducible modern SKEE crash occurred at the end of save loading while RaceMenu was processing unrelated overlay installation tasks. BodyHairSliders was not applying an overlay at the time; simply exchanging SKEE interfaces during `DataLoaded` was enough to alter the timing of that work on the affected setup.

v0.10.3 therefore changed initialization to:

```text
load BodyHairSliders providers
-> load save normally without touching SKEE
-> initialize SKEE only when RaceMenu requests BodyHairSliders controls
```

### v0.10.4 — targeted live node updates

The modern SKEE wrapper now includes RaceMenu's official targeted node-property methods. BodyHairSliders persists its normal node overrides and then updates only the affected live overlay geometry:

```text
SetNodeProperty(texture)
SetNodeProperty(tint)
SetNodeProperty(alpha)
```

It does **not** perform the previous broad modern refresh sequence:

```text
SetNodeProperties(actor)
AddNodeOverrideUpdate(actor)
Flush()
```

and does not call `AddOverlays()` on every slider movement.

This specifically targets:

- Full Body Hair selections being stored but not appearing live;
- Body Hair Color being stored for every active region while only some overlays visibly recolored;
- cumulative RaceMenu face distortion after repeated slider changes;
- interactions with unrelated SKEE overlay work.

Modern apply/clear log entries end with:

```text
refresh=targeted-node
```

## Extended Overlay Slots

The optional extended slot configuration provides:

```ini
[Overlays/Body]
iNumOverlays=20

[Overlays/Hands]
iNumOverlays=10

[Overlays/Feet]
iNumOverlays=10
```

Enable it when the normal RaceMenu Body/Hands/Feet slots are already occupied. On the current test setup, the default `6 / 3 / 3` counts had no free BodyHairSliders slots, while `20 / 10 / 10` worked and recovered existing body-hair overlays correctly.

## Requirements

- SKSE64 matching the Skyrim runtime
- Address Library matching the runtime
- RaceMenu matching the runtime
- at least one supported body-hair provider

## Installation

1. Install SKSE64, Address Library and RaceMenu for the current Skyrim runtime.
2. Install one or more supported body-hair provider mods.
3. Install `BodyHairSliders-v0.10.4-FOMOD.zip`.
4. Select only providers actually installed.
5. Enable Extended Overlay Slots if the normal RaceMenu slots are already occupied.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and load the save normally.
8. Open RaceMenu with `showracemenu` and use the Hair category.

## Diagnostics

Log path:

```text
Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log
```

Expected modern initialization after opening RaceMenu:

```text
Initializing RaceMenu/SKEE integration on demand
SKEE interface versions: Overlay=2 Override=2 ActorUpdate=2
Selected SKEE backend: modern (...)
```

Expected modern style operations:

```text
Applied provider=... refresh=targeted-node
Cleared BodyHairSliders region='...' refresh=targeted-node
```

## Version 0.10.4 changelog

- Completed the local modern SKEE v2 node-property interface using RaceMenu's official ABI order.
- Added targeted `SetNodeProperty()` updates for live overlay texture, tint and alpha.
- Removed global `SetNodeProperties()` from normal modern style and color changes.
- Added targeted live clearing for None / Shaved.
- Targets Full Body Hair live rendering and Body Hair Color updates across HIMBO/body regions.
- Keeps ActorUpdateManager node-update/flush operations disabled.

## Version 0.10.3 changelog

- Deferred SKEE initialization until BodyHairSliders is actually used in RaceMenu.
- Removed normal save-load SKEE interaction.
- Fixed the reproducible save-load CTD on the tested Skyrim 1.6.1170 setup.

## Recommended Nexus metadata

**Game:** Skyrim Special Edition

**Category:** Body, Face, and Hair

Suggested tags:

- RaceMenu
- Character Appearance
- Body
- Hair
- SKSE

**Version:** 0.10.4

**Main file name:**

`BodyHairSliders-v0.10.4-FOMOD.zip`

**Main file label:**

`Body Hair Sliders v0.10.4 - Universal FOMOD`

## Publication checklist

- [ ] Build from `dev/unified-runtime-support` with `VERSION = 0.10.4`.
- [ ] Verify the save still loads before RaceMenu is opened.
- [ ] Verify Full Body Hair appears/disappears immediately.
- [ ] Verify Body Hair Color visibly updates active HIMBO Body/Hands/Feet overlays as well as pubic hair.
- [ ] Verify None / Shaved clears only the selected region.
- [ ] Rapidly move several body-hair sliders and confirm the face does not progressively deform.
- [ ] Close and reopen RaceMenu and confirm existing choices are recovered.
- [ ] Play/load cells with NPC overlays and verify the previous SKEE save-load crash does not recur.
- [ ] Re-test the legacy SKEE v1 path before public release.
