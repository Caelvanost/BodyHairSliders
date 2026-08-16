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
    If BHS_AddRegionSlider("Leg Hair", "legs", "BHS_Legs", isFemale)
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
        BHS_ApplyRegion("Pubic Hair", "pubic", value)
    ElseIf callback == "BHS_Armpits"
        BHS_ApplyRegion("Armpit Hair", "armpits", value)
    ElseIf callback == "BHS_Chest"
        BHS_ApplyRegion("Chest Hair", "chest", value)
    ElseIf callback == "BHS_Stomach"
        BHS_ApplyRegion("Stomach Hair", "stomach", value)
    ElseIf callback == "BHS_Back"
        BHS_ApplyRegion("Back Hair", "back", value)
    ElseIf callback == "BHS_Arms"
        BHS_ApplyRegion("Arm Hair", "arms", value)
    ElseIf callback == "BHS_Legs"
        BHS_ApplyRegion("Leg Hair", "legs", value)
    ElseIf callback == "BHS_Butt"
        BHS_ApplyRegion("Butt Hair", "butt", value)
    ElseIf callback == "BHS_Color"
        BHS_ApplyColor(value)
    EndIf
EndEvent

Function BHS_ApplyRegion(String displayName, String region, Float value)
    Int requested = value as Int
    Int count = BodyHairSliders.GetStyleCount(region, BHS_IsFemale)

    If requested < 0
        requested = 0
    ElseIf requested > count
        requested = count
    EndIf

    If BodyHairSliders.ApplyStyle(region, requested, BHS_IsFemale)
        Debug.Notification(displayName + ": " + BodyHairSliders.GetStyleName(region, requested, BHS_IsFemale))
    Else
        Debug.Notification(displayName + ": application failed")
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

    If BodyHairSliders.SetColorIndex(requested)
        Debug.Notification("Body Hair Color: " + BodyHairSliders.GetColorName(requested))
    Else
        Debug.Notification("Body Hair Color: application failed")
    EndIf
EndFunction
