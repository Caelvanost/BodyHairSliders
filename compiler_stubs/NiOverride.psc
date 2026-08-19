; Compile-only stub for the RaceMenu/NiOverride Papyrus API.
; DO NOT install this file into Skyrim; RaceMenu supplies the real script/native functions.
Scriptname NiOverride Hidden

Int Function GetNumBodyOverlays() Global Native
Int Function GetNumHandOverlays() Global Native
Int Function GetNumFeetOverlays() Global Native

Bool Function HasNodeOverride(ObjectReference ref, Bool isFemale, String nodeName, Int key, Int index) Global Native
String Function GetNodeOverrideString(ObjectReference ref, Bool isFemale, String nodeName, Int key, Int index) Global Native

Function AddNodeOverrideString(ObjectReference ref, Bool isFemale, String nodeName, Int key, Int index, String value, Bool persist) Global Native
Function AddNodeOverrideInt(ObjectReference ref, Bool isFemale, String nodeName, Int key, Int index, Int value, Bool persist) Global Native
Function AddNodeOverrideFloat(ObjectReference ref, Bool isFemale, String nodeName, Int key, Int index, Float value, Bool persist) Global Native
Function RemoveNodeOverride(ObjectReference ref, Bool isFemale, String nodeName, Int key, Int index) Global Native
Function ApplyNodeOverrides(ObjectReference ref) Global Native
