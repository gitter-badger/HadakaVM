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
#include "Logger.hpp"

void Window::init()
{
  //TODO Init code
  sdl_window = SDL_CreateWindow("HadakaVM",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,800,600,SDL_WINDOW_SHOWN);
  surface = SDL_GetWindowSurface(sdl_window);
}

void Window::setTitle(std::string title)
{this->title = title;}

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
      case SDL_KEYDOWN:
        switch (evt.key.keysym.sym) {
          case SDLK_ESCAPE:
            u_evt.type = UpdateEventType::EXIT;
          break;
        }
      break;
    }
  }
  return u_evt;
}

void Window::sleep(uint32_t ms,bool skippable)
{
  if (skippable)
  {
    uint64_t c_old = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (uint64_t c_now=c_old;c_now-c_old < ms;c_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
    {
      bool skip=false;
      //TODO Merge with Window::update()
      SDL_Event evt;
      while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
          case SDL_KEYDOWN:
            switch (evt.key.keysym.sym) {
              case SDLK_SPACE:
                skip=true;
                break;
            }
            break;
          case SDL_MOUSEBUTTONDOWN:
            switch (evt.button.button) {
              case SDL_BUTTON_LEFT:
                skip=true;
                break;
            }
            break;
        }
      }
      if (skip) break;
    }
  } else {SDL_Delay(ms);}
}
