#pragma once

class Settings
{
public:
	static Settings* GetSingleton();

	void LoadSettings();
	void LoadForms();
	void AdjustWeaponStaggerVals();

	void SetGlobalsAndGameSettings();

	RE::FormID IsAttackingSpellFormId;
	RE::FormID IsBlockingSpellFormId;
	RE::FormID IsSneakingSpellFormId;
	RE::FormID IsSprintingSpellFormId;
	RE::FormID MountSprintingSpellFormId;
	RE::FormID IsCastingSpellFormId;
	RE::FormID BowDrainStaminaFormId;
	RE::FormID XbowDrainStaminaFormId;
	RE::FormID BashPerkFormId;
	RE::FormID BlockPerkFormId;
	RE::FormID BlockStaggerPerkFormId;
	RE::FormID DualWieldReplaceFormId;

    // new
    RE::FormID MAGParryControllerSpellID;
    RE::FormID MAGParryStaggerSpellFormID;
    RE::FormID MAGBlockStaggerSpellFormID;
    RE::FormID MAGBlockStaggerSpell2FormID;
    RE::FormID MAGCrossbowStaminaDrainSpellFormID;
    RE::FormID MAG_ParryWindowEffectFormID;

	RE::SpellItem* IsAttackingSpell;
	RE::SpellItem* IsBlockingSpell;
	RE::SpellItem* IsSneakingSpell;
	RE::SpellItem* IsSprintingSpell;
	RE::SpellItem* MountSprintingSpell;
	RE::SpellItem* BowStaminaSpell;
	RE::SpellItem* XbowStaminaSpell;
	RE::SpellItem* IsCastingSpell;
	RE::SpellItem* MAGParryStaggerSpell;
	RE::SpellItem* MAGBlockStaggerSpell;
	RE::SpellItem* MAGBlockStaggerSpell2;
	RE::SpellItem* MAGParryControllerSpell;
	RE::SpellItem* MAGCrossbowStaminaDrainSpell;
	
	RE::BGSPerk* BashStaminaPerk;
	RE::BGSPerk* BlockStaminaPerk;
	RE::BGSPerk* BlockStaggerPerk;	

	RE::TESGlobal* MAG_levelBasedDifficulty;
	
	RE::EffectSetting* MAG_ParryWindowEffect;

	RE::TESCondition* IsPowerAttacking;

	bool enableSneakStaminaCost;
	bool enableLevelDifficulty;
	
	bool zeroAllWeapStagger;
	bool armorScalingEnabled;	

	bool IsBlockingWeaponSpellCasted = false;

	int maxFrameCheck = 6;

	static RE::FormID ParseFormID(const std::string& str);

	std::string FileName;

	static RE::TESFile* LookupLoadedModByName(std::string_view a_modName)
    {
        for (auto& file : RE::TESDataHandler::GetSingleton()->compiledFileCollection.files) {
            if (a_modName.size() == strlen(file->fileName) && _strnicmp(file->fileName, a_modName.data(), a_modName.size()) == 0) {
                return file;
            }
        }
        return nullptr;
    }

    static const RE::TESFile* LookupLoadedLightModByName(std::string_view a_modName)
    {
        for (auto& smallFile : RE::TESDataHandler::GetSingleton()->compiledFileCollection.smallFiles) {
            if (a_modName.size() == strlen(smallFile->fileName) && _strnicmp(smallFile->fileName, a_modName.data(), a_modName.size()) == 0) {
                return smallFile;
            }
        }
        return nullptr;
    }
};
