Scriptname BodyHairSlidersRaceMenu extends RaceMenuBase

; Dedicated BodyHairSliders RaceMenu frontend hosted by BodyHairSliders.esp.

String Property BHS_CategoryKey = "BHS_BODYHAIR" AutoReadOnly
Bool BHS_IsFemale = False

Event OnCategoryRequest()
    ; RaceMenu sorts built-in and injected categories by numeric priority.
    ; This value targets the slot immediately following the native Hair tab.
    AddCategory(BHS_CategoryKey, "Body Hair", -125)
EndEvent

Event OnBodyPaintRequest()
    ; BodyHairSliders renders through SKEE itself. Do not add provider paints here.
EndEvent

Event OnSliderRequest(Actor player, ActorBase playerBase, Race actorRace, Bool isFemale)
    BHS_IsFemale = isFemale

    Int priority = 10
    priority = BHS_AddRegionSlider("Pubic Hair", "pubic", "BHS_Pubic", isFemale, priority)
    priority = BHS_AddRegionSlider("Armpit Hair", "armpits", "BHS_Armpits", isFemale, priority)
    priority = BHS_AddRegionSlider("Chest Hair", "chest", "BHS_Chest", isFemale, priority)
    priority = BHS_AddRegionSlider("Stomach Hair", "stomach", "BHS_Stomach", isFemale, priority)
    priority = BHS_AddRegionSlider("Back Hair", "back", "BHS_Back", isFemale, priority)
    priority = BHS_AddRegionSlider("Arm Hair", "arms", "BHS_Arms", isFemale, priority)
    priority = BHS_AddRegionSlider("Hand Hair", "hands", "BHS_Hands", isFemale, priority)
    priority = BHS_AddRegionSlider("Leg Hair", "legs", "BHS_Legs", isFemale, priority)
    priority = BHS_AddRegionSlider("Foot Hair", "feet", "BHS_Feet", isFemale, priority)
    priority = BHS_AddRegionSlider("Butt Hair", "butt", "BHS_Butt", isFemale, priority)

    Int colorCount = BodyHairSliders.GetColorCount()
    If colorCount > 0
        AddSliderEx("Body Hair Color", BHS_CategoryKey, "BHS_Color", 0.0, (colorCount - 1) as Float, 1.0, BodyHairSliders.GetCurrentColorIndex() as Float, 0, priority)
    EndIf
EndEvent

Int Function BHS_AddRegionSlider(String displayName, String region, String callbackName, Bool isFemale, Int priority)
    Int styleCount = BodyHairSliders.GetStyleCount(region, isFemale)
    If styleCount <= 0
        Return priority
    EndIf

    Int current = BodyHairSliders.GetCurrentStyleIndex(region, isFemale)
    If current < 0 || current > styleCount
        current = 0
    EndIf

    ; Slider position 0 is always None / Shaved. 1..N map to detected styles.
    AddSliderEx(displayName, BHS_CategoryKey, callbackName, 0.0, styleCount as Float, 1.0, current as Float, 0, priority)
    Return priority + 10
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
