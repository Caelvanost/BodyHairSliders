#include "HairColor.h"
#include "OverlayManager.h"
#include "PapyrusAPI.h"
#include "RaceMenuIntegration.h"
#include "Settings.h"

#ifndef BHS_VERSION_STRING
#define BHS_VERSION_STRING "unknown"
#endif

namespace
{
    constexpr REL::Version kTargetRuntime{ 1, 6, 640, 0 };
    constexpr std::string_view kTargetRaceMenu = "0.4.19.14";

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

    bool ValidateRuntime()
    {
        const auto runtime = REL::Module::get().version();
        SKSE::log::info(
            "Compatibility target: Skyrim {} / RaceMenu {}",
            kTargetRuntime.string(),
            kTargetRaceMenu);
        SKSE::log::info("Detected Skyrim runtime: {}", runtime.string());

        if (runtime != kTargetRuntime) {
            SKSE::log::error(
                "This test build only supports Skyrim {}. Detected {}. Refusing to load to avoid using the wrong compatibility branch.",
                kTargetRuntime.string(),
                runtime.string());
            return false;
        }

        return true;
    }

    void LogOverlayCapacity()
    {
        auto* overlay = BHS::RaceMenuIntegration::GetSingleton().Overlay();
        if (!overlay) {
            return;
        }

        using Type = BHS::SKEE::IOverlayInterface::OverlayType;
        using Location = BHS::SKEE::IOverlayInterface::OverlayLocation;
        SKSE::log::info(
            "RaceMenu normal overlay capacity: body={} hands={} feet={} face={}",
            overlay->GetOverlayCount(Type::Normal, Location::Body),
            overlay->GetOverlayCount(Type::Normal, Location::Hand),
            overlay->GetOverlayCount(Type::Normal, Location::Feet),
            overlay->GetOverlayCount(Type::Normal, Location::Face));
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
            if (BHS::RaceMenuIntegration::GetSingleton().Initialize()) {
                LogOverlayCapacity();
                SKSE::log::info("BodyHairSliders data-loaded initialization complete");
            } else {
                SKSE::log::error("BodyHairSliders RaceMenu integration unavailable; sliders will not apply overlays");
            }
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

    SKSE::log::info("BodyHairSliders v{} loading - Skyrim 1.6.640 / RaceMenu 0.4.19.14 TEST", BHS_VERSION_STRING);

    if (!ValidateRuntime()) {
        return false;
    }

    if (const auto papyrus = SKSE::GetPapyrusInterface()) {
        papyrus->Register(BHS::PapyrusAPI::Register);
    }

    if (const auto messaging = SKSE::GetMessagingInterface()) {
        messaging->RegisterListener(OnMessage);
    }

    return true;
}
