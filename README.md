# HadakaVM [![Build Status](https://travis-ci.org/spycrab/HadakaVM.svg)](https://travis-ci.org/spycrab/HadakaVM)
Free implementation of Hadaka Shitsujis Game Engine

## Status
Currently the game is not playable, as only fragments of the engine are implemented.  
See [Operation Codes](https://github.com/spycrab/HadakaVM/wiki/Operation-Codes)  

## Demos
HadakaVM Daily 10.08.15 - Booting up to the menu screen  
[![HadakaVM Daily 10.08.15 - Booting up to the menu screen](http://img.youtube.com/vi/bf4CctMoAao/0.jpg)](http://www.youtube.com/watch?v=bf4CctMoAao)

## Supported plattforms

Every plattform that supports:
- Make
- C++11 (Clang/GCC tested)
- SDL2
  - SDL2_image
  - SDL2_mixer (With libvorbis support)

### Tested plattforms / setups

- Windows (Tested on Windows 7 SP1)
  - Compiler: GCC 5.1.0 (I use [Nuwen.net's MinGW Distro](http://nuwen.net/mingw.html), which already has SDL2 and SDL2_mixer prepackaged)
  - Install dependencies:
    - [SDL2](https://www.libsdl.org/download-2.0.php)
      - [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
      - [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)


- OSX (Tested on OS X 10.10.1)
  - Compiler: Apple's Clang (Part of Xcode Command Line Tools) / GCC 5.2.0
  - Install dependencies (Using Homebrew):
  ```
  brew install sdl2 sdl2_image sdl2_mixer --with-libvorbis
  ```

## TODO
- Add locale fix (Will probably result in a dependency on boost_locale)
- Implement patch loading
- Universal register
- Add debugger  
- Restructure ScenarioRunner  
- Implement remaining opcodes
- Implement missing graphics features
  - Proper rendering queue
  - Set/Update images

## License
Licensed under the GNU General Public License v3 or any later version,  
See [LICENSE](LICENSE).
