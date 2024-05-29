#pragma once
#include <Conditions.h>
#include <RecentHitEventData.h>
#include <Hooks.h>

class OnHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
{
public:
    std::multimap<std::uint32_t, RecentHitEventData> recentGeneralHits;

    static OnHitEventHandler* GetSingleton()
    {
        static OnHitEventHandler singleton;
        return &singleton;
    }

    // TODO-Temp fix until I can upgrade clib versions
    std::uint32_t GetDurationOfApplicationRunTime()
    {
        REL::Relocation<std::uint32_t*> runtime{ RELOCATION_ID(523662, 410201) };
        return *runtime;
    }

    RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override
    {
        if (!a_event || !a_event->target || !a_event->cause) {
            return RE::BSEventNotifyControl::kContinue;
        }
        auto causeActor  = a_event->cause->As<RE::Actor>();
        auto targetActor = a_event->target->As<RE::Actor>();

        if (causeActor && targetActor && targetActor->IsPlayerRef() && !causeActor->IsPlayerRef()) {
            auto applicationRuntime = GetDurationOfApplicationRunTime();

            bool skipEvent = ShouldSkipHitEvent(causeActor, targetActor, applicationRuntime); // Filters out dupe events

            if (!skipEvent) {
                auto attackingWeapon = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);

                // Something is effed with power attacks. The source isnt coming through and casting as a weapon and the hit flags are empty
                // We can work around it like this
                bool powerAttackMelee = false;
                if (a_event->flags.any(RE::TESHitEvent::Flag::kPowerAttack) || Conditions::IsPowerAttacking(causeActor)) {
                    bool rightIsMeleeWeapon = false;
                    if (auto rightHandForm = causeActor->GetEquippedObject(false)) {
                        if (rightHandForm->IsWeapon() && rightHandForm->As<RE::TESObjectWEAP>()->IsMelee()) {
                            rightIsMeleeWeapon = true;
                        }
                    }

                    bool LeftIsMeleeWeaponOrNone = false;
                    auto leftHandForm            = causeActor->GetEquippedObject(true);
                    if (!leftHandForm || (leftHandForm->IsWeapon() && leftHandForm->As<RE::TESObjectWEAP>()->IsMelee()) || leftHandForm->IsArmor()) {
                        LeftIsMeleeWeaponOrNone = true;
                    }

                    if (rightIsMeleeWeapon && LeftIsMeleeWeaponOrNone) {
                        powerAttackMelee = true;
                    }
                }
                auto defender        = a_event->target->As<RE::Actor>();
                auto defenderProcess = defender->GetActorRuntimeData().currentProcess;
                auto player          = a_event->cause->As<RE::Actor>();
                auto playerAttkData  = player->GetActorRuntimeData().currentProcess->high->attackData;
                if ((defender->AsActorState()->GetLifeState() != RE::ACTOR_LIFE_STATE::kDead) && a_event->cause->IsPlayerRef() && !IsBeastRace()
                    && attackingWeapon->IsHandToHandMelee())
                {
                    ApplyHandToHandXP();
                };

                bool isBlocking      = a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked) || targetActor->IsBlocking();
                auto leftHand        = targetActor->GetEquippedObject(true);
                bool blockedMeleeHit = false;
                if (!a_event->projectile && ((attackingWeapon && attackingWeapon->IsMelee()) || powerAttackMelee) && isBlocking) {
                    blockedMeleeHit = true;
                }

                // Shield Parry (different hit explosion effects)
                if (leftHand && leftHand->IsArmor() && blockedMeleeHit) {
                    auto settings = Settings::GetSingleton();
                    targetActor->PlaceObjectAtMe(settings->APOSparks, false);
                    targetActor->PlaceObjectAtMe(settings->APOSparksPhysics, false);
                    ProcessHitEventForParryShield(targetActor, causeActor);
                }
                else if (blockedMeleeHit) {
                    // Weapon Parry
                    auto settings = Settings::GetSingleton();
                    targetActor->PlaceObjectAtMe(settings->APOSparks, false);
                    targetActor->PlaceObjectAtMe(settings->APOSparksPhysics, false);
                    ProcessHitEventForParry(targetActor, causeActor);
                }
                recentGeneralHits.insert(std::make_pair(applicationRuntime, RecentHitEventData(targetActor, causeActor, applicationRuntime)));
            }
        }
        else if (causeActor && targetActor && causeActor->IsPlayerRef() && !targetActor->IsPlayerRef()) {
            auto applicationRuntime = GetDurationOfApplicationRunTime();

            bool skipEvent = ShouldSkipHitEvent(causeActor, targetActor, applicationRuntime); // Filters out dupe events

            if (!skipEvent) {
                auto attackingWeapon = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);

                // Something is effed with power attacks. The source isnt coming through and casting as a weapon and the hit flags are empty
                // We can work around it like this
                bool powerAttackMelee = false;
                if (a_event->flags.any(RE::TESHitEvent::Flag::kPowerAttack) || Conditions::IsPowerAttacking(causeActor)) {
                    bool rightIsMeleeWeapon = false;
                    if (auto rightHandForm = causeActor->GetEquippedObject(false)) {
                        if (rightHandForm->IsWeapon() && rightHandForm->As<RE::TESObjectWEAP>()->IsMelee()) {
                            rightIsMeleeWeapon = true;
                        }
                    }

                    bool LeftIsMeleeWeaponOrNone = false;
                    auto leftHandForm            = causeActor->GetEquippedObject(true);
                    if (!leftHandForm || (leftHandForm->IsWeapon() && leftHandForm->As<RE::TESObjectWEAP>()->IsMelee()) || leftHandForm->IsArmor()) {
                        LeftIsMeleeWeaponOrNone = true;
                    }

                    if (rightIsMeleeWeapon && LeftIsMeleeWeaponOrNone) {
                        powerAttackMelee = true;
                    }
                }

                bool isBlocking = a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked) || targetActor->IsBlocking();

                auto leftHand = targetActor->GetEquippedObject(true);

                bool blockedMeleeHit = false;
                if (!a_event->projectile && ((attackingWeapon && attackingWeapon->IsMelee()) || powerAttackMelee) && isBlocking) {
                    blockedMeleeHit = true;
                }

                // Shield Parry (different hit explosion effects)
                if (leftHand && leftHand->IsArmor() && blockedMeleeHit) {
                    auto settings = Settings::GetSingleton();
                    targetActor->PlaceObjectAtMe(settings->APOSparks, false);
                    targetActor->PlaceObjectAtMe(settings->APOSparksPhysics, false);
                }
                else if (blockedMeleeHit) {
                    // Weapon Parry
                    auto settings = Settings::GetSingleton();
                    targetActor->PlaceObjectAtMe(settings->APOSparks, false);
                    targetActor->PlaceObjectAtMe(settings->APOSparksPhysics, false);
                }
                

                
                

                recentGeneralHits.insert(std::make_pair(applicationRuntime, RecentHitEventData(targetActor, causeActor, applicationRuntime)));
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    static void ProcessHitEventForParry(RE::Actor* target, RE::Actor* aggressor)
    {
        auto settings = Settings::GetSingleton();
        if (Conditions::PlayerHasActiveMagicEffect(settings->MAG_ParryWindowEffect)) {
            Conditions::ApplySpell(target, aggressor, settings->MAGParryStaggerSpell);
            Conditions::ApplySpell(aggressor, target, settings->APOParryBuffSPell);
            target->PlaceObjectAtMe(settings->APOSparksFlash, false);
        }
    }

    static void ProcessHitEventForParryShield(RE::Actor* target, RE::Actor* aggressor)
    {
        auto settings = Settings::GetSingleton();
        if (Conditions::PlayerHasActiveMagicEffect(settings->MAG_ParryWindowEffect)) {
            Conditions::ApplySpell(target, aggressor, settings->MAGParryStaggerSpell);
            Conditions::ApplySpell(aggressor, target, settings->APOParryBuffSPell);
            target->PlaceObjectAtMe(settings->APOSparksShieldFlash, false);
        }
    }

    inline static void ApplyHandToHandXP()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        float HandToHandLevel = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kLockpicking);

        float baseXP = (Settings::BonusXPPerLevel * HandToHandLevel) + Settings::BaseXP;

        player->AddSkillExperience(RE::ActorValue::kLockpicking, baseXP);
    }

    static bool IsBeastRace()
    {
        RE::MenuControls* MenuControls = RE::MenuControls::GetSingleton();
        return MenuControls->InBeastForm();
    }

    bool ShouldSkipHitEvent(RE::Actor* causeActor, RE::Actor* targetActor, std::uint32_t runTime)
    {
        bool skipEvent = false;

        auto matchedHits = recentGeneralHits.equal_range(runTime);
        for (auto it = matchedHits.first; it != matchedHits.second; ++it) {
            if (it->second.cause == causeActor && it->second.target == targetActor) {
                skipEvent = true;
                break;
            }
        }

        auto upper = recentGeneralHits.lower_bound(runTime);
        auto it    = recentGeneralHits.begin();
        while (it != upper) {
            it = recentGeneralHits.erase(it);
        }

        return skipEvent;
    }

    static void Register()
    {
        RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventHolder->AddEventSink(OnHitEventHandler::GetSingleton());
    }
};

