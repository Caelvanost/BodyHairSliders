# BodyHairSliders - Skyrim 1.6.640 / RaceMenu 0.4.19.14 Test Build

## Nexus file name

`BodyHairSliders-v0.8.1-SE-1.6.640-RM-0.4.19.14-TEST-FOMOD.zip`

## Suggested Nexus file label

**Body Hair Sliders v0.8.1 - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST**

## Suggested short description

Compatibility test build for Skyrim 1.6.640 + RaceMenu 0.4.19.14. v0.8.1 adds a safe NiOverride Papyrus fallback for installations where RaceMenu exposes SKEE Overlay/Override v1 instead of the modern wrapper ABI.

## What changed from v0.8.0

A field report from Skyrim 1.6.640 + RaceMenu 0.4.19.14 showed:

```text
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=1
```

v0.8.0 incorrectly required Overlay v2 / Override v2 and therefore disabled overlay application. v0.8.1 now selects the backend from the actual SKEE ABI:

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
5. Change several body-hair styles and confirm overlays appear immediately.
6. Test `None / Shaved`.
7. Test `Body Hair Color`.
8. Close and reopen RaceMenu and verify current style detection.

### Expected log for the reported 1.6.640 configuration

```text
BodyHairSliders v0.8.1 loading - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST
Compatibility target: Skyrim 1-6-640-0 / RaceMenu 0.4.19.14
Detected Skyrim runtime: 1-6-640-0
SKEE interface versions: Overlay=1 Override=1 ActorUpdate=1
Selected SKEE backend: legacy-papyrus (Overlay v1 Override v1 ActorUpdate v1)
```

If RaceMenu exposes wrapper interfaces instead, `Selected SKEE backend: modern (...)` is also valid.

### If it fails

Please provide the full `BodyHairSliders.log`, crashlog if any, exact Skyrim/SKSE/RaceMenu versions, selected providers, and whether the failure occurs when opening RaceMenu or changing a slider. Make sure no other BodyHairSliders build is enabled. The first log line must identify **v0.8.1**.
