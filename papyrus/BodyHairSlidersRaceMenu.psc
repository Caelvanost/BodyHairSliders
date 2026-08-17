Scriptname BodyHairSlidersRaceMenu extends RaceMenuBase

; BodyHairSliders RaceMenu frontend hosted by BodyHairSliders.esp.
; Sliders are injected directly into RaceMenu's native Hair category.
;
; Modern RaceMenu uses BodyHairSliders' C++ SKEE wrapper backend. Legacy
; RaceMenu/SKEE (notably the 1.5.97-era 0.4.16 build) has an incompatible C++
; interface ABI, so this script uses its stable NiOverride Papyrus natives.

Bool BHS_IsFemale = False
Int BHS_ShaderTintColor = 7
Int BHS_ShaderAlpha = 8
Int BHS_ShaderTexture = 9
Int BHS_Unindexed = 255
String BHS_DefaultOverlayTexture = "textures\\actors\\character\\overlays\\default.dds"

Event OnBodyPaintRequest()
    ; BodyHairSliders renders through SKEE itself. Do not add provider paints here.
EndEvent

Event OnSliderRequest(Actor player, ActorBase playerBase, Race actorRace, Bool isFemale)
    BHS_IsFemale = isFemale

    BHS_AddRegionSlider("Pubic Hair", "pubic", "BHS_Pubic", isFemale)
    BHS_AddRegionSlider("Armpit Hair", "armpits", "BHS_Armpits", isFemale)
    BHS_AddRegionSlider("Chest Hair", "chest", "BHS_Chest", isFemale)
    BHS_AddRegionSlider("Stomach Hair", "stomach", "BHS_Stomach", isFemale)
    BHS_AddRegionSlider("Back Hair", "back", "BHS_Back", isFemale)
    BHS_AddRegionSlider("Arm Hair", "arms", "BHS_Arms", isFemale)
    BHS_AddRegionSlider("Hand Hair", "hands", "BHS_Hands", isFemale)
    BHS_AddRegionSlider("Leg Hair", "legs", "BHS_Legs", isFemale)
    BHS_AddRegionSlider("Foot Hair", "feet", "BHS_Feet", isFemale)
    BHS_AddRegionSlider("Butt Hair", "butt", "BHS_Butt", isFemale)

    Int colorCount = BodyHairSliders.GetColorCount()
    If colorCount > 0
        AddSlider("Body Hair Color", CATEGORY_HAIR, "BHS_Color", 0.0, (colorCount - 1) as Float, 1.0, BodyHairSliders.GetCurrentColorIndex() as Float)
    EndIf
EndEvent

Function BHS_AddRegionSlider(String displayName, String region, String callbackName, Bool isFemale)
    Int styleCount = BodyHairSliders.GetStyleCount(region, isFemale)
    If styleCount <= 0
        Return
    EndIf

    Int current = 0
    If BodyHairSliders.IsLegacySKEE()
        current = BHS_GetLegacyCurrentStyle(region, isFemale)
    Else
        current = BodyHairSliders.GetCurrentStyleIndex(region, isFemale)
    EndIf

    If current < 0 || current > styleCount
        current = 0
    EndIf

    ; Slider position 0 is always None / Shaved. 1..N map to detected styles.
    AddSlider(displayName, CATEGORY_HAIR, callbackName, 0.0, styleCount as Float, 1.0, current as Float)
EndFunction

Event OnSliderChanged(String callback, Float value)
    If callback == "BHS_Pubic"
        BHS_ApplyRegion("pubic", value)
    ElseIf callback == "BHS_Armpits"
        BHS_ApplyRegion("armpits", value)
    ElseIf callback == "BHS_Chest"
        BHS_ApplyRegion("chest", value)
    ElseIf callback == "BHS_Stomach"
        BHS_ApplyRegion("stomach", value)
    ElseIf callback == "BHS_Back"
        BHS_ApplyRegion("back", value)
    ElseIf callback == "BHS_Arms"
        BHS_ApplyRegion("arms", value)
    ElseIf callback == "BHS_Hands"
        BHS_ApplyRegion("hands", value)
    ElseIf callback == "BHS_Legs"
        BHS_ApplyRegion("legs", value)
    ElseIf callback == "BHS_Feet"
        BHS_ApplyRegion("feet", value)
    ElseIf callback == "BHS_Butt"
        BHS_ApplyRegion("butt", value)
    ElseIf callback == "BHS_Color"
        BHS_ApplyColor(value)
    EndIf