class AnimationGraphEventHandler : public RE::BSTEventSink<RE::BSAnimationGraphEvent>,
                                   public RE::BSTEventSink<RE::TESObjectLoadedEvent>,
                                   public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>

{
public:
    static AnimationGraphEventHandler* GetSingleton()
    {
        static AnimationGraphEventHandler singleton;
        return &singleton;
    }

    inline static void StaminaCost(RE::Actor* actor, double cost)
    {
        logger::info("stamina for attacks is {}", cost);
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
        if (actor == player && !player->IsGodMode()) {
            actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, cost * -1.0);
        }                
    }

    const char* jumpAnimEventString = "JumpUp";
    const char* HitString        = "HitFrame";

    // Anims
    RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!a_event->tag.empty() && a_event->holder && a_event->holder->As<RE::Actor>()) {
            if (std::strcmp(a_event->tag.c_str(), jumpAnimEventString) == 0) {
                HandleJumpAnim();
            }
        }

        if (!a_event->tag.empty() && a_event->holder && a_event->holder->As<RE::Actor>()) {
            if (std::strcmp(a_event->tag.c_str(), HitString) == 0) {
                if (a_event->holder->As<RE::Actor>()) {
                    auto                 actor       = const_cast<RE::TESObjectREFR*>(a_event->holder)->As<RE::Actor>();
                    RE::PlayerCharacter*   player    = Cache::GetPlayerSingleton();
                    auto            wieldedWeap = Conditions::getWieldingWeapon(actor);
                    const Settings*      settings = Settings::GetSingleton();
                    RE::TESGlobal*         stamGlob    = settings->StaminaCostGlobal;
                    auto                   global      = stamGlob->value;
                    
                    double                stam_cost = 10.0;
                    if (actor == player) {
                        if (wieldedWeap && wieldedWeap->IsWeapon()) {
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
                    else
                        stam_cost = 10.0;
                                       
                    if (!Conditions::IsPowerAttacking(actor)) {
                        logger::info("{} is attacking and it costs {} stamina", actor->GetName(), stam_cost);
                        StaminaCost(actor, stam_cost);
                    }                    
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    void HandleJumpAnim()
    {
        auto settings = Settings::GetSingleton();
        auto player   = RE::PlayerCharacter::GetSingleton();
        if (!player->IsGodMode()) {
            Conditions::ApplySpell(player, player, settings->jumpSpell);
        }
    }

    // Object load
    RE::BSEventNotifyControl ProcessEvent(const RE::TESObjectLoadedEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_eventSource) override
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto actor = RE::TESForm::LookupByID<RE::Actor>(a_event->formID);
        if (!actor || !actor->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        // Register for anim event
        actor->AddAnimationGraphEventSink(AnimationGraphEventHandler::GetSingleton());

        return RE::BSEventNotifyControl::kContinue;
    }

    // Race Switch
    RE::BSEventNotifyControl ProcessEvent(const RE::TESSwitchRaceCompleteEvent*                                a_event,
                                          [[maybe_unused]] RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>* a_eventSource) override
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto actor = a_event->subject->As<RE::Actor>();
        if (!actor || !actor->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        // Register for anim event
        actor->AddAnimationGraphEventSink(AnimationGraphEventHandler::GetSingleton());

        return RE::BSEventNotifyControl::kContinue;
    }

    static void Register()
    {
        // Register for load event, then in the load event register for anims
        RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventHolder->AddEventSink<RE::TESObjectLoadedEvent>(GetSingleton());
        eventHolder->AddEventSink<RE::TESSwitchRaceCompleteEvent>(GetSingleton());
    }
};

class WeaponFireHandler
{
public:
    static void InstallArrowReleaseHook()
    {
        logger::info("Writing arrow release handler hook");

        auto& trampoline = SKSE::GetTrampoline();
        _Weapon_Fire     = trampoline.write_call<5>(Hooks::arrow_release_handler.address(), WeaponFire);

        logger::info("Release arrow hooked");
    }

    static void WeaponFire(RE::TESObjectWEAP* a_weapon, RE::TESObjectREFR* a_source, RE::TESAmmo* a_ammo, RE::EnchantmentItem* a_ammoEnchantment, RE::AlchemyItem* a_poison)
    {
        _Weapon_Fire(a_weapon, a_source, a_ammo, a_ammoEnchantment, a_poison);

        if (!a_source) {
            return;
        }

        auto source = a_source->As<RE::Actor>();

        if (source->IsPlayerRef() && a_weapon->IsCrossbow()) {
            Conditions::ApplySpell(source, source, Settings::GetSingleton()->MAGCrossbowStaminaDrainSpell);
        }
    }

    inline static REL::Relocation<decltype(WeaponFire)> _Weapon_Fire;
};
