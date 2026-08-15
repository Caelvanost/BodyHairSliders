#pragma once

namespace NBHS
{
    struct OverlayStyle
    {
        std::string id;
        std::string label;
        std::string texture;
    };

    struct Settings
    {
        std::vector<OverlayStyle> pubic;
        std::vector<OverlayStyle> armpits;
        std::vector<OverlayStyle> butt;
        bool matchHairByDefault{ true };

        static Settings& GetSingleton();
        bool Load();
    };
}