EndEvent

Function BHS_ApplyRegion(String region, Float value)
    Int requested = value as Int
    Int count = BodyHairSliders.GetStyleCount(region, BHS_IsFemale)

    If requested < 0
        requested = 0
    ElseIf requested > count
        requested = count
    EndIf

    If BodyHairSliders.IsLegacySKEE()
        BHS_ApplyRegionLegacy(region, requested, BHS_IsFemale)
    Else
        BodyHairSliders.ApplyStyle(region, requested, BHS_IsFemale)
    EndIf
EndFunction

Function BHS_ApplyColor(Float value)
    Int requested = value as Int
    Int count = BodyHairSliders.GetColorCount()

    If requested < 0
        requested = 0
    ElseIf requested >= count
        requested = count - 1
    EndIf

    BodyHairSliders.SetColorIndex(requested)

    ; The modern C++ backend reapplies selected styles itself. Legacy SKEE cannot
    ; be called through that ABI, so recolor currently active styles via NiOverride.
    If BodyHairSliders.IsLegacySKEE()
        BHS_ReapplyLegacyRegion("pubic")
        BHS_ReapplyLegacyRegion("armpits")
        BHS_ReapplyLegacyRegion("chest")
        BHS_ReapplyLegacyRegion("stomach")
        BHS_ReapplyLegacyRegion("back")
        BHS_ReapplyLegacyRegion("arms")
        BHS_ReapplyLegacyRegion("hands")
        BHS_ReapplyLegacyRegion("legs")
        BHS_ReapplyLegacyRegion("feet")
        BHS_ReapplyLegacyRegion("butt")
    EndIf
EndFunction

; -----------------------------------------------------------------------------
; Legacy RaceMenu / SKEE fallback (RaceMenu 0.4.16 / Skyrim 1.5.97 generation)
; -----------------------------------------------------------------------------

Int Function BHS_LegacyOverlayCount(String slotLocation)
    If slotLocation == "hand" || slotLocation == "hands"
        Return NiOverride.GetNumHandOverlays()
    ElseIf slotLocation == "feet" || slotLocation == "foot"
        Return NiOverride.GetNumFeetOverlays()
    EndIf
    Return NiOverride.GetNumBodyOverlays()
EndFunction

String Function BHS_LegacyNodeName(String slotLocation, Int index)
    If slotLocation == "hand" || slotLocation == "hands"
        Return "Hands [Ovl" + index + "]"
    ElseIf slotLocation == "feet" || slotLocation == "foot"
        Return "Feet [Ovl" + index + "]"
    EndIf
    Return "Body [Ovl" + index + "]"
EndFunction

Int Function BHS_FindLegacyStyleInLocation(String region, Bool isFemale, String slotLocation)
    Actor player = Game.GetPlayer()
    Int count = BHS_LegacyOverlayCount(slotLocation)
    Int i = 0

    While i < count
        String nodeName = BHS_LegacyNodeName(slotLocation, i)
        If NiOverride.HasNodeOverride(player, isFemale, nodeName, BHS_ShaderTexture, 0)
            String texture = NiOverride.GetNodeOverrideString(player, isFemale, nodeName, BHS_ShaderTexture, 0)
            Int styleIndex = BodyHairSliders.FindStyleIndexByTexture(region, texture, isFemale)
            If styleIndex > 0
                Return styleIndex
            EndIf
        EndIf
        i += 1
    EndWhile

    Return 0
EndFunction

Int Function BHS_GetLegacyCurrentStyle(String region, Bool isFemale)
    Int styleIndex = BHS_FindLegacyStyleInLocation(region, isFemale, "body")
    If styleIndex > 0
        Return styleIndex
    EndIf

    styleIndex = BHS_FindLegacyStyleInLocation(region, isFemale, "hand")
    If styleIndex > 0
        Return styleIndex
    EndIf

    Return BHS_FindLegacyStyleInLocation(region, isFemale, "feet")
EndFunction

