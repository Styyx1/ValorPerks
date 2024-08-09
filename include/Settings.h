#pragma once

class Settings
{
public:
    static Settings* GetSingleton();

    void LoadSettings();
    void LoadForms();
    void AdjustWeaponStaggerVals();
    void GetIngameData();
    void SetGlobalsAndGameSettings();
    // Spells
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

    RE::SpellItem* DodgeRuneSpell;
    // Perks
    RE::BGSPerk* BashStaminaPerk;
    RE::BGSPerk* BlockStaminaPerk;
    RE::BGSPerk* BlockStaggerPerk;
    // Explosions (Sparks)
    RE::BGSExplosion* APOSparks;
    RE::BGSExplosion* APOSparksPhysics;
    RE::BGSExplosion* APOSparksFlash;
    RE::BGSExplosion* APOSparksShieldFlash;
    // Globals
    RE::TESGlobal* StaminaCostGlobal;
    RE::TESGlobal* NPCStaminaCostGlobal;
    RE::TESGlobal* DualBlockKey;
    // Effects
    RE::EffectSetting* MAG_ParryWindowEffect;
    // Conditions
    RE::TESCondition* IsPowerAttacking;

    // bools
    bool               enableSneakStaminaCost;
    bool               enableLevelDifficulty;
    bool               zeroAllWeapStagger;
    bool               armorScalingEnabled;
    bool               IsBlockingWeaponSpellCasted = false;
    bool               wasPowerAttacking           = false;
    inline static bool debug_logging{};
    // floats
    inline static float BonusXPPerLevel;
    inline static float BaseXP;
    float               blockAngleSetting;
    // int
    inline static uint32_t blockingKey[RE::INPUT_DEVICE::kFlatTotal] = { 0xFF, 0xFF, 0xFF };
    inline static uint32_t blockKeyMouse{ 0xFF };
    inline static uint32_t blockKeyKeyboard{ 0xFF };
    inline static uint32_t blockKeyGamePad{ 0xFF };
    int                    maxFrameCheck = 6;
    uint32_t               dualBlockKey;

    static RE::FormID ParseFormID(const std::string& str);

    std::string FileName;
};
