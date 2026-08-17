# BodyHairSliders - Skyrim 1.5.97 Test Build

## Nexus file name

`BodyHairSliders-v0.7.2-SE-1.5.97-TEST-FOMOD.zip`

## Suggested Nexus file label

**Body Hair Sliders v0.7.2 - Skyrim 1.5.97 TEST**

## Suggested file description

Experimental compatibility build for **Skyrim SE 1.5.97**.

This build adds a dedicated legacy RaceMenu/SKEE backend to address crashes reported when opening RaceMenu on 1.5.97 installations. The crash was traced to an ABI mismatch between the modern SKEE C++ interface used by BodyHairSliders and the legacy SKEE interface exposed by 1.5.97-era RaceMenu.

This test build avoids calling the legacy C++ SKEE vtables and uses RaceMenu's legacy `NiOverride` Papyrus API instead.

v0.7.2 also fixes a Papyrus compiler name collision in the legacy frontend (`Location`) and makes the release script fail immediately if a required `.pex` was not produced.

**Do not install this test archive on Skyrim 1.6.x.** Use the normal main file for modern Skyrim versions.

### Intended test environment

- Skyrim SE **1.5.97**
- SKSE64 matching Skyrim 1.5.97 (typically SKSE 2.0.20)
- RaceMenu version intended for Skyrim 1.5.97
- Address Library version matching Skyrim 1.5.97
- At least one supported BodyHairSliders provider

### What to test

1. Launch the game and load a save.
2. Open RaceMenu with `showracemenu`.
3. Confirm RaceMenu no longer crashes while BodyHairSliders is enabled.
4. Open the **Hair** category and confirm BodyHairSliders sliders appear.
5. Test changing at least one body-hair style.
6. Test `None / Shaved`.
7. Test `Body Hair Color`.
8. Close RaceMenu, reopen it and confirm the currently applied style is detected correctly.
9. If using HIMBO, test Body, Hands and Feet sliders if possible.

### Expected log line

On a legacy SKEE setup, `BodyHairSliders.log` should contain a line similar to:

```text
SKEE acquired: Overlay v1 Override v1 ActorUpdate v0 backend=legacy-papyrus
```

### If you still crash

Please provide:

- `Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log`
- your full crash log from CrashLoggerSSE or NetScriptFramework
- your exact Skyrim version
- your SKSE version
- your RaceMenu version
- whether the crash occurs immediately when opening RaceMenu or only after changing a BodyHairSliders slider

Please do not report unrelated modlist warnings as BodyHairSliders bugs unless the crash stack contains `BodyHairSliders.dll`, `BodyHairSliders` Papyrus functions, or `skee64.dll` in the failing call path.
