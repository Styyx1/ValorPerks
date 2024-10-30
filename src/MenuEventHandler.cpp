#include "MenuEventHandler.h"

RE::BSEventNotifyControl MenuEventHandler::MenuEvent::ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
{
    auto            input_event  = Input::InputEventSink::GetSingleton();
    auto            journal_menu = RE::JournalMenu::MENU_NAME;
    const Settings* settings     = Settings::GetSingleton();
    RE::UI*         menu         = RE::UI::GetSingleton();

    if (!event) {
        return continueEvent;
    }
    if (event->opening) {
        return continueEvent;
    }
    if (event->menuName != journal_menu)
        return continueEvent;

    if (event->menuName == journal_menu) {
        if (!event->opening) {
            input_event->GetMappedKey();
            logger::debug("got mapped keys");
        }
    }

    return continueEvent;
};
