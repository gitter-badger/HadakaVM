# HadakaVM
Free implementation of Hadaka Shitsujis Game Engine

## Status
Currently the game is not playable, as only fragments of the engine are implemented.

## Supported plattforms

Every plattform that supports:
- Make
- C++11 (Clang/GCC tested)
- SDL2
  - SDL2_image
  - SDL2_mixer

### Tested plattforms / setups

- Windows ( Tested on Windows 7 SP1)
  - Compiler: MinGW 5.1.0 (I use [Nuwen.net's MinGW Distro](http://nuwen.net/mingw.html), which already has SDL2 and SDL2_mixer prepackaged)
  - Install dependencies:
    - [SDL2](https://www.libsdl.org/download-2.0.php)
    - [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
    - [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)
  
    
- OSX (Tested on OS X 10.10.1)
  - Compiler: Apple's Clang (Part of Xcode Command Line Tools)
  - Install dependencies (Using Homebrew): 
  ```
  brew install sdl2 sdl2_image sdl2_mixer
  ```

## TODO
- Add locale fix (Will probably result in a dependency on boost_locale)
- Add archive memory parsing
- Implement patch loading
- Opcode cleanup
- Universal register
- Add debugger
- Implement remaining opcodes
  - Variable manipulations
  - Conditinal jumps
  - Skippable sleep
- Implement missing graphics/interface features
  - Proper rendering queue
  - Set/Update images
  - Play BGM
  - Play SFX
  - Improve user input

## License
Licensed under the GNU General Public License v3 or any later version,  
See [LICENSE](LICENSE).
