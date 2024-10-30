#include "Settings.h"
#include "Cache.h"
#include "Conditions.h"
#include <SimpleIni.h>
#include <sstream>

Settings* Settings::GetSingleton()
{
    static Settings settings;
    return &settings;
}

void Settings::LoadSettings()
{
    logger::info("loading settings...");
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\ValorPerks.ini)");

    enableSneakStaminaCost = ini.GetBoolValue("", "bEnableSneakStaminaCost", true);
    zeroAllWeapStagger     = ini.GetBoolValue("", "bZeroAllWeaponStagger", true);
    armorScalingEnabled    = ini.GetBoolValue("", "bArmorRatingScalingEnabled", true);
    debug_logging          = ini.GetBoolValue("", "Debug");

    surroundingActorsRange = (float)ini.GetDoubleValue("", "fRangeActors", 16.0);
    auto bonusXP           = (float)ini.GetDoubleValue("", "fBonusXPPerLevel", 0.15);
    auto baseXP            = (float)ini.GetDoubleValue("", "fBaseXPHerHit", 3.0);

    (bonusXP < 0.0 || bonusXP > 100.0) ? BonusXPPerLevel = 0.15f : BonusXPPerLevel = bonusXP;
    baseXP < 0.0 ? BaseXP = 3.0f : BaseXP = baseXP;

    FileName = "ValorPerks.esp";

    if (debug_logging) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    };
    logger::info("... finished");
}

RE::FormID Settings::ParseFormID(const std::string& str)
{
    RE::FormID         result;
    std::istringstream ss{ str };
    ss >> std::hex >> result;
    return result;
}

void Settings::AdjustWeaponStaggerVals()
{
    if (zeroAllWeapStagger) {
        logger::info("Adjusting weapon stagger values");
        int16_t totalWeaps = 0;

        auto dataHandler = RE::TESDataHandler::GetSingleton();
        if (dataHandler) {
            for (const auto& foundWeap : dataHandler->GetFormArray<RE::TESObjectWEAP>()) {
                if (foundWeap && !foundWeap->weaponData.flags.any(RE::TESObjectWEAP::Data::Flag::kNonPlayable)) {
                    foundWeap->weaponData.staggerValue = 0.0f;
                    totalWeaps++;
                }
            }
        }

        logger::info(FMT_STRING("Stagger values adjusted: {} weapons"), totalWeaps);
    }
}

