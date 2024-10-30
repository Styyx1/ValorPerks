# Valor Perks 
## Features:

**Adds specified spells in ini for the following scenarios:**  
- `IsAttacking`
- `IsBlocking`
- `IsSneaking`
- `IsSprinting` (including differentiation for Mounted)
- `IsCasting`
- `Spell when drawing bow`  
- `Spell when firing xbow`  
- `Spell when reloading xbow`  
- `IsJumping`

**Includes engine fixes/changes for:**  
- `Movement speed for player scale`  
- `Block cap will use fBlockMax instead of fPlayerMaxResistance`  
- `Spell absorption cap will no longer exceed fPlayerMaxResistance` 

**Other features**  
- `Implements a new armor rating scaling formula`    
- `Overrides the Stamina cost of Blocking and Bashing`
- `Implements a Stamina Cost for light attacks` (Controlled by a global variable, one for player and one for NPCs)
- `Spawns Sparks on timed blocks/parries`
- `Applies a spell to aggressor upon a successful timed block/parry`
- `Redirects lockpicking XP to pickpocket`
- `Catches melee attacks while unarmed and redirects the XP to lockpicking`
- `Changes XP bar in lockpick menu to use pickpocket XP and level`

### Requirements
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang?)

## Getting Started
```bat
git clone clone https://github.com/Styyx1/ValorPerks --recursive
cd ValorPerks
git checkout xmake-dev
```

### Build
To build the project, run the following command:
```bat
xmake build
```

> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Build Output (Optional)
If you want to redirect the build output, set one of or both of the following environment variables:

- Path to a Skyrim install folder: `XSE_TES5_GAME_PATH`

- Path to a Mod Manager mods folder: `XSE_TES5_MODS_PATH`

### Project Generation (Optional)
If you want to generate a Visual Studio project, run the following command:
```bat
xmake project -k vsxmake
```

> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

### Upgrading Packages (Optional)
If you want to upgrade the project's dependencies, run the following commands:
```bat
xmake repo --update
xmake require --upgrade
```

# Credits
* This DLL is a fork of [colinswrath's](https://github.com/colinswrath) DLL for [Blade and Blunt](https://github.com/colinswrath/BladeAndBlunt) by SimonMagus. 
* A majority of the original RE work for Blade and Blunt was done by [KernalsEgg](https://github.com/KernalsEgg). 
* This DLL includes code from [dTry's](https://github.com/D7ry) DLL for [Valhalla Combat](https://github.com/D7ry/valhallaCombat) and [colinswrath's](https://github.com/colinswrath) DLL for [Hand to Hand](https://github.com/colinswrath/handtohand) by SimonMagus.
* [CharmedBaron](https://github.com/CharmedBaryon) for Commonlib-NG.
* [Alandtse](https://github.com/alandtse) for Clib-VR with NG branch.