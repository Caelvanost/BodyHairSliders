#include "HairColor.h"
#include "OverlayManager.h"
#include "PapyrusAPI.h"
#include "RaceMenuIntegration.h"
#include "Settings.h"

namespace
{
    void InitializeLog()
    {
        auto path = SKSE::log::log_directory();
        if (!path) {
            return;
        }
        *path /= "BodyHairSliders.log";
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
        auto logger = std::make_shared<spdlog::logger>("global log", std::move(sink));
        spdlog::set_default_logger(std::move(logger));
        spdlog::set_level(spdlog::level::info);
        spdlog::flush_on(spdlog::level::info);
    }

    void RunProofOfConcept()
    {
        auto& settings = BHS::Settings::GetSingleton();
        if (!settings.proofOfConceptEnabled) {
            return;
        }

        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            SKSE::log::error("PoC: player unavailable");
            return;
        }

        const auto styles = settings.GetStylesForRegion(settings.proofOfConceptRegion, settings.proofOfConceptSex);
        if (styles.empty()) {
            SKSE::log::error("PoC: no detected styles for region='{}' sex='{}'",
                settings.proofOfConceptRegion, settings.proofOfConceptSex);
            return;
        }
        if (settings.proofOfConceptStyleIndex >= styles.size()) {
            SKSE::log::error("PoC: styleIndex {} out of range ({} styles)",
                settings.proofOfConceptStyleIndex, styles.size());
            return;
        }

        const auto* style = styles[settings.proofOfConceptStyleIndex];
        const auto color = BHS::HairColor::FromActor(player);
        SKSE::log::info("PoC: applying style {} ({})", style->id, style->label);
        BHS::OverlayManager::GetSingleton().Apply(player, style, color);
    }

    void OnMessage(SKSE::MessagingInterface::Message* message)
    {
        if (!message) {
            return;
        }

        switch (message->type) {
        case SKSE::MessagingInterface::kDataLoaded:
            BHS::Settings::GetSingleton().Load();
            BHS::RaceMenuIntegration::GetSingleton().Initialize();
            SKSE::log::info("BodyHairSliders data-loaded initialization complete");
            break;
        case SKSE::MessagingInterface::kPostLoadGame:
            RunProofOfConcept();
            BHS::PapyrusAPI::ReapplyPlayerSelections();
            break;
        default:
            break;
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitializeLog();
    SKSE::Init(skse);

    SKSE::log::info("BodyHairSliders v0.1.0 draft loading");

    if (const auto papyrus = SKSE::GetPapyrusInterface()) {
        papyrus->Register(BHS::PapyrusAPI::Register);
    }

    if (const auto messaging = SKSE::GetMessagingInterface()) {
        messaging->RegisterListener(OnMessage);
    }

    return true;
}
