# Features:

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

# Requirements
* [CMake](https://cmake.org/)
    * Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
    * Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2019](https://visualstudio.microsoft.com/)
    * Desktop development with C++

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/Styyx1/ValorPerks
cd ExamplePlugin-CommonLibSSE
git submodule update --init --recursive
cmake -B build -S .
```

# Credits
* This DLL is a fork of [colinswrath's](https://github.com/colinswrath) DLL for [Blade and Blunt](https://github.com/colinswrath/BladeAndBlunt) by SimonMagus. 
* A majority of the original RE work for Blade and Blunt was done by [KernalsEgg](https://github.com/KernalsEgg). 
* This DLL includes code from [dTry's](https://github.com/D7ry) DLL for [Valhalla Combat](https://github.com/D7ry/valhallaCombat) and [colinswrath's](https://github.com/colinswrath) DLL for [Hand to Hand](https://github.com/colinswrath/handtohand) by SimonMagus.
* [CharmedBaron](https://github.com/CharmedBaryon) for Commonlib-NG.
* [Alandtse](https://github.com/alandtse) for Clib-VR with NG branch.




