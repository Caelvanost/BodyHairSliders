# BodyHairSliders - Skyrim 1.6.640 / RaceMenu 0.4.19.14 Test Build

## Nexus file name

`BodyHairSliders-v0.8.2-SE-1.6.640-RM-0.4.19.14-TEST-FOMOD.zip`

## Suggested Nexus file label

**Body Hair Sliders v0.8.2 - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST**

## Suggested short description

Compatibility test build for Skyrim 1.6.640 + RaceMenu 0.4.19.14. v0.8.2 keeps the v0.8.1 SKEE v1 NiOverride fallback and optimizes Body Hair Color changes to avoid multi-second overlay refresh stalls.

## What changed from v0.8.1

A tester confirmed all 65 Natural Pubic Hairstyles apply correctly with the `legacy-papyrus` backend, but reported several seconds of lag when changing Body Hair Color.

The previous legacy color path rescanned Body/Hands/Feet overlay slots for every possible BodyHairSliders region and refreshed NiOverride repeatedly.

v0.8.2 now:

- caches the active style index for each BodyHairSliders region while RaceMenu is open;
- recolors only active regions;
- avoids full overlay rescans on each color change;
- batches legacy tint/texture writes;
- performs one `NiOverride.ApplyNodeOverrides()` refresh after the complete color batch.

USMP or other mods that add many hair colors do not change BodyHairSliders' own color preset count; this fix specifically targets the expensive legacy overlay refresh path.

## Compatibility backend

A real Skyrim 1.6.640 + RaceMenu 0.4.19.14 installation reported:

```text
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=1
```

BodyHairSliders selects its backend from the actual SKEE ABI:

```text
Overlay v1 + Override v1 -> legacy-papyrus (NiOverride)
Overlay v2+ + Override v2+ -> modern C++ wrapper backend
```

The DLL remains strictly locked to Skyrim **1.6.640** for this compatibility test branch.

### Intended test environment

- Skyrim SE/AE **1.6.640**
- SKSE64 matching Skyrim 1.6.640
- Address Library matching Skyrim 1.6.640
- RaceMenu **0.4.19.14**
- At least one supported BodyHairSliders provider

### What to test

1. Launch and load a save.
2. Check `BodyHairSliders.log`.
3. Open RaceMenu with `showracemenu`.
4. Confirm the Hair sliders appear.
5. Change several body-hair styles and confirm overlays appear.
6. Test `None / Shaved`.
7. With at least one body-hair overlay active, move `Body Hair Color` through several values and compare responsiveness with v0.8.1.
8. If several regions are available, activate multiple regions and repeat the color test.
9. Close and reopen RaceMenu and verify current style detection.

### Expected log for the reported 1.6.640 configuration

```text
BodyHairSliders v0.8.2 loading - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST
Compatibility target: Skyrim 1-6-640-0 / RaceMenu 0.4.19.14
Detected Skyrim runtime: 1-6-640-0
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=1
Selected SKEE backend: legacy-papyrus (Overlay v1 Override v1 ActorUpdate v1)
```

If RaceMenu exposes wrapper interfaces instead, `Selected SKEE backend: modern (...)` is also valid.

### If it fails

Please provide the full `BodyHairSliders.log`, crashlog if any, exact Skyrim/SKSE/RaceMenu versions, selected providers, and whether the issue occurs when changing a style or changing Body Hair Color. Make sure no other BodyHairSliders build is enabled. The first log line must identify **v0.8.2**.
