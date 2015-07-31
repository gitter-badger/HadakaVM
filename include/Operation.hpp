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

#ifndef OPERATION_HPP
#define OPERATION_HPP

#include "Scenario.hpp"

namespace Operation
{
  //Archive
  void archive_graphics(ScenarioRunner* sr);
  void archive_music(ScenarioRunner* sr);
  void archive_sound(ScenarioRunner* sr);
  //Window
  void window_icon(ScenarioRunner* sr);
  void window_title(ScenarioRunner* sr);
  void window_open(ScenarioRunner* sr);
  //Audio
  void play_uncategorized(ScenarioRunner* sr);
  void play_bgm(ScenarioRunner* sr);
  void play_sfx(ScenarioRunner* sr);
  //Jump
  void jump(ScenarioRunner* sr);
  void jump_conditinal(ScenarioRunner* sr);
  //Scenario
  void scenario_enter(ScenarioRunner* sr);
  void scenario_title_set(ScenarioRunner* sr);
  //Choice
  void choice_add(ScenarioRunner* sr);
  void choices_show(ScenarioRunner* sr);
  //Variable
  void set(ScenarioRunner* sr);
  void add(ScenarioRunner* sr);
  //Other
  void sleep(ScenarioRunner* sr);
  void exit(ScenarioRunner* sr);
  void stop(ScenarioRunner* sr);
  void scale_down(ScenarioRunner* sr);
}

#endif
