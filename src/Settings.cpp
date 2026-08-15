#include "Settings.h"

namespace BHS
{
    Settings& Settings::GetSingleton()
    {
        static Settings singleton;
        return singleton;
    }

    bool Settings::Load()
    {
        constexpr auto path = "Data/SKSE/Plugins/BodyHairSliders/config.json";
        std::ifstream file(path);
        if (!file) {
            SKSE::log::error("Could not open {}", path);
            return false;
        }

        try {
            nlohmann::json root;
            file >> root;

            providers.clear();
            matchHairByDefault = root.value("matchHairByDefault", true);

            if (root.contains("providers") && root.at("providers").is_array()) {
                for (const auto& providerEntry : root.at("providers")) {
                    Provider provider;
                    provider.id = providerEntry.value("id", "");
                    provider.name = providerEntry.value("name", provider.id);
                    provider.plugin = providerEntry.value("plugin", "");

                    if (providerEntry.contains("styles") && providerEntry.at("styles").is_array()) {
                        for (const auto& styleEntry : providerEntry.at("styles")) {
                            OverlayStyle style;
                            style.id = styleEntry.value("id", "");
                            style.provider = provider.id;
                            style.region = styleEntry.value("region", "");
                            style.label = styleEntry.value("label", style.id);
                            style.texture = styleEntry.value("texture", "");
                            style.type = styleEntry.value("type", "overlay");
                            provider.styles.push_back(std::move(style));
                        }
                    }

                    providers.push_back(std::move(provider));
                }
            }

            std::size_t styleCount = 0;
            for (const auto& provider : providers) {
                styleCount += provider.styles.size();
            }

            SKSE::log::info("Loaded {} body-hair providers with {} styles", providers.size(), styleCount);
            return true;
        } catch (const std::exception& e) {
            SKSE::log::error("Failed to parse config.json: {}", e.what());
            return false;
        }
    }

    std::vector<const OverlayStyle*> Settings::GetStylesForRegion(std::string_view region) const
    {
        std::vector<const OverlayStyle*> result;
        for (const auto& provider : providers) {
            for (const auto& style : provider.styles) {
                if (style.region == region) {
                    result.push_back(&style);
                }
            }
        }
        return result;
    }
}
