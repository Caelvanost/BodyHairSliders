#include "Settings.h"

namespace BHS
{
    namespace
    {
        std::string NormalizeId(std::string value)
        {
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
                if (std::isalnum(c)) {
                    return static_cast<char>(std::tolower(c));
                }
                return '_';
            });

            while (value.find("__") != std::string::npos) {
                value.replace(value.find("__"), 2, "_");
            }
            while (!value.empty() && value.front() == '_') {
                value.erase(value.begin());
            }
            while (!value.empty() && value.back() == '_') {
                value.pop_back();
            }
            return value;
        }

        std::string MakeLabel(std::string value)
        {
            std::replace(value.begin(), value.end(), '_', ' ');
            return value;
        }

        bool EndsWithInsensitive(std::string_view value, std::string_view suffix)
        {
            if (suffix.size() > value.size()) {
                return false;
            }
            const auto offset = value.size() - suffix.size();
            for (std::size_t i = 0; i < suffix.size(); ++i) {
                if (std::tolower(static_cast<unsigned char>(value[offset + i])) !=
                    std::tolower(static_cast<unsigned char>(suffix[i]))) {
                    return false;
                }
            }
            return true;
        }
    }

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
                            style.textureDark = styleEntry.value("textureDark", "");
                            style.textureFair = styleEntry.value("textureFair", "");
                            style.sex = styleEntry.value("sex", "any");
                            style.type = styleEntry.value("type", "overlay");
                            provider.styles.push_back(std::move(style));
                        }
                    }

                    if (providerEntry.contains("scanRules") && providerEntry.at("scanRules").is_array()) {
                        for (const auto& ruleEntry : providerEntry.at("scanRules")) {
                            ScanRule rule;
                            rule.directory = ruleEntry.value("directory", "");
                            rule.region = ruleEntry.value("region", "");
                            rule.sex = ruleEntry.value("sex", "any");
                            rule.prefix = ruleEntry.value("prefix", "");
                            rule.suffix = ruleEntry.value("suffix", ".dds");
                            rule.excludeSuffix = ruleEntry.value("excludeSuffix", "");
                            rule.pairDarkFair = ruleEntry.value("pairDarkFair", false);
                            provider.scanRules.push_back(std::move(rule));
                        }
                    }

                    ScanProvider(provider);
                    providers.push_back(std::move(provider));
                }
            }

            std::size_t styleCount = 0;
            std::size_t detectedCount = 0;
            for (const auto& provider : providers) {
                styleCount += provider.styles.size();
                detectedCount += provider.detected ? 1 : 0;
            }

            SKSE::log::info("Loaded {} providers ({} detected) with {} body-hair styles",
                providers.size(), detectedCount, styleCount);
            return true;
        } catch (const std::exception& e) {
            SKSE::log::error("Failed to parse config.json: {}", e.what());
            return false;
        }
    }

    void Settings::ScanProvider(Provider& provider)
    {
        std::unordered_map<std::string, std::size_t> pairedStyles;

        for (const auto& rule : provider.scanRules) {
            if (rule.directory.empty()) {
                continue;
            }

            const std::filesystem::path directory(rule.directory);
            std::error_code ec;
            if (!std::filesystem::exists(directory, ec)) {
                continue;
            }

            provider.detected = true;

            for (const auto& entry : std::filesystem::directory_iterator(directory, ec)) {
                if (ec || !entry.is_regular_file()) {
                    continue;
                }

                const auto filename = entry.path().filename().string();
                if (!EndsWithInsensitive(filename, rule.suffix)) {
                    continue;
                }
                if (!rule.excludeSuffix.empty() && EndsWithInsensitive(filename, rule.excludeSuffix)) {
                    continue;
                }
                if (!rule.prefix.empty() && !filename.starts_with(rule.prefix)) {
                    continue;
                }

                auto stem = entry.path().stem().string();
                if (!rule.prefix.empty() && stem.starts_with(rule.prefix)) {
                    stem.erase(0, rule.prefix.size());
                }

                std::string sourceColor;
                if (rule.pairDarkFair) {
                    if (stem.starts_with("Dark_")) {
                        sourceColor = "dark";
                        stem.erase(0, 5);
                    } else if (stem.starts_with("Fair_")) {
                        sourceColor = "fair";
                        stem.erase(0, 5);
                    }
                }

                const auto key = provider.id + ":" + rule.region + ":" + rule.sex + ":" + NormalizeId(stem);
                const auto texturePath = entry.path().generic_string();

                if (rule.pairDarkFair && !sourceColor.empty()) {
                    auto it = pairedStyles.find(key);
                    if (it == pairedStyles.end()) {
                        OverlayStyle style;
                        style.id = NormalizeId(provider.id + "_" + rule.region + "_" + stem);
                        style.provider = provider.id;
                        style.region = rule.region;
                        style.label = MakeLabel(stem);
                        style.sex = rule.sex;
                        provider.styles.push_back(std::move(style));
                        const auto index = provider.styles.size() - 1;
                        pairedStyles.emplace(key, index);
                        it = pairedStyles.find(key);
                    }

                    auto& style = provider.styles[it->second];
                    if (sourceColor == "dark") {
                        style.textureDark = texturePath;
                    } else {
                        style.textureFair = texturePath;
                    }
                } else {
                    OverlayStyle style;
                    style.id = NormalizeId(provider.id + "_" + rule.region + "_" + stem);
                    style.provider = provider.id;
                    style.region = rule.region;
                    style.label = MakeLabel(stem);
                    style.texture = texturePath;
                    style.sex = rule.sex;
                    provider.styles.push_back(std::move(style));
                }
            }
        }

        if (!provider.plugin.empty()) {
            if (const auto* handler = RE::TESDataHandler::GetSingleton(); handler && handler->LookupModByName(provider.plugin)) {
                provider.detected = true;
            }
        }

        SKSE::log::info("Provider '{}' detected={} styles={}", provider.name, provider.detected, provider.styles.size());
    }

    std::vector<const OverlayStyle*> Settings::GetStylesForRegion(std::string_view region, std::string_view sex) const
    {
        std::vector<const OverlayStyle*> result;
        for (const auto& provider : providers) {
            if (!provider.detected) {
                continue;
            }
            for (const auto& style : provider.styles) {
                if (style.region == region &&
                    (sex == "any" || style.sex == "any" || style.sex == sex)) {
                    result.push_back(&style);
                }
            }
        }
        return result;
    }
}
