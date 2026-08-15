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
        std::string type{ "overlay" };
    };

    struct Provider
    {
        std::string id;
        std::string name;
        std::string plugin;
        std::vector<OverlayStyle> styles;
    };

    struct Settings
    {
        std::vector<Provider> providers;
        bool matchHairByDefault{ true };

        static Settings& GetSingleton();
        bool Load();

        std::vector<const OverlayStyle*> GetStylesForRegion(std::string_view region) const;
    };
}
