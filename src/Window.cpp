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

#include <stdint.h>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>

#include "Window.hpp"
#include "Scenario.hpp"
#include "Logger.hpp"

void Window::init()
{
  sdl_window = SDL_CreateWindow("HadakaVM",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_SHOWN);
  sdl_renderer = SDL_CreateRenderer(sdl_window,-1, SDL_RENDERER_ACCELERATED);
}

void Window::setTitle(std::string title)
{this->title = title;}

void Window::setLayer(uint32_t id,Layer layer)
{
  layer_buffer[id] = layer;
}

void Window::flushLayers(uint32_t fadeIn)
{
  uint64_t c_old = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  for (uint64_t c_now=c_old;c_now-c_old < fadeIn;c_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
  {
    for (auto& it : layer_buffer) {
      if (layers.find(it.first)==layers.end()) continue;
      SDL_SetSurfaceAlphaMod(layers[it.first].surface,255-((c_now-c_old)*255)/fadeIn);
    }
    update();
  }
  for (auto it : layer_buffer) {
    layers[it.first] = it.second;
    SDL_SetSurfaceAlphaMod(layers[it.first].surface,0);
    SDL_SetSurfaceBlendMode(layers[it.first].surface,SDL_BLENDMODE_ADD);
  }
  layer_buffer.clear();
  c_old=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  for (uint64_t c_now=c_old;c_now-c_old < fadeIn;c_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
  {
    for (auto& it : layers) {
      SDL_SetSurfaceAlphaMod(layers[it.first].surface,((c_now-c_old)*255)/fadeIn);
    }
    update();
  }
}

UpdateEvent Window::update()
{
  UpdateEvent u_evt;
  SDL_Event evt;
  while (SDL_PollEvent(&evt)) {
    switch (evt.type)
    {
      case SDL_QUIT:
        u_evt.type = UpdateEventType::EXIT;
        break;
      case SDL_MOUSEBUTTONDOWN:
        switch (evt.button.button) {
          case SDL_BUTTON_LEFT:
            u_evt.type = UpdateEventType::CLICK;
            u_evt.click = new UpdateEventClick;
            SDL_GetMouseState(&u_evt.click->x,&u_evt.click->y);
            break;
        }
        break;
      case SDL_KEYDOWN:
        switch (evt.key.keysym.sym) {
          case SDLK_SPACE:
            u_evt.type = UpdateEventType::CLICK;
            u_evt.click = new UpdateEventClick;
            SDL_GetMouseState(&u_evt.click->x,&u_evt.click->y);
            break;
          case SDLK_ESCAPE:
            u_evt.type = UpdateEventType::EXIT;
          break;
        }
      break;
    }
  }
  SDL_RenderClear(sdl_renderer);
  for (auto it : layers) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer,it.second.surface);
    SDL_RenderCopy(sdl_renderer,texture,nullptr,&it.second.rect);
  }
  SDL_RenderPresent(sdl_renderer);
  return u_evt;
}

void Window::sleep(ScenarioRunner* sr,uint32_t ms,bool skippable)
{
  if (skippable)
  {
    uint32_t skip=1;
    uint64_t c_old = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (uint64_t c_now=c_old;c_now-c_old < ms;c_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
    {
        if (Window::update().type==UpdateEventType::CLICK || Window::update().type==UpdateEventType::EXIT) {
          skip=0;
          break;
        }
    }
    sr->setLogicVar(0,skip);
  } else {SDL_Delay(ms);}
}
