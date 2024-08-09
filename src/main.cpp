#include "Cache.h"
#include "Events.h"
#include "Hooks.h"
#include "InputHandler.h"
#include "MenuEventHandler.h"
#include "PickpocketReplace.h"

void InitLogger()
{
    auto path{ SKSE::log::log_directory() };
    if (!path)
        stl::report_and_fail("Unable to lookup SKSE logs directory.");
    *path /= SKSE::PluginDeclaration::GetSingleton()->GetName();
    *path += L".log";

    std::shared_ptr<spdlog::logger> log;
    if (IsDebuggerPresent())
        log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
    else
        log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));

    log->set_level(spdlog::level::level_enum::info);
    log->flush_on(spdlog::level::level_enum::trace);

    set_default_logger(std::move(log));

    spdlog::set_pattern("[%T.%e UTC%z] [%L] [%=5t] %v");
}

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
    auto settings = Settings::GetSingleton();
    switch (a_msg->type) {
    case SKSE::MessagingInterface::kNewGame:
    case SKSE::MessagingInterface::kPostLoadGame:
        Settings::GetSingleton()->SetGlobalsAndGameSettings();
        break;
    case SKSE::MessagingInterface::kPostLoad:
        if (!Hooks::InstallBashMultHook()) {
            logger::error("Bash hook installation failed.");
        }
        else {
            logger::info("Bash hook installed");
        }

        break;
    case SKSE::MessagingInterface::kDataLoaded:
        if (settings) {
            settings->LoadForms();
            settings->AdjustWeaponStaggerVals();
        }
        AnimationGraphEventHandler::Register();
        AnimationGraphEventHandler::RegisterAnimHook();
        OnHitEventHandler::Register();
        Input::InputEventSink::Register();
        Input::InputEventSink::GetSingleton()->GetMappedKey();
        MenuEventHandler::MenuEvent::GetSingleton()->RegisterMenuEvents();

        break;
    }
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v{};
    v.PluginVersion(REL::Version{ 2, 7, 0, 0 });
    v.PluginName("ValorPerks"sv);
    v.AuthorName("colinswrath and Kernalsegg modified by Styyx"sv);
    v.UsesAddressLibrary(true);
    v.HasNoStructUse(true);
    v.UsesStructsPost629(false);
    return v;
}();

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitLogger(); // 1.6

    SKSE::Init(skse);
    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} loading...", plugin->GetName(), version);

    SKSE::AllocTrampoline(320);
    Cache::CacheAddLibAddresses();
    Settings::GetSingleton()->LoadSettings();
    if (!Hooks::InstallHooks()) {
        logger::error("Hook installation failed.");
        return false;
    }
    PickpocketReplace::Install();

    auto messaging = SKSE::GetMessagingInterface();
    if (!messaging->RegisterListener(InitListener)) {
        return false;
    }

    logger::info("Valor Perks loaded.");
    spdlog::default_logger()->flush();
    return true;
}
