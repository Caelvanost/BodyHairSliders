Scriptname BodyHairSlidersRaceMenu extends RaceMenuBase

; BodyHairSliders RaceMenu frontend hosted by BodyHairSliders.esp.
; Sliders are injected directly into RaceMenu's native Hair category.

Bool BHS_IsFemale = False

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

    Int current = BodyHairSliders.GetCurrentStyleIndex(region, isFemale)
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

    BodyHairSliders.ApplyStyle(region, requested, BHS_IsFemale)
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
EndFunction
