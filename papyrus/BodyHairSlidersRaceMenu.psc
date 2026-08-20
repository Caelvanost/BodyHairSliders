Scriptname BodyHairSlidersRaceMenu extends RaceMenuBase

; Unified BodyHairSliders RaceMenu frontend.
; Modern RaceMenu/SKEE uses the C++ wrapper backend. Legacy SKEE v1 uses
; RaceMenu's stable NiOverride Papyrus natives instead of the incompatible v1 C++ ABI.

Bool BHS_IsFemale = False
Int BHS_ShaderTintColor = 7
Int BHS_ShaderAlpha = 8
Int BHS_ShaderTexture = 9
Int BHS_Unindexed = 255
String BHS_DefaultOverlayTexture = "textures\\actors\\character\\overlays\\default.dds"

; Cache legacy selections so color changes do not rescan every Body/Hands/Feet
; overlay slot for every body-hair region. These values are refreshed when
; RaceMenu builds the sliders and whenever a BodyHairSliders slider changes.
Int BHS_LegacyFullBody = 0
Int BHS_LegacyPubic = 0
Int BHS_LegacyArmpits = 0
Int BHS_LegacyChest = 0
Int BHS_LegacyStomach = 0
Int BHS_LegacyBack = 0
Int BHS_LegacyArms = 0
Int BHS_LegacyHands = 0
Int BHS_LegacyLegs = 0
Int BHS_LegacyFeet = 0
Int BHS_LegacyButt = 0

Event OnBodyPaintRequest()
EndEvent

Event OnSliderRequest(Actor player, ActorBase playerBase, Race actorRace, Bool isFemale)
    BHS_IsFemale = isFemale

    BHS_AddRegionSlider("Full Body Hair", "fullbody", "BHS_FullBody", isFemale)
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

Function BHS_SetLegacyCachedStyle(String region, Int styleIndex)
    If region == "fullbody"
        BHS_LegacyFullBody = styleIndex
    ElseIf region == "pubic"
        BHS_LegacyPubic = styleIndex
    ElseIf region == "armpits"
        BHS_LegacyArmpits = styleIndex
    ElseIf region == "chest"
        BHS_LegacyChest = styleIndex
    ElseIf region == "stomach"
        BHS_LegacyStomach = styleIndex
    ElseIf region == "back"
        BHS_LegacyBack = styleIndex
    ElseIf region == "arms"
        BHS_LegacyArms = styleIndex
    ElseIf region == "hands"
        BHS_LegacyHands = styleIndex
    ElseIf region == "legs"
        BHS_LegacyLegs = styleIndex
    ElseIf region == "feet"
        BHS_LegacyFeet = styleIndex
    ElseIf region == "butt"
        BHS_LegacyButt = styleIndex
    EndIf
EndFunction

Function BHS_AddRegionSlider(String displayName, String region, String callbackName, Bool isFemale)
    Int styleCount = BodyHairSliders.GetStyleCount(region, isFemale)
    If styleCount <= 0
        If BodyHairSliders.IsLegacySKEE()
            BHS_SetLegacyCachedStyle(region, 0)
        EndIf
        Return
    EndIf

    Int current = 0
    If BodyHairSliders.IsLegacySKEE()
        current = BHS_GetLegacyCurrentStyle(region, isFemale)
        BHS_SetLegacyCachedStyle(region, current)
    Else
        current = BodyHairSliders.GetCurrentStyleIndex(region, isFemale)
    EndIf

    If current < 0 || current > styleCount
        current = 0
        If BodyHairSliders.IsLegacySKEE()
            BHS_SetLegacyCachedStyle(region, 0)
        EndIf
    EndIf

    AddSlider(displayName, CATEGORY_HAIR, callbackName, 0.0, styleCount as Float, 1.0, current as Float)
EndFunction

Event OnSliderChanged(String callback, Float value)
    If callback == "BHS_FullBody"
        BHS_ApplyRegion("fullbody", value)
    ElseIf callback == "BHS_Pubic"
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
        BHS_SetLegacyCachedStyle(region, requested)
        BHS_ApplyRegionLegacy(region, requested, BHS_IsFemale, True)
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

    If BodyHairSliders.IsLegacySKEE()
        ; Recolor only cached active BodyHairSliders regions. Each call updates the
        ; saved NiOverride values but deliberately defers the expensive actor refresh.
        Bool changed = False
        If BHS_LegacyFullBody > 0
            BHS_ApplyRegionLegacy("fullbody", BHS_LegacyFullBody, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyPubic > 0
            BHS_ApplyRegionLegacy("pubic", BHS_LegacyPubic, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyArmpits > 0
            BHS_ApplyRegionLegacy("armpits", BHS_LegacyArmpits, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyChest > 0
            BHS_ApplyRegionLegacy("chest", BHS_LegacyChest, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyStomach > 0
            BHS_ApplyRegionLegacy("stomach", BHS_LegacyStomach, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyBack > 0
            BHS_ApplyRegionLegacy("back", BHS_LegacyBack, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyArms > 0
            BHS_ApplyRegionLegacy("arms", BHS_LegacyArms, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyHands > 0
            BHS_ApplyRegionLegacy("hands", BHS_LegacyHands, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyLegs > 0
            BHS_ApplyRegionLegacy("legs", BHS_LegacyLegs, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyFeet > 0
            BHS_ApplyRegionLegacy("feet", BHS_LegacyFeet, BHS_IsFemale, False)
            changed = True
        EndIf
        If BHS_LegacyButt > 0
            BHS_ApplyRegionLegacy("butt", BHS_LegacyButt, BHS_IsFemale, False)
            changed = True
        EndIf

        If changed
            NiOverride.ApplyNodeOverrides(Game.GetPlayer())
        EndIf
    EndIf
EndFunction

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

Function BHS_ApplyRegionLegacy(String region, Int requested, Bool isFemale, Bool refreshActor)
    If requested <= 0
        BHS_ClearLegacyRegion(region, isFemale)
        If refreshActor
            NiOverride.ApplyNodeOverrides(Game.GetPlayer())
        EndIf
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

    If refreshActor
        NiOverride.ApplyNodeOverrides(player)
    EndIf
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
                NiOverride.RemoveNodeOverride(player, isFemale, nodeName, BHS_ShaderTexture, 0)
                NiOverride.RemoveNodeOverride(player, isFemale, nodeName, BHS_ShaderTintColor, BHS_Unindexed)
                NiOverride.RemoveNodeOverride(player, isFemale, nodeName, BHS_ShaderAlpha, BHS_Unindexed)
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
