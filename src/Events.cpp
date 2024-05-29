#include "Events.h"

inline void AnimationGraphEventHandler::StaminaCost(RE::Actor* actor, double cost)
{
        logger::info("stamina for attacks is {}", cost);
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
        if (actor == player && !player->IsGodMode()) {
            actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, cost * -1.0);
        }
        else
            actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, cost * -1.0);
}

inline void AnimationGraphEventHandler::ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event,
                                                     RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
{
    const char* jumpAnimEventString = "JumpUp";
    const char* HitString           = "HitFrame";

    if (!a_event) {
        return;
    }

    if (!a_event->tag.empty() && a_event->holder && a_event->holder->As<RE::Actor>()) {
        if (std::strcmp(a_event->tag.c_str(), jumpAnimEventString) == 0) {
            HandleJumpAnim();
        }
    }

    if (!a_event->tag.empty() && a_event->holder && a_event->holder->As<RE::Actor>()) {
        if (std::strcmp(a_event->tag.c_str(), HitString) == 0) {
            if (a_event->holder->As<RE::Actor>()) {
                RE::PlayerCharacter* player      = Cache::GetPlayerSingleton();
                auto                 actor       = const_cast<RE::TESObjectREFR*>(a_event->holder)->As<RE::Actor>();
                auto                 wieldedWeap = Conditions::getWieldingWeapon(actor);
                const Settings*      settings    = Settings::GetSingleton();
                RE::TESGlobal*       stamGlob    = settings->StaminaCostGlobal;
                auto                 global      = stamGlob->value;
                auto                 npc_glob    = settings->NPCStaminaCostGlobal->value;
                double               stam_cost   = 10.0;

                if (actor == player) {
                    if (wieldedWeap && wieldedWeap->IsWeapon() && wieldedWeap->IsMelee()) {
                        bool dagger     = wieldedWeap->IsOneHandedDagger();
                        bool sword      = wieldedWeap->IsOneHandedSword();
                        bool mace       = wieldedWeap->IsOneHandedMace();
                        bool axe        = wieldedWeap->IsOneHandedAxe();
                        bool greatsword = wieldedWeap->IsTwoHandedSword();
                        bool greataxe   = wieldedWeap->IsTwoHandedAxe();

                        if (sword || axe || mace) {
                            logger::info("stamina cost is {} for sword, axe or mace", stam_cost);
                            stam_cost = global;
                        }
                        else if (greatsword || greataxe) {
                            stam_cost = global * 1.5;
                            logger::info("stamina cost is {} for Greatsword, Greataxe or Greatmace", stam_cost);
                        }
                        else if (dagger || wieldedWeap->IsHandToHandMelee()) {
                            stam_cost = global * 0.8;
                            logger::info("stamina cost is {} for dagger and unarmed", stam_cost);
                        }
                    }
                    else
                        stam_cost = global * 0.8;
                }
                else {
                    if (actor != player) {
                        if (wieldedWeap && wieldedWeap->IsWeapon() && wieldedWeap->IsMelee()) {
                            bool dagger     = wieldedWeap->IsOneHandedDagger();
                            bool sword      = wieldedWeap->IsOneHandedSword();
                            bool mace       = wieldedWeap->IsOneHandedMace();
                            bool axe        = wieldedWeap->IsOneHandedAxe();
                            bool greatsword = wieldedWeap->IsTwoHandedSword();
                            bool greataxe   = wieldedWeap->IsTwoHandedAxe();

                            if (sword || axe || mace) {
                                logger::info("stamina cost is {} for sword, axe or mace", stam_cost);
                                stam_cost = npc_glob;
                            }
                            else if (greatsword || greataxe) {
                                stam_cost = npc_glob * 1.5;
                                logger::info("stamina cost is {} for Greatsword, Greataxe or Greatmace", stam_cost);
                            }
                            else if (dagger || wieldedWeap->IsHandToHandMelee()) {
                                stam_cost = npc_glob * 0.8;
                                logger::info("stamina cost is {} for dagger and unarmed", stam_cost);
                            }
                        }
                        else
                            stam_cost = npc_glob * 0.8;
                    }
                }
                if (!Conditions::IsPowerAttacking(actor)) {
                    logger::info("{} is attacking and it costs {} stamina", actor->GetName(), stam_cost);
                    StaminaCost(actor, stam_cost);
                }
            }
        }
    }
    return;

}

EventResult AnimationGraphEventHandler::ProcessEvent_NPC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event,
                                                         RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
{
    ProcessEvent(a_sink, a_event, a_eventSource);
    return _ProcessEvent_NPC(a_sink, a_event, a_eventSource);
}

EventResult AnimationGraphEventHandler::ProcessEvent_PC(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event,
                                                        RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
{
    ProcessEvent(a_sink, a_event, a_eventSource);
    return _ProcessEvent_PC(a_sink, a_event, a_eventSource);
}




