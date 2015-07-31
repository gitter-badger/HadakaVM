/*
    HadakaVM: Free implementation of Hadaka Shitsujis Game Engine
    Copyright (C) 2015 spycrab0
A
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
#include <SDL2/SDL.h>

class Window
{
  public:
    void setTitle(std::string title);
    void init();
    bool update();
    void sleep(uint32_t ms,bool skippable);
  private:
    std::string icon,title;
    SDL_Window* sdl_window;
    SDL_Surface* surface;
};

#endif
