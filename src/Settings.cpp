#include "Settings.h"

namespace NBHS
{
    namespace
    {
        std::vector<OverlayStyle> ReadStyles(const nlohmann::json& root, std::string_view key)
        {
            std::vector<OverlayStyle> result;
            if (!root.contains(key) || !root.at(key).is_array()) {
                return result;
            }

            for (const auto& entry : root.at(key)) {
                OverlayStyle style;
                style.id = entry.value("id", "");
                style.label = entry.value("label", style.id);
                style.texture = entry.value("texture", "");
                result.push_back(std::move(style));
            }
            return result;
        }
    }

    Settings& Settings::GetSingleton()
    {
        static Settings singleton;
        return singleton;
    }

    bool Settings::Load()
    {
        constexpr auto path = "Data/SKSE/Plugins/NordicBodyHairSliders/config.json";
        std::ifstream file(path);
        if (!file) {
            SKSE::log::error("Could not open {}", path);
            return false;
        }

        try {
            nlohmann::json root;
            file >> root;
            pubic = ReadStyles(root, "pubic");
            armpits = ReadStyles(root, "armpits");
            butt = ReadStyles(root, "butt");
            matchHairByDefault = root.value("matchHairByDefault", true);

            SKSE::log::info("Loaded config: {} pubic, {} armpit, {} butt styles",
                pubic.size(), armpits.size(), butt.size());
            return true;
        } catch (const std::exception& e) {
            SKSE::log::error("Failed to parse config.json: {}", e.what());
            return false;
        }
    }
}
