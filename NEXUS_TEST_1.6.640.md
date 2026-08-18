# BodyHairSliders - Skyrim 1.6.640 / RaceMenu 0.4.19.14 Test Build

## Nexus file name

`BodyHairSliders-v0.8.0-SE-1.6.640-RM-0.4.19.14-TEST-FOMOD.zip`

## Suggested Nexus file label

**Body Hair Sliders v0.8.0 - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST**

## Suggested short description

Experimental compatibility build for Skyrim 1.6.640 + RaceMenu 0.4.19.14. Validates the SKEE v2 wrapper ABI before use and refuses other Skyrim runtimes. Testing only; please report BodyHairSliders.log and crashlogs.

## Suggested file description

Experimental compatibility build for **Skyrim SE/AE 1.6.640** with **RaceMenu 0.4.19.14**.

This build uses RaceMenu's SKEE wrapper-generation interfaces and validates their ABI versions before BodyHairSliders performs any overlay calls. It expects `Overlay v2` and `Override v2` and will refuse to activate the RaceMenu backend if an incompatible SKEE generation is detected.

The DLL also verifies the Skyrim executable version and refuses to load on runtimes other than **1.6.640**. This is intentional so compatibility reports cannot be mixed between different Skyrim/RaceMenu generations.

**Do not use this archive on Skyrim 1.5.97 or other Skyrim 1.6.x runtimes.** Use the matching BodyHairSliders build instead.

### Intended test environment

- Skyrim SE/AE **1.6.640**
- SKSE64 matching Skyrim 1.6.640
- Address Library matching Skyrim 1.6.640
- RaceMenu **0.4.19.14**
- At least one supported BodyHairSliders provider

### What to test

1. Launch the game and load a save.
2. Check `BodyHairSliders.log` before opening RaceMenu.
3. Open RaceMenu with `showracemenu`.
4. Confirm RaceMenu opens without CTD or freeze.
5. Open the **Hair** category and confirm BodyHairSliders sliders appear.
6. Test changing at least one body-hair style.
7. Test `None / Shaved`.
8. Test `Body Hair Color`.
9. Close RaceMenu, reopen it and confirm the currently applied style is detected correctly.
10. If using HIMBO, test Body, Hands and Feet regions if possible.

### Expected log lines

```text
BodyHairSliders v0.8.0 loading - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST
Compatibility target: Skyrim 1-6-640-0 / RaceMenu 0.4.19.14
Detected Skyrim runtime: 1-6-640-0
SKEE interface versions: Overlay=2 Override=2 ActorUpdate=2
SKEE acquired: Overlay v2 Override v2 ActorUpdate v2 backend=rm-0.4.19.14-wrapper
```

### If you crash or freeze

Please provide:

- `Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log`
- the full CrashLoggerSSE / NetScriptFramework crash log if one is generated
- exact Skyrim version
- exact SKSE version
- exact RaceMenu version
- whether the problem occurs when loading the game, opening RaceMenu, or changing a BodyHairSliders slider
- which BodyHairSliders providers were selected in the FOMOD

Please make sure no other BodyHairSliders release/test build is enabled at the same time. The first log line must say `BodyHairSliders v0.8.0 loading` for this test archive.
