#pragma once

namespace BHS::SKEE
{
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using i32 = std::int32_t;

    class IPluginInterface
    {
    public:
        virtual ~IPluginInterface() = default;
        virtual u32 GetVersion() = 0;
        virtual void Revert() = 0;
    };

    class IInterfaceMap
    {
    public:
        virtual IPluginInterface* QueryInterface(const char* name) = 0;
        virtual bool AddInterface(const char* name, IPluginInterface* pluginInterface) = 0;
        virtual IPluginInterface* RemoveInterface(const char* name) = 0;
    };

    struct InterfaceExchangeMessage
    {
        static constexpr u32 kMessage_ExchangeInterface = 0x9E3779B9;
        IInterfaceMap* interfaceMap{ nullptr };
    };

    class IOverlayInterface : public IPluginInterface
    {
    public:
        enum class OverlayType { Normal, Spell };
        enum class OverlayLocation { Body, Hand, Feet, Face };
        using OverlayInstallCallback = void (*)(RE::TESObjectREFR*, RE::NiAVObject*);

        virtual bool HasOverlays(RE::TESObjectREFR* reference) = 0;
        virtual void AddOverlays(RE::TESObjectREFR* reference, bool defer = true) = 0;
        virtual void RemoveOverlays(RE::TESObjectREFR* reference, bool defer = true) = 0;
        virtual void RevertOverlays(RE::TESObjectREFR* reference, bool resetDiffuse, bool defer = true) = 0;
        virtual void RevertOverlay(RE::TESObjectREFR* reference, const char* nodeName, u32 armorMask, u32 addonMask, bool resetDiffuse, bool defer = true) = 0;
        virtual void EraseOverlays(RE::TESObjectREFR* reference, bool defer = true) = 0;
        virtual void RevertHeadOverlays(RE::TESObjectREFR* reference, bool resetDiffuse, bool defer = true) = 0;
        virtual void RevertHeadOverlay(RE::TESObjectREFR* reference, const char* nodeName, u32 partType, u32 shaderType, bool resetDiffuse, bool defer = true) = 0;
        virtual u32 GetOverlayCount(OverlayType type, OverlayLocation location) = 0;
        virtual const char* GetOverlayFormat(OverlayType type, OverlayLocation location) = 0;
        virtual bool RegisterInstallCallback(const char* key, OverlayInstallCallback cb) = 0;
        virtual bool UnregisterInstallCallback(const char* key) = 0;
    };

    class IOverrideInterface : public IPluginInterface
    {
    public:
        class GetVariant
        {
        public:
            virtual void Int(i32) = 0;
            virtual void Float(float) = 0;
            virtual void String(const char*) = 0;
            virtual void Bool(bool) = 0;
            virtual void TextureSet(const RE::BGSTextureSet*) = 0;
        };

        class SetVariant
        {
        public:
            enum class Type { None, Int, Float, String, Bool, TextureSet };
            virtual Type GetType() { return Type::None; }
            virtual i32 Int() { return 0; }
            virtual float Float() { return 0.0F; }
            virtual const char* String() { return nullptr; }
            virtual bool Bool() { return false; }
            virtual RE::BGSTextureSet* TextureSet() { return nullptr; }
        };

        virtual bool HasArmorAddonNode(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*, bool) = 0;
        virtual bool HasArmorOverride(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*, u16, u8) = 0;
        virtual void AddArmorOverride(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*, u16, u8, SetVariant&) = 0;
        virtual bool GetArmorOverride(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*, u16, u8, GetVariant&) = 0;
        virtual void RemoveArmorOverride(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*, u16, u8) = 0;
        virtual void SetArmorProperties(RE::TESObjectREFR*, bool) = 0;
        virtual void SetArmorProperty(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*, u16, u8, SetVariant&, bool) = 0;
        virtual bool GetArmorProperty(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*, u16, u8, GetVariant&) = 0;
        virtual void ApplyArmorOverrides(RE::TESObjectREFR*, RE::TESObjectARMO*, RE::TESObjectARMA*, RE::NiAVObject*, bool) = 0;
        virtual void RemoveAllArmorOverrides() = 0;
        virtual void RemoveAllArmorOverridesByReference(RE::TESObjectREFR*) = 0;
        virtual void RemoveAllArmorOverridesByArmor(RE::TESObjectREFR*, bool, RE::TESObjectARMO*) = 0;
        virtual void RemoveAllArmorOverridesByAddon(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*) = 0;
        virtual void RemoveAllArmorOverridesByNode(RE::TESObjectREFR*, bool, RE::TESObjectARMO*, RE::TESObjectARMA*, const char*) = 0;

        virtual bool HasNodeOverride(RE::TESObjectREFR*, bool, const char*, u16, u8) = 0;
        virtual void AddNodeOverride(RE::TESObjectREFR*, bool, const char*, u16, u8, SetVariant&) = 0;
        virtual bool GetNodeOverride(RE::TESObjectREFR*, bool, const char*, u16, u8, GetVariant&) = 0;
        virtual void RemoveNodeOverride(RE::TESObjectREFR*, bool, const char*, u16, u8) = 0;
        virtual void SetNodeProperties(RE::TESObjectREFR*, bool immediate) = 0;
    };

    class IActorUpdateManager : public IPluginInterface
    {
    public:
        virtual void AddBodyUpdate(u32 formId) = 0;
        virtual void AddTransformUpdate(u32 formId) = 0;
        virtual void AddOverlayUpdate(u32 formId) = 0;
        virtual void AddNodeOverrideUpdate(u32 formId) = 0;
        virtual void AddWeaponOverrideUpdate(u32 formId) = 0;
        virtual void AddAddonOverrideUpdate(u32 formId) = 0;
        virtual void AddSkinOverrideUpdate(u32 formId) = 0;
        virtual void Flush() = 0;
    };

    inline constexpr u16 kShaderTintColor = 7;
    inline constexpr u16 kShaderAlpha = 8;
    inline constexpr u16 kShaderTexture = 9;
}
