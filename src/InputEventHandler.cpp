#include "InputHandler.h"

namespace
{
    class HotkeyContext
    {
    public:
        inline static std::string logBool(bool& bool_to_log)
        {
            if (bool_to_log)
                return "true";
            if (!bool_to_log)
                return "false";
        }

        explicit HotkeyContext(const Settings* settings)
            : hotkeyDual(settings->DualBlockKey->value), hotkey(settings->blockKeyKeyboard), hotkeyMouse(settings->blockKeyMouse), hotkeyGamepad(settings->blockKeyGamePad)
        {
        }

        void Update(const RE::ButtonEvent* a_button)
        {
            if (!a_button->HasIDCode()) {
                return;
            }

            if (a_button->IsPressed() && !a_button->IsHeld()) {
                auto key = CLib::ParseKey(a_button->GetIDCode(), a_button->GetDevice());

                hotkey.Update(key);
                hotkeyDual.Update(key);
                hotkeyGamepad.Update(key);
                hotkeyMouse.Update(key);
            }
        }

        void Finalize(Input::InputEventSink* app)
        {
            RE::PlayerCharacter* player   = Cache::GetPlayerSingleton();
            const Settings*      settings = Settings::GetSingleton();
            for (std::uint32_t count = 2; count > 0; --count) {
                bool done = false;
                if (hotkeyMouse.IsActive() && !Conditions::PlayerHasActiveMagicEffect(settings->MAG_ParryWindowEffect)) {
                    logger::debug("block key was pressed");
                    Conditions::ApplySpell(player, player, settings->MAGParryControllerSpell);
                    logger::debug("PARRY SPELL APPLIED");
                    done = true;
                }
                if (hotkeyGamepad.IsActive() && !Conditions::PlayerHasActiveMagicEffect(settings->MAG_ParryWindowEffect)) {
                    logger::debug("block key was pressed");
                    Conditions::ApplySpell(player, player, settings->MAGParryControllerSpell);
                    logger::debug("PARRY SPELL APPLIED");
                    done = true;
                }

                if (done) {
                    break;
                }
            }
        }

    private:
        CLib::Key hotkey;
        CLib::Key hotkeyDual;
        CLib::Key hotkeyMouse;
        CLib::Key hotkeyGamepad;
    };
} // namespace

void Input::HotkeyManager::Process(const RE::InputEvent* const* a_event)
{
    auto config = Settings::GetSingleton();

    HotkeyContext ctx{ config };

    for (auto event = *a_event; event; event = event->next) {
        if (auto button = event->AsButtonEvent()) {
            ctx.Update(button);
        }
    }

    auto app = Input::InputEventSink::GetSingleton();

    ctx.Finalize(app);
}
