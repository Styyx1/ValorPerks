#include "MenuEventHandler.h"

RE::BSEventNotifyControl MenuEventHandler::MenuEvent::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
{
    auto input_event  = Input::InputEventSink::GetSingleton();
    auto journal_menu = RE::JournalMenu::MENU_NAME;
    const Settings* settings     = Settings::GetSingleton();

    if (!event) {
        return RE::BSEventNotifyControl::kContinue;
    }

    if (event->menuName == journal_menu) {
        if (!event->opening) {
            input_event->GetMappedKey();
            SetDualKey();

        }
    }
    return RE::BSEventNotifyControl::kContinue;

    
};

void MenuEventHandler::MenuEvent::SetDualKey() {
    Settings* settings = Settings::GetSingleton();

    settings->dualBlockKey   = settings->DualBlockKey->value;
}
