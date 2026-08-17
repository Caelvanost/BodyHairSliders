# Nexus Mods publication pack — BodyHairSliders v0.5.0

## Mod name

**Body Hair Sliders - RaceMenu Body Hair Integration**

Alternative shorter title:

**Body Hair Sliders for RaceMenu**

## One-line summary

Adds unified RaceMenu sliders for compatible male and female body-hair overlay packs, directly inside the native Hair tab.

## Short description

Body Hair Sliders provides a unified RaceMenu frontend for compatible third-party body-hair overlays. Choose pubic, armpit, chest, stomach, back, arm, hand, leg, foot and butt hair from the normal Hair tab, with automatic detection of supported overlays already present on your character.

The mod does not redistribute third-party textures or meshes; supported body-hair packs remain separate requirements/providers.

## Main description

### Overview

**Body Hair Sliders** is a SKSE/CommonLibSSE-NG plugin that integrates compatible third-party body-hair overlays directly into RaceMenu's native **Hair** category.

Instead of opening Body Paint, Hand Paint and Feet Paint separately and manually finding individual overlays, supported body-hair styles are exposed as normal RaceMenu sliders.

Only regions for which compatible styles are actually detected are shown.

### Available controls

Depending on the installed providers, Body Hair Sliders can expose:

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

Every region includes **None / Shaved** as position 0.

`Body Hair Color` includes **Match Hair** plus built-in color presets.

### Existing overlays are detected

Body Hair Sliders reads the character's existing RaceMenu/SKEE overlay textures when RaceMenu opens.

If the character already uses a supported body-hair overlay, the corresponding slider opens on the matching value instead of resetting to zero.

### Supported providers

The v0.5.0 FOMOD supports:

- **Nordic Warmaiden Body Hair**
- **HIMBO V3 Bodyhair Overlays for Racemenu**
- **Pubes Forever Female / Pubic Hairstyles All In One CBBE**
- **Pubes Forever for Males**
- **OPubes NG** compatibility

The provider mods supply the actual textures/assets. **Body Hair Sliders does not redistribute them.**

Select only the providers you actually have installed during the FOMOD installation.

### HIMBO support

HIMBO body-hair overlays are mapped to dedicated controls for:

- Arms — Light / Medium / Heavy
- Hands — Light / Medium / Heavy
- Armpits
- Butt — Light / Medium / Heavy
- Back — Light / Medium / Heavy
- Stomach/Belly — Light / Medium / Heavy
- Chest — Light / Medium / Heavy
- Legs — Light / Medium / Heavy
- Feet

Hands use RaceMenu Hand overlay slots and feet use Feet overlay slots, while all controls remain together in the **Hair** tab.

### Requirements

Hard requirements:

- **SKSE64** matching your Skyrim runtime
- **Address Library for SKSE Plugins**
- **RaceMenu**

You also need at least one supported body-hair provider if you want body-hair styles to appear.

RaceMenu provides the SKEE overlay system used by Body Hair Sliders. RaceMenu itself requires SKSE64. See the RaceMenu Nexus page for the correct version for your Skyrim runtime.

### Installation

1. Install SKSE64, Address Library and RaceMenu.
2. Install one or more supported body-hair provider mods.
3. Install **Body Hair Sliders** with Vortex or another FOMOD-capable mod manager.
4. Select the provider packs you actually have installed.
5. Enable the optional extended overlay slots if your current RaceMenu configuration does not already provide enough Body/Hands/Feet overlay slots.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and open RaceMenu (`showracemenu`).
8. Open **Hair**. The body-hair controls will appear alongside the normal hair/facial-hair controls.

### Extended overlay slots

RaceMenu's overlay capacity can be shared by tattoos, body paints and other mods.

The FOMOD includes an optional `skee64_custom.ini` configuration that increases the available normal overlay slots to:

- Body: 20
- Hands: 10
- Feet: 10

Do not install this option if another mod already manages these values and your setup already has enough slots.

### Compatibility

Body Hair Sliders is designed to coexist with other RaceMenu overlay mods. It searches for free slots and attempts to reclaim already-existing supported body-hair overlays rather than overwriting unrelated paints.

It does not replace or modify the original provider textures.

### Upgrading from early development builds

If you previously installed a manually compiled development version directly into Skyrim's `Data` folder, remove loose copies of:

- `Data/BodyHairSliders.esp`
- `Data/Scripts/BodyHairSliders.pex`
- `Data/Scripts/BodyHairSlidersRaceMenu.pex`

Then redeploy/reinstall the current version through your mod manager.

### Troubleshooting

The plugin writes a diagnostic log to:

`Documents/My Games/Skyrim Special Edition/SKSE/BodyHairSliders.log`

If a slider is missing, check that:

1. the provider mod is installed;
2. the corresponding provider was selected in the Body Hair Sliders FOMOD;
3. RaceMenu/SKEE is working;
4. the relevant body-hair textures exist in the expected provider paths.

