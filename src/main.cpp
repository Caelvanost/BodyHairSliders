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
        *path /= "NordicBodyHairSliders.log";
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
        auto logger = std::make_shared<spdlog::logger>("global log", std::move(sink));
        spdlog::set_default_logger(std::move(logger));
        spdlog::set_level(spdlog::level::info);
        spdlog::flush_on(spdlog::level::info);
    }

    void OnMessage(SKSE::MessagingInterface::Message* message)
    {
        if (!message) {
            return;
        }

        if (message->type == SKSE::MessagingInterface::kDataLoaded) {
            NBHS::Settings::GetSingleton().Load();
            NBHS::RaceMenuIntegration::GetSingleton().Initialize();
            SKSE::log::info("NordicBodyHairSliders data-loaded initialization complete");
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitializeLog();
    SKSE::Init(skse);

    SKSE::log::info("NordicBodyHairSliders v0.1.0 draft loading");

    if (const auto messaging = SKSE::GetMessagingInterface()) {
        messaging->RegisterListener(OnMessage);
    }

    return true;
}
