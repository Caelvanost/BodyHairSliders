# Nexus Mods publication pack — BodyHairSliders v0.7.0

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

The v0.7.0 FOMOD supports:

- **Nordic Warmaiden Body Hair**
- **HIMBO V3 Bodyhair Overlays for Racemenu**
- **Pubes Forever Female / Pubic Hairstyles All In One CBBE**
- **Pubes Forever for Males**
- **OPubes NG** compatibility
- **More Pubes for SlaveTats** — 48 female pubic styles
- **Natural Pubic Hairstyles** — standard 2K/4K, 65 female pubic styles
- **Natural Pubic Hairstyles - UBE** — UBE 2K/4K, 65 female pubic styles

The provider mods supply the actual textures/assets. **Body Hair Sliders does not redistribute them.**

Select only the providers you actually have installed during the FOMOD installation.

### More Pubes for SlaveTats support

Body Hair Sliders reads the original DDS files directly from the More Pubes for SlaveTats texture directory and exposes its 48 entries through the `Pubic Hair` slider.

The style names from the original SlaveTats JSON are preserved, including the CBBE/UNP-specific variants.

**SlaveTats itself is not required by Body Hair Sliders.** The original More Pubes for SlaveTats texture pack must be installed because Body Hair Sliders does not redistribute its assets.

### Natural Pubic Hairstyles support

Body Hair Sliders supports both Natural Pubic Hairstyles families:

- **Standard 2K/4K** — scans `Data/Textures/Actors/Character/PubicHairStyles/` and detects `NaturalPubicHairstyles.esp`.
- **UBE 2K/4K** — scans `Data/Textures/Actors/Character/UBE_PubicHairStyles/` and detects `UBENaturalPubicHairstyles.esp`.

Both families expose the same 65 `PubicHairStyles-*.dds` styles through the female `Pubic Hair` slider. Choose only the FOMOD option matching the body/texture variant you have installed.

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

### Installation

1. Install SKSE64, Address Library and RaceMenu.
2. Install one or more supported body-hair provider mods.
3. Install **Body Hair Sliders** with Vortex or another FOMOD-capable mod manager.
4. Select the provider packs you actually have installed. For Natural Pubic Hairstyles, select either the standard 2K/4K option or the UBE 2K/4K option as appropriate.
5. Enable the optional extended overlay slots if your current RaceMenu configuration does not already provide enough Body/Hands/Feet overlay slots.
6. Enable `BodyHairSliders.esp`.
7. Launch Skyrim and open RaceMenu (`showracemenu`).
8. Open **Hair**. The body-hair controls will appear alongside the normal hair/facial-hair controls.

### Extended overlay slots

The FOMOD includes an optional `skee64_custom.ini` configuration that increases the available normal overlay slots to:

- Body: 20
- Hands: 10
- Feet: 10

Do not install this option if another mod already manages these values and your setup already has enough slots.

### Compatibility

Body Hair Sliders is designed to coexist with other RaceMenu overlay mods. It searches for free slots and attempts to reclaim already-existing supported body-hair overlays rather than overwriting unrelated paints.

It does not replace or modify the original provider textures.

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
- Authors of all supported body-hair provider mods — all body-hair textures and assets remain their work and are not redistributed by Body Hair Sliders

Please endorse and support the original body-hair provider mods whose assets you use.

### Source

Source code is available on GitHub under the `Caelvanost/BodyHairSliders` repository.

## Version 0.7.0 changelog

- Added support for **Natural Pubic Hairstyles - UBE**.
- Added separate detection for `UBENaturalPubicHairstyles.esp`.
- Added scanning of the UBE texture directory `Data/Textures/Actors/Character/UBE_PubicHairStyles/`.
- Supports the UBE 2K and UBE 4K variants with the same 65 logical styles.
- Kept standard Natural Pubic Hairstyles and UBE as separate FOMOD options to avoid duplicate style sets.
- Renumbered Extended Overlay Slots to follow the new provider package.

## Previous v0.6.0 highlights

- Added support for **More Pubes for SlaveTats** with 48 named female pubic-hair styles.
- Added support for standard **Natural Pubic Hairstyles 2K/4K** with 65 female pubic-hair styles.
- Added both providers to the modular FOMOD installer.

## Previous v0.5.0 highlights

- Integrated Body Hair Sliders directly into RaceMenu's native Hair category.
- Added dynamic sliders for pubic, armpit, chest, stomach, back, arm, hand, leg, foot and butt hair.
- Added Body Hair Color with Match Hair and preset colors.
- Added Nordic Warmaiden, HIMBO, Pubes Forever Female/Male and OPubes NG provider support.
- Added automatic detection of supported overlays already applied to the player.
- Added saved overlay-slot reclamation and optional expanded RaceMenu Body/Hands/Feet overlay capacity.

## Recommended Nexus metadata

**Game:** Skyrim Special Edition

**Category:** Body, Face, and Hair

Suggested tags:

- RaceMenu
- Character Appearance
- Body
- Hair
- SKSE

**Version:** 0.7.0

**Main file name:**

`BodyHairSliders-v0.7.0-FOMOD.zip`

**Main file label:**

`Body Hair Sliders v0.7.0 - FOMOD`

**Main file description:**

Main installer. Select only the body-hair provider packs installed in your setup. Includes support for Natural Pubic Hairstyles standard 2K/4K and UBE 2K/4K, plus the other supported providers and optional RaceMenu overlay-capacity configuration.

## Nexus requirements to add

Hard requirements:

- RaceMenu
- Address Library for SKSE Plugins
- SKSE64

Optional/provider requirements:

- Nordic Warmaiden Body Hair
- HIMBO V3 Bodyhair Overlays for Racemenu
- Pubes Forever SSE / Pubic Hairstyles All In One CBBE
- Pubes Forever for Males
- OPubes NG
- More Pubes for SlaveTats
- Natural Pubic Hairstyles (standard or UBE variant)

At least one provider is needed for styles to appear.

## Permissions recommendation

Body Hair Sliders itself contains original code/configuration and does not redistribute the supported providers' textures or meshes.

Suggested Nexus permission stance:

- Other user's assets: **No assets from other authors are included in this file**.
- Upload to other sites: **Ask permission first**.
- Modification: **Allowed with credit**, or **ask permission first** during early development.
- Conversion to other games: **Ask permission first**.
- Asset use in other mods: code/config only; **ask permission first** unless/until a repository license is chosen.

Do not claim ownership of any body-hair textures or meshes from the supported provider mods.

## Publication checklist

- [ ] Build from clean `main` with `VERSION = 0.7.0`.
- [ ] Run `build_release.bat`.
- [ ] Verify `dist/BodyHairSliders-v0.7.0-FOMOD.zip` installs cleanly in Vortex.
- [ ] Confirm all selected provider JSONs are installed under `SKSE/Plugins/BodyHairSliders/providers/`.
- [ ] Test Natural Pubic Hairstyles standard 2K/4K on a female character.
- [ ] Test Natural Pubic Hairstyles UBE 2K/4K on a UBE character.
- [ ] Test More Pubes for SlaveTats on a female character.
- [ ] Re-test male HIMBO controls.
- [ ] Test reopening RaceMenu preserves/detects existing slider positions.
- [ ] Test `None / Shaved` clears representative Body/Hand/Feet overlays.
- [ ] Test optional extended overlay-slot FOMOD choice.
- [ ] Add Nexus requirements and credits to provider authors.
- [ ] Upload only the FOMOD archive as the main file.
