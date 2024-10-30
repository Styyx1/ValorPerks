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

    // FormIDs for ini lookup
    RE::FormID ID1AttackSpell;
    RE::FormID ID2BlockSpell;
    RE::FormID ID3SneakSpell;
    RE::FormID ID4SprinSpell;
    RE::FormID ID5MountSprint;
    RE::FormID ID6BowStam;
    RE::FormID ID7XBowStam;
    RE::FormID ID8CastSpell;
    RE::FormID ID9ParryStagger;
    RE::FormID ID10ParryController;
    RE::FormID ID11CrossbowDrain;
    RE::FormID ID12ParryBuff;
    RE::FormID ID13JumpSpell;
    RE::FormID ID14PowerAttackStop;
    RE::FormID ID15DodgeSpell;
    RE::FormID ID16BashStamina;
    RE::FormID ID17BlockStamina;
    RE::FormID ID18SparksNormal;
    RE::FormID ID19SparksPhysics;
    RE::FormID ID20SparksWeapon;
    RE::FormID ID21SparksShield;
    RE::FormID ID22StaminaCost;
    RE::FormID ID23StaminaCostNpc;
    RE::FormID ID24DualBlock;
    RE::FormID ID25ParryWinEffect;

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
    RE::SpellItem* MAGParryControllerSpell;
    RE::SpellItem* MAGCrossbowStaminaDrainSpell;
    RE::SpellItem* APOParryBuffSPell;
    RE::SpellItem* jumpSpell;
    RE::SpellItem* PowerAttackStopSpell;

    RE::SpellItem* DodgeRuneSpell;
    // Perks
    RE::BGSPerk* BashStaminaPerk;
    RE::BGSPerk* BlockStaminaPerk;
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

    //tests
    RE::SpellItem* fireBolt;

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
    float               surroundingActorsRange;
    // int
    inline static uint32_t blockingKey[RE::INPUT_DEVICE::kFlatTotal] = { 0xFF, 0xFF, 0xFF };
    inline static uint32_t blockKeyMouse{ 0xFF };
    inline static uint32_t blockKeyKeyboard{ 0xFF };
    inline static uint32_t blockKeyGamePad{ 0xFF };
    int                    maxFrameCheck = 6;
    uint32_t               dualBlockKey;

    // tests
    RE::BGSPerk* dummyPerkDodge;

    static RE::FormID ParseFormID(const std::string& str);

    std::string FileName;
};
