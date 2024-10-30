#pragma once
#include "CLib/Key.h"
#include "Cache.h"
#include "Conditions.h"
#include "Settings.h"

#define continueEvent RE::BSEventNotifyControl::kContinue

namespace Input
{
    class HotkeyManager
    {
    public:
        static void Process(const RE::InputEvent* const* a_event);
    };

    class InputEventSink final : public Singleton<InputEventSink>, public RE::BSTEventSink<RE::InputEvent*>
    {
        friend class Singleton<InputEventSink>;

    public:
        using Event       = RE::InputEvent*;
        using EventSource = RE::BSTEventSource<Event>;

        inline static bool          key_active{ false };
        inline static std::uint32_t blockKey[RE::INPUT_DEVICE::kFlatTotal] = { 0xFF, 0xFF, 0xFF };

        static void Register()
        {
            if (auto manager = RE::BSInputDeviceManager::GetSingleton()) {
                manager->AddEventSink(GetSingleton());
                SKSE::log::info("Successfully registered input event.");
            }
            else {
                SKSE::log::error("Failed to register input event.");
            }
        };

        inline std::uint32_t RemapKey(std::uint32_t a_key, RE::INPUT_DEVICE a_device)
        {
            switch (a_device) {
            case RE::INPUT_DEVICE::kKeyboard:
                return a_key;
            case RE::INPUT_DEVICE::kMouse:
                return a_key + SKSE::InputMap::kMacro_MouseButtonOffset;
            case RE::INPUT_DEVICE::kGamepad:
                return SKSE::InputMap::GamepadMaskToKeycode(a_key);
            default:
                return a_key;
            }
        }

        void GetMappedKey()
        {
            const RE::ControlMap* cm        = RE::ControlMap::GetSingleton();
            const RE::UserEvents* userEvent = RE::UserEvents::GetSingleton();
            Settings*             settings  = Settings::GetSingleton();

            for (int i = RE::INPUT_DEVICE::kKeyboard; i <= RE::INPUT_DEVICE::kGamepad; ++i) {
                switch (i) {
                case RE::INPUT_DEVICE::kKeyboard:
                    blockKey[i] = cm->GetMappedKey(userEvent->leftAttack, RE::INPUT_DEVICE::kKeyboard);
                    logger::debug("____GET MAPPED KEY____ KeyCode for keyboard block is {}", blockKey[i]);
                    settings->blockKeyKeyboard = blockKey[RE::INPUT_DEVICE::kKeyboard];
                    logger::debug("____GET MAPPED KEY____ KeyCode for chached keyboard block is {}", settings->blockKeyKeyboard);
                    break;
                case RE::INPUT_DEVICE::kMouse:
                    blockKey[i] = SKSE::InputMap::kMacro_MouseButtonOffset + cm->GetMappedKey(userEvent->leftAttack, RE::INPUT_DEVICE::kMouse);
                    logger::debug("____GET MAPPED KEY____ KeyCode for mouse block is {}", blockKey[i]);
                    settings->blockKeyMouse = blockKey[RE::INPUT_DEVICE::kMouse];
                    logger::debug("____GET MAPPED KEY____ KeyCode for chached keyboard block is {}", settings->blockKeyMouse);
                    break;
                case RE::INPUT_DEVICE::kGamepad:
                    blockKey[i] = SKSE::InputMap::GamepadMaskToKeycode(cm->GetMappedKey(userEvent->leftAttack, RE::INPUT_DEVICE::kGamepad));
                    logger::debug("____GET MAPPED KEY____ KeyCode for Gamepad block is {}", blockKey[i]);
                    settings->blockKeyGamePad = blockKey[RE::INPUT_DEVICE::kGamepad];
                    logger::debug("____GET MAPPED KEY____ KeyCode for chached keyboard block is {}", settings->blockKeyGamePad);
                    break;
                }
            }
        }

        bool IsCorrectKey(uint32_t compare_key, RE::INPUT_DEVICE device)
        {
            Settings* settings = Settings::GetSingleton();

            if (compare_key == blockKey[device]) {
                return true;
            }
            else if (compare_key == settings->DualBlockKey->value) {
                return true;
            }
            else {
                return false;
            }
        }

        inline static bool updating{ false };

        RE::BSEventNotifyControl ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
        {
            if (!a_event || !RE::Main::GetSingleton()->gameActive) {
                return RE::BSEventNotifyControl::kContinue;
            }

            Input::HotkeyManager::Process(a_event);

            return RE::BSEventNotifyControl::kContinue;
        }

    private:
        InputEventSink() = default;
    };

} // namespace Input
