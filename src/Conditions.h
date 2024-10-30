#pragma once
#include "Cache.h"
#include "Settings.h"
#include <numbers>

// Originally intended to just implement some condition functions but iv been placing extensions/utility here as well
namespace Conditions
{
    inline static int GetRandomINT(int a_min, int a_max)
    {
        static std::random_device       rd;
        static std::mt19937             gen(rd());
        std::uniform_int_distribution<int> distrib(a_min, a_max);
        return distrib(gen);
    }

    inline static double GetRandomDouble(double a_min, double a_max)
    {
        static std::random_device       rd;
        static std::mt19937             gen(rd());
        std::uniform_real_distribution<> distrib(a_min, a_max);
        return distrib(gen);
    }

    inline static float GetPointInRange(double radius, float start_point) 
    {
        auto r = radius * std::sqrt(GetRandomDouble(0.0, 1.0));
        auto theta = GetRandomDouble(0.0, 1.0) * 2 * std::numbers::pi_v<float>;
        return start_point + r * std::cos(theta);
    }

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

    inline static bool resetBool(bool a_bool) {
        dlog("bool was set from {} to {}", a_bool ? "true" : "false", !a_bool ? "true" : "false");
        return !a_bool;
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

    inline static std::vector<RE::Actor*> GetNearbyActors(RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
    {
        {
            std::vector<RE::Actor*> result;
            if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
                if (a_ignorePlayer && processLists->numberHighActors == 0) {
                    dlog("no process list");
                    return result;
                }

                const auto squaredRadius = a_radius * a_radius;
                const auto originPos     = a_ref->GetPosition();

                result.reserve(processLists->numberHighActors);

                const auto get_actor_within_radius = [&](RE::Actor* a_actor) {
                    if (a_actor && a_actor != a_ref && originPos.GetSquaredDistance(a_actor->GetPosition()) <= squaredRadius) {
                        result.emplace_back(a_actor);
                    }
                };
                for (auto& actorHandle : processLists->highActorHandles) {
                    const auto actor = actorHandle.get();
                    get_actor_within_radius(actor.get());
                }

                if (!a_ignorePlayer) {
                    get_actor_within_radius(Cache::GetPlayerSingleton());
                }

                if (!result.empty()) {
                    dlog("vector is not empty");
                    return result;
                }
            }
            return result;
        }
    }

    inline static std::int32_t NumNearbyActors(RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
    {
        {
            std::int32_t num_result = 0;
            
            if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
                if (a_ignorePlayer && processLists->numberHighActors == 0) {
                    dlog("no process list");
                    return num_result;
                }

                const auto squaredRadius = a_radius * a_radius;
                const auto originPos     = a_ref->GetPosition();

                std::vector<RE::Actor*> result;
                result.reserve(processLists->numberHighActors);

                const auto get_actor_within_radius = [&](RE::Actor* a_actor) {
                    if (a_actor && a_actor != a_ref && originPos.GetSquaredDistance(a_actor->GetPosition()) <= squaredRadius) {
                        result.emplace_back(a_actor);
                    }
                    };
                for (auto& actorHandle : processLists->highActorHandles) {
                    const auto actor = actorHandle.get();
                    get_actor_within_radius(actor.get());
                }

                if (!a_ignorePlayer) {
                    get_actor_within_radius(Cache::GetPlayerSingleton());
                }

                if (!result.empty()) {
                    for (const auto& enemy : result) {
                        if (enemy) {
                            num_result++;
                        }
                    }
                    return num_result;
                }
            }
            return num_result;
        }
    }

    // credits: https://github.com/Sacralletius/ANDR_SKSEFunctions currently unused though
    struct ProjectileRot
    {
        float x, z;
    };

    inline float SkyrimSE_c51f70(RE::NiPoint3* dir)
    {
        using func_t = decltype(SkyrimSE_c51f70);
        REL::Relocation<func_t> func{ REL::RelocationID(68820, 70172) };
        return func(dir);
    }

    inline ProjectileRot rot_at(RE::NiPoint3 dir)
    {
        ProjectileRot rot;
        auto          len = dir.Unitize();
        if (len == 0) {
            rot = { 0, 0 };
        }
        else {
            float polar_angle = SkyrimSE_c51f70(&dir);
            rot               = { -asin(dir.z), polar_angle };
        }
        return rot;
    }

    inline ProjectileRot rot_at(const RE::NiPoint3& from, const RE::NiPoint3& to)
    {
        return rot_at(to - from);
    }

    inline static void ArrowRain(RE::Actor* a_shooterForLevel, RE::Actor* a_Source, RE::TESAmmo* a_arrow, RE::Actor* start_actor, RE::Actor* target, int range, float extra_height, RE::AlchemyItem* a_poison)
    {
        

        RE::NiPoint3 StartPos;
        StartPos.x = start_actor->GetPositionX();
        StartPos.y = start_actor->GetPositionY();
        StartPos.z = start_actor->GetPositionZ() + extra_height;

        RE::NiPoint3 EndPos;

        EndPos.x = GetPointInRange(range, target->GetPositionX());
        EndPos.y = GetPointInRange(range, target->GetPositionY());
        EndPos.z = target->GetPositionZ();
        //logger::debug("end position is: \ X:{} \n Y:{} \n Z:{}", EndPos.x, EndPos.y, EndPos.z);

        auto rot = rot_at(StartPos, EndPos);

        auto arrows = a_arrow;
        RE::Projectile::LaunchData ldata;
        ldata.origin                = StartPos;
        ldata.contactNormal         = { 0.0f, 0.0f, 0.0f };
        ldata.projectileBase        = a_arrow->GetRuntimeData().data.projectile;
        ldata.shooter               = a_Source;
        ldata.combatController      = a_Source->GetActorRuntimeData().combatController;
        ldata.weaponSource          = getWieldingWeapon(a_Source);
        ldata.ammoSource            = a_arrow;
        ldata.angleZ                = rot.z;
        ldata.angleX                = rot.x;
        ldata.unk50                 = nullptr;
        ldata.desiredTarget         = target;
        ldata.unk60                 = 0.0f;
        ldata.unk64                 = 0.0f;
        ldata.parentCell            = a_Source->GetParentCell();
        ldata.spell                 = nullptr;
        ldata.castingSource         = RE::MagicSystem::CastingSource::kOther;
        ldata.pad7C                 = 0;
        ldata.enchantItem           = nullptr;
        ldata.poison                = a_poison;
        ldata.area                  = 0.0f;
        ldata.power                 = 1.0f;
        ldata.scale                 = 1.0f;
        ldata.alwaysHit             = false;
        ldata.noDamageOutsideCombat = false;
        ldata.autoAim               = false;
        ldata.chainShatter          = false;
        ldata.useOrigin             = true;
        ldata.deferInitialization   = false;
        ldata.forceConeOfFire       = false;
        RE::BSPointerHandle<RE::Projectile> handle;
        RE::Projectile::Launch(&handle, ldata);
            
        
    }

    inline void LaunchFireMeteores(RE::Actor* a_actor, RE::SpellItem* a_spell, RE::TESObjectREFR* a_target, float a_area)
    {
        SKSE::GetTaskInterface()->AddTask([a_actor, a_spell, a_target, a_area]() {

            RE::NiPoint3 NodePosition;

            NodePosition.x = a_target->GetPositionX();
            NodePosition.y = a_target->GetPositionY();
            NodePosition.z = a_target->GetPositionZ() + 600;

            logger::debug("NodePosition: X = {}, Y = {}, Z = {}.", NodePosition.x, NodePosition.y, NodePosition.z);

            RE::NiPoint3 DestinationPosition;

            DestinationPosition.x = a_target->GetPositionX() + GetRandomINT(0, a_area);
            DestinationPosition.y = a_target->GetPositionY() + GetRandomINT(0, a_area);
            DestinationPosition.z = a_target->GetPositionZ();

            logger::debug("DestinationPosition: X = {}, Y = {}, Z = {}.", DestinationPosition.x, DestinationPosition.y, DestinationPosition.z);

            auto rot = rot_at(NodePosition, DestinationPosition);

            auto eff = a_spell->GetCostliestEffectItem();

            auto mgef = a_spell->GetAVEffect();

            RE::Projectile::LaunchData ldata;

            ldata.origin = NodePosition;
            ldata.contactNormal = { 0.0f, 0.0f, 0.0f };
            ldata.projectileBase = mgef->data.projectileBase;
            ldata.shooter = a_actor;
            ldata.combatController = a_actor->GetActorRuntimeData().combatController;
            ldata.weaponSource = nullptr;
            ldata.ammoSource = nullptr;
            ldata.angleZ = rot.z;
            ldata.angleX = rot.x;
            ldata.unk50 = nullptr;
            ldata.desiredTarget = nullptr;
            ldata.unk60 = 0.0f;
            ldata.unk64 = 0.0f;
            ldata.parentCell = a_actor->GetParentCell();
            ldata.spell = a_spell;
            ldata.castingSource = RE::MagicSystem::CastingSource::kOther;
            ldata.pad7C = 0;
            ldata.enchantItem = nullptr;
            ldata.poison = nullptr;
            ldata.area = eff->GetArea();
            ldata.power = 1.0f;
            ldata.scale = 1.0f;
            ldata.alwaysHit = false;
            ldata.noDamageOutsideCombat = false;
            ldata.autoAim = false;
            ldata.chainShatter = false;
            ldata.useOrigin = true;
            ldata.deferInitialization = false;
            ldata.forceConeOfFire = false;
            RE::BSPointerHandle<RE::Projectile> handle;
            RE::Projectile::Launch(&handle, ldata);


            });
            
    }

    inline static void CastSpellFromPointToPoint(RE::Actor* akSource, RE::SpellItem* akSpell, float StartPoint_X, float StartPoint_Y, float StartPoint_Z, float EndPoint_X,
                                                 float EndPoint_Y, float EndPoint_Z)
    {
        RE::NiPoint3 NodePosition;

        NodePosition.x = StartPoint_X;
        NodePosition.y = StartPoint_Y;
        NodePosition.z = StartPoint_Z;

        logger::debug("NodePosition: X = {}, Y = {}, Z = {}.", NodePosition.x, NodePosition.y, NodePosition.z);

        RE::NiPoint3 DestinationPosition;

        DestinationPosition.x = EndPoint_X;
        DestinationPosition.y = EndPoint_Y;
        DestinationPosition.z = EndPoint_Z;

        logger::debug("DestinationPosition: X = {}, Y = {}, Z = {}.", DestinationPosition.x, DestinationPosition.y, DestinationPosition.z);

        auto rot = rot_at(NodePosition, DestinationPosition);

        auto eff = akSpell->GetCostliestEffectItem();

        auto mgef = akSpell->GetAVEffect();

        RE::Projectile::LaunchData ldata;

        ldata.origin                = NodePosition;
        ldata.contactNormal         = { 0.0f, 0.0f, 0.0f };
        ldata.projectileBase        = mgef->data.projectileBase;
        ldata.shooter               = akSource;
        ldata.combatController      = akSource->GetActorRuntimeData().combatController;
        ldata.weaponSource          = nullptr;
        ldata.ammoSource            = nullptr;
        ldata.angleZ                = rot.z;
        ldata.angleX                = rot.x;
        ldata.unk50                 = nullptr;
        ldata.desiredTarget         = nullptr;
        ldata.unk60                 = 0.0f;
        ldata.unk64                 = 0.0f;
        ldata.parentCell            = akSource->GetParentCell();
        ldata.spell                 = akSpell;
        ldata.castingSource         = RE::MagicSystem::CastingSource::kOther;
        ldata.pad7C                 = 0;
        ldata.enchantItem           = nullptr;
        ldata.poison                = nullptr;
        ldata.area                  = eff->GetArea();
        ldata.power                 = 1.0f;
        ldata.scale                 = 1.0f;
        ldata.alwaysHit             = false;
        ldata.noDamageOutsideCombat = false;
        ldata.autoAim               = false;
        ldata.chainShatter          = false;
        ldata.useOrigin             = true;
        ldata.deferInitialization   = false;
        ldata.forceConeOfFire       = false;
        RE::BSPointerHandle<RE::Projectile> handle;
        RE::Projectile::Launch(&handle, ldata);
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
