#include <InputHandler.h>

namespace MenuEventHandler
{
    class MenuEvent : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
    {
        MenuEvent()                            = default;
        MenuEvent(const MenuEvent&)            = delete;
        MenuEvent(MenuEvent&&)                 = delete;
        MenuEvent& operator=(const MenuEvent&) = delete;
        MenuEvent& operator=(MenuEvent&&)      = delete;

    public:
        static MenuEvent* GetSingleton()
        {
            static MenuEvent singleton;
            return &singleton;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;

        void RegisterMenuEvents()
        {
            if (const auto scripts = RE::UI::GetSingleton()) {
                scripts->AddEventSink<RE::MenuOpenCloseEvent>(this);
                logger::info("Registered {}"sv, typeid(RE::MenuOpenCloseEvent).name());
            }
        }
    };
} // namespace MenuEventHandler
