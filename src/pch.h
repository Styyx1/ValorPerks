#pragma once

#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include "SimpleIni.h"

// Xbyak
#include <xbyak/xbyak.h>

#define DLLEXPORT     __declspec(dllexport)
#define continueEvent RE::BSEventNotifyControl::kContinue;

using namespace std::literals;
using namespace REL::literals;

namespace logger = SKSE::log;
#define dlog logger::debug


template <typename T>
class Singleton
{
protected:
    constexpr Singleton() noexcept  = default;
    constexpr ~Singleton() noexcept = default;

public:
    constexpr Singleton(const Singleton&)      = delete;
    constexpr Singleton(Singleton&&)           = delete;
    constexpr auto operator=(const Singleton&) = delete;
    constexpr auto operator=(Singleton&&)      = delete;

    [[nodiscard]] static constexpr T* GetSingleton() noexcept
    {
        static T singleton;
        return std::addressof(singleton);
    }
};

template <typename TDerived, typename TEvent>
class EventSingleton : public RE::BSTEventSink<TEvent>
{
protected:
    constexpr EventSingleton() noexcept           = default;
    constexpr ~EventSingleton() noexcept override = default;

public:
    constexpr EventSingleton(const EventSingleton&) = delete;
    constexpr EventSingleton(EventSingleton&&)      = delete;
    constexpr auto operator=(const EventSingleton&) = delete;
    constexpr auto operator=(EventSingleton&&)      = delete;

    [[nodiscard]] static constexpr TDerived* GetSingleton() noexcept
    {
        static TDerived singleton;
        return std::addressof(singleton);
    }

    static constexpr void Register() noexcept
    {
        using TEventSource = RE::BSTEventSource<TEvent>;

        constexpr std::string dirty_name{ typeid(TEvent).name() };
        const std::regex      p{ "struct |RE::|SKSE::| * __ptr64" };
        const auto            name{ std::regex_replace(dirty_name, p, "") };

        if constexpr (std::is_base_of_v<TEventSource, RE::BSInputDeviceManager>) {
            const auto manager{ RE::BSInputDeviceManager::GetSingleton() };
            manager->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_base_of_v<TEventSource, RE::UI>) {
            const auto ui{ RE::UI::GetSingleton() };
            ui->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::ActionEvent>) {
            SKSE::GetActionEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::CameraEvent>) {
            SKSE::GetCameraEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::CrosshairRefEvent>) {
            SKSE::GetCrosshairRefEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::ModCallbackEvent>) {
            SKSE::GetModCallbackEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_same_v<TEvent, SKSE::NiNodeUpdateEvent>) {
            SKSE::GetNiNodeUpdateEventSource()->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        else if constexpr (std::is_base_of_v<TEventSource, RE::ScriptEventSourceHolder>) {
            const auto holder{ RE::ScriptEventSourceHolder::GetSingleton() };
            holder->AddEventSink(GetSingleton());
            logger::info("Registered {} handler", name);
            return;
        }
        const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
        SKSE::stl::report_and_fail(fmt::format("{}: Failed to register {} handler", plugin->GetName(), name));
    }
};

namespace stl
{
    using namespace SKSE::stl;

    template <typename T>
    constexpr auto write_thunk_call() noexcept
    {
        SKSE::AllocTrampoline(14);
        auto& trampoline{ SKSE::GetTrampoline() };
        T::func = trampoline.write_call<5>(T::address, T::Thunk);
    }

    template <typename TDest, typename TSource>
    constexpr auto write_vfunc() noexcept
    {
        REL::Relocation<std::uintptr_t> vtbl{ TDest::VTABLE[0] };
        TSource::func = vtbl.write_vfunc(TSource::idx, TSource::Thunk);
    }

    template <typename T>
    constexpr auto write_vfunc(const REL::VariantID variant_id) noexcept
    {
        REL::Relocation<std::uintptr_t> vtbl{ variant_id };
        T::func = vtbl.write_vfunc(T::idx, T::Thunk);
    }
} // namespace stl
