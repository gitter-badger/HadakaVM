/*
    HadakaVM: Free implementation of Hadaka Shitsujis Game Engine
    Copyright (C) 2015 spycrab0

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Operation.hpp"
#include "Scenario.hpp"
#include "Logger.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

inline void loadArchive(ScenarioRunner* sr,std::string type)
{
  sr->getResourceManager()->loadArchive(type,"data/"+sr->getString());
}

inline void playSFX(ScenarioRunner* sr)
{
  uint32_t size=0;
  SDL_RWops* rw = SDL_RWFromMem(sr->getResourceManager()->getFile("sound",sr->getString(),size),size);
  Mix_Chunk* chunk = Mix_LoadWAV_RW(rw,1);
  Mix_PlayChannel(-1,chunk,0);
}

inline void playBGM(ScenarioRunner* sr)
{
  uint32_t size=0;
  SDL_RWops* rw = SDL_RWFromMem(sr->getResourceManager()->getFile("music",sr->getString(),size),size);
  Mix_Music* music = Mix_LoadMUS_RW(rw,1);
  Mix_PlayMusic(music,-1);
}

//Graphics
void Operation::image_set(ScenarioRunner* sr)
{
  uint32_t id = sr->getDWORD(),size=0;
  SDL_RWops* rw = SDL_RWFromMem(sr->getResourceManager()->getFile("graphics",sr->getString(),size),size);

  Layer layer;
  layer.surface = IMG_LoadPNG_RW(rw);

  layer.rect.x = sr->getDWORD();
  layer.rect.y = sr->getDWORD();
  layer.rect.w = layer.surface->w;
  layer.rect.h = layer.surface->h;

  if (sr->getDWORD()) sr->getWindow()->setLayer(id,layer);

  sr->getDWORD();
  sr->getDWORD();
}
void Operation::image_flush(ScenarioRunner* sr)
{
  sr->getDWORD();
  sr->getWindow()->flushLayers(sr->getDWORD());
}

//Archive
void Operation::archive_graphics(ScenarioRunner* sr) {loadArchive(sr,"graphics");}
void Operation::archive_music(ScenarioRunner* sr) {loadArchive(sr,"music");}
void Operation::archive_sound(ScenarioRunner* sr) {loadArchive(sr,"sound");}

//Jump
void Operation::jump(ScenarioRunner* sr)
{
  sr->setPC(sr->getScenario()->offset+4+sr->getDWORD());
}

void Operation::jump_conditinal(ScenarioRunner* sr)
{
  uint32_t a = sr->getLogicVar(sr->getDWORD()),opr = sr->getDWORD(), b = sr->getDWORD();bool cond=false;
    switch (opr) {
      case 0x0: //ZERO
        cond = (a-b)==0;
        break;
      case 0x1: //NOT LESS EQUALS
        cond = !(a<=b);
        break;
      case 0x2: //LESS
        cond = a<b;
        break;
      case 0x3: //NOT LESS
        cond = !(a<b);
        break;
      case 0x4: //LESS EQUALS
        cond = a<=b;
        break;
      case 0x5: //NOT ZERO
        cond = (a-b)!=0;
        break;
      default:
      ERROR("Unknown Jump Code: "+to_string(opr));
    };
    if (cond) {Operation::jump(sr);} else {sr->getDWORD();};
}

//Audio
void Operation::play_uncategorized(ScenarioRunner* sr)
{
  playSFX(sr);
  sr->getDWORD();
  sr->getDWORD();
}

void Operation::play_sfx(ScenarioRunner* sr)
{
  playSFX(sr);
  sr->getDWORD();
  sr->getDWORD();
}

void Operation::play_bgm(ScenarioRunner* sr)
{
  playBGM(sr);
  sr->getDWORD();
}

//Scenario
void Operation::scenario_title_set(ScenarioRunner* sr)
{sr->setTitleScenario(sr->getString());}

void Operation::scenario_enter(ScenarioRunner* sr)
{
  uint32_t old_pc = sr->getPC();
  Scenario old_sc = *sr->getScenario();

  sr->runScenario(sr->getString());

  sr->setScenario(old_sc);
  sr->setPC(old_pc);
}

//Choice
void Operation::choice_add(ScenarioRunner* sr)
{
  sr->getString();
  sr->getDWORD();
}

void Operation::choices_show(ScenarioRunner* sr)
{
  sr->getString();
}
//Window
void Operation::window_open(ScenarioRunner* sr) {sr->getWindow()->init();}
void Operation::window_icon(ScenarioRunner* sr) {sr->getString();}
void Operation::window_title(ScenarioRunner* sr) {sr->getString();}

//Variable
void Operation::set(ScenarioRunner* sr) {sr->setLogicVar(sr->getDWORD(),sr->getDWORD());}
void Operation::add(ScenarioRunner* sr)
{
  uint32_t key = sr->getDWORD();
  sr->setLogicVar(key,sr->getLogicVar(key)+sr->getDWORD());
}

//Other
void Operation::sleep(ScenarioRunner* sr) {sr->getWindow()->sleep(sr,sr->getDWORD(),sr->getDWORD());}
void Operation::exit(ScenarioRunner* sr) {sr->exit();ERROR("Execution halted by user")}
void Operation::stop(ScenarioRunner* sr) {sr->exit();ERROR("Received stop signal by engine");}
void Operation::scale_down(ScenarioRunner* sr) {sr->getDWORD();}