When reporting an issue, include `BodyHairSliders.log`.

### Credits

- **expired6978** — RaceMenu / SKEE and the RaceMenu Modders Package/API
- **CommonLibSSE-NG contributors** — SKSE/CommonLib plugin framework
- Authors of the supported body-hair provider mods — all body-hair textures and assets remain their work and are not redistributed by Body Hair Sliders

Please endorse and support the original body-hair provider mods whose assets you use.

### Source

Source code is available on GitHub under the `Caelvanost/BodyHairSliders` repository.

## Version 0.5.0 changelog

**Initial Nexus release candidate**

- Integrated Body Hair Sliders directly into RaceMenu's native Hair category.
- Added dynamic sliders for pubic, armpit, chest, stomach, back, arm, hand, leg, foot and butt hair.
- Added Body Hair Color with Match Hair and preset colors.
- Added support for Nordic Warmaiden Body Hair.
- Added explicit HIMBO body-hair mappings for Body, Hands and Feet overlays.
- Added Pubes Forever Female / Pubic Hairstyles All In One CBBE support.
- Added Pubes Forever for Males support.
- Added OPubes NG compatibility provider.
- Added modular FOMOD provider selection.
- Added automatic detection of supported overlays already applied to the player.
- Added saved overlay-slot reclamation to avoid needless duplicate overlays.
- Added case-insensitive provider region lookups.
- Added optional expanded RaceMenu Body/Hands/Feet overlay capacity.
- Added diagnostic logging to `BodyHairSliders.log`.

## Recommended Nexus metadata

**Game:** Skyrim Special Edition

**Category:** Body, Face, and Hair

Suggested tags (choose the closest available Nexus tags):

- RaceMenu
- Character Appearance
- Body
- Hair
- SKSE
- Utilities / Quality of Life if available

**Version:** 0.5.0

**Main file name:**

`BodyHairSliders-v0.5.0-FOMOD.zip`

**Main file label:**

`Body Hair Sliders v0.5.0 - FOMOD`

**Main file description:**

Main installer. Select only the body-hair provider packs installed in your setup. Includes an optional RaceMenu overlay-capacity configuration.

## Nexus requirements to add

Hard requirements:

- RaceMenu
- Address Library for SKSE Plugins
- SKSE64 (can be mentioned in description if Nexus does not expose it conveniently as a requirement entry)

Optional/provider requirements — mark them as optional and explain that at least one provider is required to expose styles:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever SSE / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG

## Permissions recommendation

Body Hair Sliders itself contains original code/configuration and does not redistribute the supported providers' textures or meshes.

Suggested Nexus permission stance for the first release:

- Other user's assets: **No assets from other authors are included in this file**.
- Upload to other sites: **Ask permission first**.
- Modification: **Allowed with credit**, or **ask permission first** if you want to keep tighter control during early development.
- Conversion to other games: **Ask permission first**.
- Asset use in other mods: code/config only; **ask permission first** unless/until a repository license is chosen.
- Donation Points: author's choice; no third-party provider assets are included in the download.

Do not claim ownership of any body-hair textures or meshes from the supported provider mods.

## Suggested images/screenshots

Minimum useful set:

1. **Main image** — RaceMenu Hair tab with several Body Hair Sliders visible.
2. **Full slider list** — screenshot showing Pubic/Armpit/Chest/Stomach/Back/Arm/Hand/Leg/Foot/Butt + color controls.
3. **Male HIMBO example** — visible chest/stomach/arm/leg hair selection.
4. **Female example** — Nordic Warmaiden and/or Pubes Forever style shown in RaceMenu.
5. **FOMOD provider selection** — screenshot showing the modular provider choices.
6. Optional before/after image demonstrating an existing body-hair overlay being detected at the correct slider position.

Avoid using provider authors' promotional screenshots without permission. Prefer screenshots taken in your own game.

## Publication checklist

- [ ] Build from clean `main` with `VERSION = 0.5.0`.
- [ ] Run `build_release.bat`.
- [ ] Verify `dist/BodyHairSliders-v0.5.0-FOMOD.zip` installs cleanly in Vortex.
- [ ] Confirm all selected provider JSONs are installed under `SKSE/Plugins/BodyHairSliders/providers/`.
- [ ] Confirm `BodyHairSliders.esp` is active.
- [ ] Test male HIMBO controls.
- [ ] Test at least one female provider.
- [ ] Test reopening RaceMenu preserves/detects existing slider positions.
- [ ] Test `None / Shaved` clears each representative Body/Hand/Feet location.
- [ ] Test optional extended overlay-slot FOMOD choice.
- [ ] Capture screenshots.
- [ ] Add Nexus requirements and credits to provider authors.
- [ ] Upload only the FOMOD archive as the main file.
- [ ] Publish the GitHub source link on the Nexus page.
