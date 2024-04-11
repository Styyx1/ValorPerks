#include "Settings.h"
#include <SimpleIni.h>
#include <sstream>
#include "Cache.h"
#include "Conditions.h"

Settings* Settings::GetSingleton()
{
	static Settings settings;
	return &settings;
}

void Settings::LoadSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(R"(.\Data\SKSE\Plugins\ValorPerks.ini)");

	enableSneakStaminaCost = ini.GetBoolValue("", "bEnableSneakStaminaCost", true);
	enableLevelDifficulty = ini.GetBoolValue("", "bLevelBasedDifficulty", true);
	zeroAllWeapStagger = ini.GetBoolValue("", "bZeroAllWeaponStagger", true);
	armorScalingEnabled = ini.GetBoolValue("", "bArmorRatingScalingEnabled", true);

	std::string attackingSpellFormID((ini.GetValue("", "IsAttackingSpellFormId", "")));
	std::string blockingSpellFormID((ini.GetValue("", "IsBlockingSpellFormId", "")));
	std::string sneakingSpellFormID((ini.GetValue("", "IsSneakingSpellFormId", "")));
	std::string sprintingSpellFormId((ini.GetValue("", "IsSprintingSpellFormId", "")));
	std::string mountSprintingSpellFormId((ini.GetValue("", "MountSprintingSpellFormId", "")));
	std::string bowStaminaSpellFormID((ini.GetValue("", "BowStaminaSpellFormId", "")));
	std::string xbowStaminaSpellFormID((ini.GetValue("", "XbowStaminaSpellFormId", "")));
	std::string IsCastingFormId((ini.GetValue("", "IsCastingSpellFormId", "")));
	std::string bashPerkFormId((ini.GetValue("", "BashStaminaPerkFormId", "")));
	std::string blockPerkFormId((ini.GetValue("", "BlockStaminaPerkFormId", "")));
	std::string blockStaggerPerkFormId((ini.GetValue("", "BlockStaggerPerkFormId", "")));

    //new

    std::string parryControllerSpellID((ini.GetValue("", "MAGParryControllerSpellID", "")));
    std::string parryStaggerSpellID((ini.GetValue("", "MAGParryStaggerSpellFormID", "")));
    std::string BlockStaggerSpellID1((ini.GetValue("", "MAGBlockStaggerSpellFormID", "")));
    std::string BlockStaggerSpellID2((ini.GetValue("", "MAGBlockStaggerSpell2FormID", "")));
    std::string CrossbowStaminaDrainSpellID((ini.GetValue("", "MAGCrossbowStaminaDrainSpellFormID", "")));
    std::string parryWindowEffectID((ini.GetValue("", "MAG_ParryWindowEffectFormID", "")));
    std::string levelBasedDifficultyID((ini.GetValue("", "MAG_levelBasedDifficultyFormID", "")));
    std::string apoParryBuffSpellID((ini.GetValue("", "ParryBuffSpellFormID", "")));

	std::string fileName(ini.GetValue("", "sModFileName", ""));



	if(!attackingSpellFormID.empty()){
		IsAttackingSpellFormId = ParseFormID(attackingSpellFormID);
	}

	if(!blockingSpellFormID.empty()){
		IsBlockingSpellFormId = ParseFormID(blockingSpellFormID);
	}

	if(!sneakingSpellFormID.empty()){
		IsSneakingSpellFormId = ParseFormID(sneakingSpellFormID);
	}

	if (!sprintingSpellFormId.empty()) {
		IsSprintingSpellFormId = ParseFormID(sprintingSpellFormId);
	}

	if (!mountSprintingSpellFormId.empty()) {
		MountSprintingSpellFormId = ParseFormID(mountSprintingSpellFormId);
	}

	if (!bowStaminaSpellFormID.empty()){
		BowDrainStaminaFormId = ParseFormID(bowStaminaSpellFormID);
	}

	if (!xbowStaminaSpellFormID.empty()) {
		XbowDrainStaminaFormId = ParseFormID(xbowStaminaSpellFormID);
	}

	if (!IsCastingFormId.empty()) {
		IsCastingSpellFormId = ParseFormID(IsCastingFormId);
	}

	if (!bashPerkFormId.empty()) {
		BashPerkFormId = ParseFormID(bashPerkFormId);
	}

	if (!blockPerkFormId.empty()) {
		BlockPerkFormId = ParseFormID(blockPerkFormId);
	}

	if (!blockStaggerPerkFormId.empty()) {
		BlockStaggerPerkFormId = ParseFormID(blockStaggerPerkFormId);
	}
    // new

    if (!parryControllerSpellID.empty()) {
        MAGParryControllerSpellID = ParseFormID(parryControllerSpellID);
    }
    if (!parryStaggerSpellID.empty()) {
        MAGParryStaggerSpellFormID = ParseFormID(parryStaggerSpellID);
    }
    if (!BlockStaggerSpellID1.empty()) {
        MAGBlockStaggerSpellFormID = ParseFormID(BlockStaggerSpellID1);
    }
    if (!BlockStaggerSpellID2.empty()) {
        MAGBlockStaggerSpell2FormID = ParseFormID(BlockStaggerSpellID2);
    }
    if (!CrossbowStaminaDrainSpellID.empty()) {
        MAGCrossbowStaminaDrainSpellFormID = ParseFormID(CrossbowStaminaDrainSpellID);
    }
    if (!parryWindowEffectID.empty()) {
        MAG_ParryWindowEffectFormID = ParseFormID(parryWindowEffectID);
    }

    if (!apoParryBuffSpellID.empty()) {
        APOParryBuffSpellFormID = ParseFormID(apoParryBuffSpellID);
    }

	FileName = fileName;
}


