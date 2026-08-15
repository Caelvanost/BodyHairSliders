#pragma once

namespace BHS
{
    struct OverlayStyle
    {
        std::string id;
        std::string provider;
        std::string region;
        std::string label;
        std::string texture;
        std::string textureDark;
        std::string textureFair;
        std::string sex{ "any" };
        std::string type{ "overlay" };
    };

    struct ScanRule
    {
        std::string directory;
        std::string region;
        std::string sex{ "any" };
        std::string prefix;
        std::string suffix{ ".dds" };
        bool pairDarkFair{ false };
    };

    struct Provider
    {
        std::string id;
        std::string name;
        std::string plugin;
        std::vector<OverlayStyle> styles;
        std::vector<ScanRule> scanRules;
        bool detected{ false };
    };

    struct Settings
    {
        std::vector<Provider> providers;
        bool matchHairByDefault{ true };

        static Settings& GetSingleton();
        bool Load();

        std::vector<const OverlayStyle*> GetStylesForRegion(std::string_view region, std::string_view sex = "any") const;

    private:
        void ScanProvider(Provider& provider);
    };
}
