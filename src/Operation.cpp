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

//Macros
#define LOAD_ARCHIVE(type) sr->getResourceManager()->loadArchive(type,"data/"+sr->getString());

//Archive
void Operation::archive_graphics(ScenarioRunner* sr) {LOAD_ARCHIVE("graphics");}
void Operation::archive_music(ScenarioRunner* sr) {LOAD_ARCHIVE("music");}
void Operation::archive_sound(ScenarioRunner* sr) {LOAD_ARCHIVE("sound");}

//Jump
void Operation::jump(ScenarioRunner* sr)
{
  sr->setPC(sr->getScenario()->offset+4+sr->getDWORD());
}
void Operation::jump_conditinal(ScenarioRunner* sr)
{
  uint32_t a = sr->getDWORD(),opr = sr->getDWORD(), b = sr->getDWORD();bool cond=false;
    switch (opr) {
      case 0x0:
      cond=(a==b);
        break;
      case 0x1:
        break;
      case 0x3:
        break;
      case 0x4:
        break;
      case 0x5:
        break;
      default:
      ERROR("Unknown Jump Code: "+to_string(opr));
    };
    if (cond) {Operation::jump(sr);} else {sr->getDWORD();};
}

//Audio
void Operation::play_uncategorized(ScenarioRunner* sr)
{
  sr->getString();
  sr->getDWORD();
  sr->getDWORD();
}

void Operation::play_sfx(ScenarioRunner* sr)
{
  sr->getString();
  sr->getDWORD();
  sr->getDWORD();
}

void Operation::play_bgm(ScenarioRunner* sr)
{
  sr->getString();
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
void Operation::sleep(ScenarioRunner* sr) {sr->getWindow()->sleep(sr->getDWORD(),sr->getDWORD());}
void Operation::exit(ScenarioRunner* sr) {sr->exit();ERROR("Execution halted by user")}
void Operation::stop(ScenarioRunner* sr) {sr->exit();ERROR("Received stop signal by engine");}
void Operation::scale_down(ScenarioRunner* sr) {sr->getDWORD();}