#pragma once
#include "Cache.h"
#include "Settings.h"

// Originally intended to just implement some condition functions but iv been placing extensions/utility here as well
namespace Conditions
{
    static bool IsAttacking(RE::Actor* actor)
    {
        using func_t = decltype(&Conditions::IsAttacking);
        REL::Relocation<func_t> func{ Cache::IsAttackingAddress };
        return func(actor);
    }

    inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

    static bool IsBlocking(RE::Actor* actor)
    {
        using func_t = decltype(&Conditions::IsBlocking);
        REL::Relocation<func_t> func{ Cache::IsBlockingAddress };
        return func(actor);
    }

    inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

    static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
    {
        using func_t = decltype(&Conditions::HasSpell);

        REL::Relocation<func_t> func{ Cache::HasSpellAddress };

        return func(actor, spell);
    }

    inline static REL::Relocation<decltype(HasSpell)> _HasSpell;

    inline static bool IsMoving(RE::PlayerCharacter* player)
    {
        auto playerState = player->AsActorState();
        return (static_cast<bool>(playerState->actorState1.movingForward) || static_cast<bool>(playerState->actorState1.movingBack)
                || static_cast<bool>(playerState->actorState1.movingLeft) || static_cast<bool>(playerState->actorState1.movingRight));
    }

    inline static RE::TESObjectWEAP* GetUnarmedWeapon()
    {
        auto** singleton{ reinterpret_cast<RE::TESObjectWEAP**>(Cache::getUnarmedWeaponAddress) };
        return *singleton;
    }

    inline static bool PlayerHasActiveMagicEffect(RE::EffectSetting* a_effect)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        auto               activeEffects = player->AsMagicTarget()->GetActiveEffectList();
        RE::EffectSetting* setting       = nullptr;
        for (auto& effect : *activeEffects) {
            setting = effect ? effect->GetBaseObject() : nullptr;
            if (setting) {
                if (setting == a_effect) {
                    return true;
                }
            }
        }
        return false;
    }

    inline static float GetMaxHealth()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        return player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, RE::ActorValue::kHealth)
               + player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kHealth);
    }

    inline static bool IsPowerAttacking(RE::Actor* actor)
    {
        if (auto high = actor->GetHighProcess()) {
            if (const auto attackData = high->attackData) {
                auto flags = attackData->data.flags;

                if (flags && flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
                    return true;
                }
            }
        }
        return false;
    }

    // Credit: D7ry for getWieldingWeapon in ValhallaCombat
    // https://github.com/D7ry/valhallaCombat/blob/48fb4c3b9bb6bbaa691ce41dbd33f096b74c07e3/src/include/Utils.cpp#L10
    inline static RE::TESObjectWEAP* getWieldingWeapon(RE::Actor* a_actor)
    {
        bool dual_wielding = false;
        auto weapon        = a_actor->GetAttackingWeapon();
        if (weapon) {
            dual_wielding = false;
            return weapon->object->As<RE::TESObjectWEAP>();
        }
        auto rhs = a_actor->GetEquippedObject(false);
        if (rhs && rhs->IsWeapon()) {
            dual_wielding = false;
            return rhs->As<RE::TESObjectWEAP>();
        }
        auto lhs = a_actor->GetEquippedObject(true);
        if (lhs && lhs->IsWeapon()) {
            dual_wielding = false;
            return lhs->As<RE::TESObjectWEAP>();
        }

        return nullptr;
    }

    inline static bool IsDualWielding(RE::Actor* a_actor)
    {
        auto weapon = a_actor->GetAttackingWeapon();
        auto rhs    = a_actor->GetEquippedObject(false);
        auto lhs    = a_actor->GetEquippedObject(true);
        if (weapon && rhs && lhs && lhs->IsWeapon() && rhs->IsWeapon()) {
            logger::debug("dual wielding is active");
            return true;
        }
        else
            return false;
    }

    static bool isInBlockAngle(RE::Actor* blocker, RE::TESObjectREFR* a_obj)
    {
        Settings* settings            = Settings::GetSingleton();
        float     fCombatHitConeAngle = settings->blockAngleSetting;

        logger::debug("block angle is {}", fCombatHitConeAngle);
        auto angle = blocker->GetHeadingAngle(a_obj->GetPosition(), false);
        logger::debug("actual angle is {}", angle);
        return (angle <= fCombatHitConeAngle && angle >= -fCombatHitConeAngle);
    }

    // Credit: KernalsEgg for ApplySpell and IsPermanent
    // extensions
    static bool IsPermanent(RE::MagicItem* item)
    {
        switch (item->GetSpellType()) {
        case RE::MagicSystem::SpellType::kDisease:
        case RE::MagicSystem::SpellType::kAbility:
        case RE::MagicSystem::SpellType::kAddiction: {
            return true;
        }
        default: {
            return false;
        }
        }
    }

    inline static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
    {
        if (IsPermanent(spell)) {
            target->AddSpell(spell);
        }
        else {
            caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
        }
    }
}; // namespace Conditions
