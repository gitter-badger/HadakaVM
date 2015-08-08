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

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdint.h>
#include <string>
#include <map>
#include <SDL2/SDL.h>

class ScenarioRunner;

enum class UpdateEventType
{
  NOTHING,
  EXIT,
  CLICK
};

struct UpdateEventClick
{
  int x,y;
};

struct UpdateEvent
{
  UpdateEventType type=UpdateEventType::NOTHING;
  UpdateEventClick* click=0;
};

struct Layer
{
  uint32_t x=0,y=0;
  SDL_Surface surface;
  SDL_Rect rect;
};

class Window
{
  public:
    void setTitle(std::string title);
    void init();
    UpdateEvent update();
    void sleep(ScenarioRunner* sr,uint32_t ms,bool skippable);
    void setLayer(uint32_t id,Layer layer);
    void flushLayers(uint32_t fadeIn);
  private:
    std::map<uint32_t,Layer> layers,layer_buffer;
    std::string icon,title;
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
};

#endif
