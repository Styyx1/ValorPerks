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
    RE::FormID APOParryBuffSpellFormID;
    RE::FormID APONormalBlockSparksFormID;
    RE::FormID APOPhysicBlockSparksFormID;
    RE::FormID APOFlashSparksFormID;
    RE::FormID APOShieldFlashSparksFormID;
    RE::FormID APOJumpSpellFormID;
    RE::FormID APOPowerAttackStopSpellFormID;
    RE::FormID APOStaminaCostGlobalFormID;
    RE::FormID APOStaminaCostNPCGlobalFormID;

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
    RE::SpellItem* APOParryBuffSPell;
    RE::SpellItem* jumpSpell;
    RE::SpellItem* PowerAttackStopSpell;

    RE::BGSPerk* BashStaminaPerk;
    RE::BGSPerk* BlockStaminaPerk;
    RE::BGSPerk* BlockStaggerPerk;

    RE::BGSExplosion* APOSparks;
    RE::BGSExplosion* APOSparksPhysics;
    RE::BGSExplosion* APOSparksFlash;
    RE::BGSExplosion* APOSparksShieldFlash;

    RE::TESGlobal* StaminaCostGlobal;
    RE::TESGlobal* NPCStaminaCostGlobal;

    RE::EffectSetting* MAG_ParryWindowEffect;

    RE::TESCondition* IsPowerAttacking;

    bool enableSneakStaminaCost;
    bool enableLevelDifficulty;

    bool zeroAllWeapStagger;
    bool armorScalingEnabled;

    bool IsBlockingWeaponSpellCasted = false;

    bool                wasPowerAttacking = false;
    inline static float BonusXPPerLevel;
    inline static float BaseXP;

    int maxFrameCheck = 6;

    static RE::FormID ParseFormID(const std::string& str);

    std::string FileName;
};
