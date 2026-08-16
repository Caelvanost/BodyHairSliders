; Compile-only stub. DO NOT install this file into Skyrim.
Scriptname RaceMenuBase extends Quest

Int Property category_body = 4 AutoReadOnly
Int Property CATEGORY_HAIR = 256 AutoReadOnly

Function AddBodyPaint(String name, String texturePath)
EndFunction

Function AddSlider(String name, Int section, String callback, Float min, Float max, Float interval, Float position)
EndFunction

Function AddCategory(String keyName, String name, Int priority = 0)
EndFunction

Function AddSliderEx(String name, String category_key, String callback, Float min, Float max, Float interval, Float position, Int section = 0, Int priority = 0)
EndFunction
