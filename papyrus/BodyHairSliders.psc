Scriptname BodyHairSliders Hidden

Int Function GetStyleCount(String region, Bool female) Global Native
String Function GetStyleName(String region, Int index, Bool female) Global Native
Int Function GetCurrentStyleIndex(String region, Bool female) Global Native
Bool Function ApplyStyle(String region, Int index, Bool female) Global Native

Bool Function IsLegacySKEE() Global Native
String Function GetStyleTexture(String region, Int index, Bool female) Global Native
String Function GetStyleLocation(String region, Int index, Bool female) Global Native
Int Function FindStyleIndexByTexture(String region, String texture, Bool female) Global Native
Int Function GetCurrentColorRGB() Global Native

Int Function GetColorCount() Global Native
String Function GetColorName(Int index) Global Native
Int Function GetCurrentColorIndex() Global Native
Bool Function SetColorIndex(Int index) Global Native