RE::FormID Settings::ParseFormID(const std::string& str)
{
	RE::FormID result;
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

void Settings::LoadForms()
{
	auto dataHandler = RE::TESDataHandler::GetSingleton();

	 auto file = LookupLoadedModByName(FileName);

	if (!file || file->compileIndex == 0xFF) {

		SKSE::stl::report_and_fail("Cannot find ValorPerks.esp. If you are on Skyrim 1.6.1130+, Engine Fixes' achievements enabler may be disabling all of your plugins."sv);
	}
    

	logger::info("Loading forms");
	if (IsBlockingSpellFormId)
		IsBlockingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsBlockingSpellFormId, FileName));

	if(IsAttackingSpellFormId)
		IsAttackingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsAttackingSpellFormId, FileName));

	if(IsSneakingSpellFormId)
		IsSneakingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsSneakingSpellFormId, FileName));

	if (IsSprintingSpellFormId)
		IsSprintingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsSprintingSpellFormId, FileName));

	if (MountSprintingSpellFormId)
		MountSprintingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(MountSprintingSpellFormId, FileName));

	if(BowDrainStaminaFormId)
		BowStaminaSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(BowDrainStaminaFormId, FileName));

	if (XbowDrainStaminaFormId)
		XbowStaminaSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(XbowDrainStaminaFormId, FileName));

	if (IsCastingSpellFormId)
		IsCastingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsCastingSpellFormId, FileName));

	if (BashPerkFormId)
		BashStaminaPerk = dataHandler->LookupForm(BashPerkFormId, "Update.esm")->As<RE::BGSPerk>();

	if (BlockPerkFormId)
		BlockStaminaPerk = dataHandler->LookupForm(BlockPerkFormId, "Update.esm")->As<RE::BGSPerk>();

	if (BlockStaggerPerkFormId)
		BlockStaggerPerk = dataHandler->LookupForm(BlockStaggerPerkFormId, "Update.esm")->As<RE::BGSPerk>();

    if (MAGParryControllerSpellID)
        MAGParryControllerSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(MAGParryControllerSpellID, FileName));

    if (MAGParryStaggerSpellFormID)
        MAGParryStaggerSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(MAGParryStaggerSpellFormID, FileName));

    if (MAGBlockStaggerSpellFormID)
        MAGBlockStaggerSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(MAGBlockStaggerSpellFormID, FileName));

    if (MAGBlockStaggerSpell2FormID)
        MAGBlockStaggerSpell2 = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(MAGBlockStaggerSpell2FormID, FileName));

    if (MAGCrossbowStaminaDrainSpellFormID)
        MAGCrossbowStaminaDrainSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(MAGCrossbowStaminaDrainSpellFormID, FileName));

    if (MAG_ParryWindowEffectFormID)
        MAG_ParryWindowEffect = skyrim_cast<RE::EffectSetting*>(dataHandler->LookupForm(MAG_ParryWindowEffectFormID, FileName));

    if (APOParryBuffSpellFormID)
        APOParryBuffSPell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(APOParryBuffSpellFormID, FileName));
 
    // Hardcoded loads
    MAG_levelBasedDifficulty = dataHandler->LookupForm(RE::FormID(ParseFormID("0xD91")), FileName)->As<RE::TESGlobal>();


	SetGlobalsAndGameSettings();

	auto isPowerAttacking = new RE::TESConditionItem;
	isPowerAttacking->data.comparisonValue.f = 1.0f;
	isPowerAttacking->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPowerAttacking;

	auto isPowerAttackingCond = new RE::TESCondition;
	isPowerAttackingCond->head = isPowerAttacking;
	IsPowerAttacking = isPowerAttackingCond;

	logger::info("All Forms loaded");

}

void Settings::SetGlobalsAndGameSettings() 
{
	MAG_levelBasedDifficulty->value = enableLevelDifficulty;


	//Set fMaxArmorRating game setting
	auto gameSettings = RE::GameSettingCollection::GetSingleton();
	auto maxRatingSetting = gameSettings->GetSetting("fMaxArmorRating");

	if (armorScalingEnabled) {
		logger::info("Setting max armor rating to 90");
		maxRatingSetting->data.f = 90.0f;
	}
    else {
		logger::info("Setting max armor rating to 75");
		maxRatingSetting->data.f = 75.0f;
	}
}

