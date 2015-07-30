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

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <stdint.h>
#include <string>

struct Rectangle {uint32_t x=0,y=0,width=0,height=0;};
struct Point {uint32_t x=0,y=0;};

struct Texture {
  Rectangle position;
  std::string texture = "";
  bool autoSize = false;
};

//Temporary
typedef Texture Texture2D;
typedef Rectangle Rectangle2D;
typedef Point Point2D;

struct MultiTexture : Rectangle {
  std::map<std::string,std::string> texture;
};

struct Button2D {
  Rectangle2D position;
  std::string t_default="",t_hover="",t_click="",t_bg="";
};

#endif
