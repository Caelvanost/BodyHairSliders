Scriptname AK_All_In_One_Script extends RaceMenuBase

; BodyHairSliders RaceMenu frontend.
; This script intentionally reuses the existing Pubes Forever RaceMenu quest,
; so no additional ESP is required for the first functional release.

Bool BHS_IsFemale = False

Event OnBodyPaintRequest()
    ; BodyHairSliders replaces the Pubes Forever entries with dedicated sliders.
    ; Other installed RaceMenu body-paint providers remain untouched.
EndEvent

Event OnSliderRequest(Actor player, ActorBase playerBase, Race actorRace, Bool isFemale)
    BHS_IsFemale = isFemale

    Bool anyRegion = False
    If BHS_AddRegionSlider("Pubic Hair", "pubic", "BHS_Pubic", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Armpit Hair", "armpits", "BHS_Armpits", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Chest Hair", "chest", "BHS_Chest", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Stomach Hair", "stomach", "BHS_Stomach", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Back Hair", "back", "BHS_Back", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Arm Hair", "arms", "BHS_Arms", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Hand Hair", "hands", "BHS_Hands", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Leg Hair", "legs", "BHS_Legs", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Foot Hair", "feet", "BHS_Feet", isFemale)
        anyRegion = True
    EndIf
    If BHS_AddRegionSlider("Butt Hair", "butt", "BHS_Butt", isFemale)
        anyRegion = True
    EndIf

    If anyRegion
        Int colorCount = BodyHairSliders.GetColorCount()
        If colorCount > 0
            AddSlider("Body Hair Color", category_body, "BHS_Color", 0.0, (colorCount - 1) as Float, 1.0, BodyHairSliders.GetCurrentColorIndex() as Float)
        EndIf
    EndIf
EndEvent

Bool Function BHS_AddRegionSlider(String displayName, String region, String callbackName, Bool isFemale)
    Int styleCount = BodyHairSliders.GetStyleCount(region, isFemale)
    If styleCount <= 0
        Return False
    EndIf

    Int current = BodyHairSliders.GetCurrentStyleIndex(region)
    If current < 0 || current > styleCount
        current = 0
    EndIf

    ; Slider position 0 is always None / Shaved. 1..N map to detected styles.
    AddSlider(displayName, category_body, callbackName, 0.0, styleCount as Float, 1.0, current as Float)
    Return True
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

    ; No Debug.Notification here: slider changes should stay silent on-screen.
    ; The SKSE plugin keeps detailed diagnostics in BodyHairSliders.log.
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

    ; Keep RaceMenu interaction silent; failures remain visible in the SKSE log.
    BodyHairSliders.SetColorIndex(requested)
EndFunction