void Settings::GetIngameData() // hard coded FormIDs to keep the ini file simpler for users
{
    /*CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\ValorFormLookup.ini)");*/


    //forms for ini:
    const int stam_cost_global = 0x0EDA69;
    const int npc_stam_cost = 0x0EDA6A;
    const int dual_block_key = 0x10C0DB;
    const int bashStamPerk = 0xADA510;
    const int blockStamPerk = 0xADA509;
    const int DodgePerk = 0x111211;
    const int ParryWindowEffect = 0x000D9E;
    const int isBlockSpell = 0x000DAC;
    const int power_attack_stop= 0x0E8932;
    const int jump_spell = 0x0E892F;
    const int isAttackSpel = 0x000DA9;
    const int isSneakSpel= 0x000DB4;
    const int sprint_spel = 0x000DB6;
    const int mount_sprint = 0x000DB1;
    const int bow_stam_spel = 0x000DAE;
    const int x_bow_stam_spel = 0x000DB0;
    const int casting_spel = 0x000DB5;
    const int parry_control_spel = 0x000DB2;
    const int parry_stagger_spel = 0x000DB3;
    const int parry_buff_spel= 0x23040;
    const int crossbow_stam_drain = 0x000DAF ;
    const int dodge_spell = 0x10C0EA;
    const int shield_sparks= 0x18E3E;
    const int weapon_sparks= 0x18E3D;
    const int physic_sparks= 0x18E3C;
    const int normal_sparks= 0x18E3B;

    auto dataHandler = RE::TESDataHandler::GetSingleton();
    // Globals:
    StaminaCostGlobal    = dataHandler->LookupForm(0x0EDA69, FileName)->As<RE::TESGlobal>();
    NPCStaminaCostGlobal = dataHandler->LookupForm(0x0EDA6A, FileName)->As<RE::TESGlobal>();
    DualBlockKey         = dataHandler->LookupForm(0x10C0DB, FileName)->As<RE::TESGlobal>();
    // Perks:
    BashStaminaPerk  = dataHandler->LookupForm(0xADA510, "Update.esm")->As<RE::BGSPerk>();
    BlockStaminaPerk = dataHandler->LookupForm(0xADA509, "Update.esm")->As<RE::BGSPerk>();
    //dummyPerkDodge = dataHandler->LookupForm(0x111211, FileName)->As<RE::BGSPerk>(); 
    dummyPerkDodge = dataHandler->LookupForm(0x58f64, "Skyrim.esm")->As<RE::BGSPerk>();
    // Effects:
    MAG_ParryWindowEffect = dataHandler->LookupForm(0x000D9E, FileName)->As<RE::EffectSetting>();
    // Spells:
    IsBlockingSpell              = dataHandler->LookupForm(0x000DAC, FileName)->As<RE::SpellItem>();
    PowerAttackStopSpell         = dataHandler->LookupForm(0x0E8932, FileName)->As<RE::SpellItem>();
    jumpSpell                    = dataHandler->LookupForm(0x0E892F, FileName)->As<RE::SpellItem>();
    IsAttackingSpell             = dataHandler->LookupForm(0x000DA9, FileName)->As<RE::SpellItem>();
    IsSneakingSpell              = dataHandler->LookupForm(0x000DB4, FileName)->As<RE::SpellItem>();
    IsSprintingSpell             = dataHandler->LookupForm(0x000DB6, FileName)->As<RE::SpellItem>();
    MountSprintingSpell          = dataHandler->LookupForm(0x000DB1, FileName)->As<RE::SpellItem>();
    BowStaminaSpell              = dataHandler->LookupForm(0x000DAE, FileName)->As<RE::SpellItem>();
    XbowStaminaSpell             = dataHandler->LookupForm(0x000DB0, FileName)->As<RE::SpellItem>();
    IsCastingSpell               = dataHandler->LookupForm(0x000DB5, FileName)->As<RE::SpellItem>();
    MAGParryControllerSpell      = dataHandler->LookupForm(0x000DB2, FileName)->As<RE::SpellItem>();
    MAGParryStaggerSpell         = dataHandler->LookupForm(0x000DB3, FileName)->As<RE::SpellItem>();
    APOParryBuffSPell            = dataHandler->LookupForm(0x23040, FileName)->As<RE::SpellItem>();
    MAGCrossbowStaminaDrainSpell = dataHandler->LookupForm(0x000DAF, FileName)->As<RE::SpellItem>();
    DodgeRuneSpell = dataHandler->LookupForm(0x10C0EA, FileName)->As<RE::SpellItem>();
    // Explosions:
    APOSparksShieldFlash = dataHandler->LookupForm(0x18E3E, FileName)->As<RE::BGSExplosion>();
    APOSparksFlash       = dataHandler->LookupForm(0x18E3D, FileName)->As<RE::BGSExplosion>();
    APOSparksPhysics     = dataHandler->LookupForm(0x18E3C, FileName)->As<RE::BGSExplosion>();
    APOSparks            = dataHandler->LookupForm(0x18E3B, FileName)->As<RE::BGSExplosion>();
    fireBolt = dataHandler->LookupForm(0x2dd29, "Skyrim.esm")->As<RE::SpellItem>();

    logger::debug("ingame forms loaded");
}



void Settings::LoadForms()
{
    auto dataHandler = RE::TESDataHandler::GetSingleton();
    auto file        = dataHandler->LookupLoadedModByName(FileName);

    if (!file || file->compileIndex == 0xFF) {
        SKSE::stl::report_and_fail("Cannot find ValorPerks.esp."sv);
    }
    logger::info("Loading forms");

    GetIngameData();
    SetGlobalsAndGameSettings();

    auto gameSettings = RE::GameSettingCollection::GetSingleton();
    blockAngleSetting = gameSettings->GetSetting("fCombatHitConeAngle")->GetFloat();

    dualBlockKey = DualBlockKey->value;

    logger::info("All Forms loaded");
}

void Settings::SetGlobalsAndGameSettings()
{
    // Set fMaxArmorRating game setting
    auto gameSettings     = RE::GameSettingCollection::GetSingleton();    
    auto maxRatingSetting = gameSettings->GetSetting("fMaxArmorRating");

    if (armorScalingEnabled) {
        logger::info("Setting max armor rating from {} to 90", maxRatingSetting->data.f);
        
        maxRatingSetting->data.f = 90.0f;
        logger::debug("donezo");
        return;
    }
    else {
        logger::info("Setting max armor rating to 75");
        maxRatingSetting->data.f = 75.0f;
        return;
    }
}