Int Function BHS_FindLegacySlot(String region, String slotLocation, Bool isFemale)
    Actor player = Game.GetPlayer()
    Int count = BHS_LegacyOverlayCount(slotLocation)
    Int freeSlot = -1
    Int i = count - 1

    While i >= 0
        String nodeName = BHS_LegacyNodeName(slotLocation, i)
        If NiOverride.HasNodeOverride(player, isFemale, nodeName, BHS_ShaderTexture, 0)
            String texture = NiOverride.GetNodeOverrideString(player, isFemale, nodeName, BHS_ShaderTexture, 0)
            If BodyHairSliders.FindStyleIndexByTexture(region, texture, isFemale) > 0
                Return i
            EndIf
        ElseIf freeSlot < 0
            freeSlot = i
        EndIf
        i -= 1
    EndWhile

    Return freeSlot
EndFunction

Function BHS_ApplyRegionLegacy(String region, Int requested, Bool isFemale)
    If requested <= 0
        BHS_ClearLegacyRegion(region, isFemale)
        Return
    EndIf

    Actor player = Game.GetPlayer()
    String texture = BodyHairSliders.GetStyleTexture(region, requested, isFemale)
    String slotLocation = BodyHairSliders.GetStyleLocation(region, requested, isFemale)

    If texture == "" || slotLocation == ""
        Return
    EndIf

    Int slot = BHS_FindLegacySlot(region, slotLocation, isFemale)
    If slot < 0
        Return
    EndIf

    String nodeName = BHS_LegacyNodeName(slotLocation, slot)
    NiOverride.AddNodeOverrideString(player, isFemale, nodeName, BHS_ShaderTexture, 0, texture, True)
    NiOverride.AddNodeOverrideInt(player, isFemale, nodeName, BHS_ShaderTintColor, BHS_Unindexed, BodyHairSliders.GetCurrentColorRGB(), True)
    NiOverride.AddNodeOverrideFloat(player, isFemale, nodeName, BHS_ShaderAlpha, BHS_Unindexed, 1.0, True)
    NiOverride.ApplyNodeOverrides(player)
EndFunction

Function BHS_ClearLegacyLocation(String region, Bool isFemale, String slotLocation)
    Actor player = Game.GetPlayer()
    Int count = BHS_LegacyOverlayCount(slotLocation)
    Int i = 0

    While i < count
        String nodeName = BHS_LegacyNodeName(slotLocation, i)
        If NiOverride.HasNodeOverride(player, isFemale, nodeName, BHS_ShaderTexture, 0)
            String texture = NiOverride.GetNodeOverrideString(player, isFemale, nodeName, BHS_ShaderTexture, 0)
            If BodyHairSliders.FindStyleIndexByTexture(region, texture, isFemale) > 0
                ; Remove saved BodyHairSliders/provider overrides first.
                NiOverride.RemoveNodeOverride(player, isFemale, nodeName, BHS_ShaderTexture, 0)
                NiOverride.RemoveNodeOverride(player, isFemale, nodeName, BHS_ShaderTintColor, BHS_Unindexed)
                NiOverride.RemoveNodeOverride(player, isFemale, nodeName, BHS_ShaderAlpha, BHS_Unindexed)

                ; Legacy RemoveNodeOverride removes persistence but does not reset the
                ; currently rendered shader. Apply a non-persistent empty texture and
                ; zero alpha so the change is immediately visible without leaving a
                ; saved override behind.
                NiOverride.AddNodeOverrideString(player, isFemale, nodeName, BHS_ShaderTexture, 0, BHS_DefaultOverlayTexture, False)
                NiOverride.AddNodeOverrideFloat(player, isFemale, nodeName, BHS_ShaderAlpha, BHS_Unindexed, 0.0, False)
            EndIf
        EndIf
        i += 1
    EndWhile
EndFunction

Function BHS_ClearLegacyRegion(String region, Bool isFemale)
    BHS_ClearLegacyLocation(region, isFemale, "body")
    BHS_ClearLegacyLocation(region, isFemale, "hand")
    BHS_ClearLegacyLocation(region, isFemale, "feet")
EndFunction

Function BHS_ReapplyLegacyRegion(String region)
    Int current = BHS_GetLegacyCurrentStyle(region, BHS_IsFemale)
    If current > 0
        BHS_ApplyRegionLegacy(region, current, BHS_IsFemale)
    EndIf
EndFunction
