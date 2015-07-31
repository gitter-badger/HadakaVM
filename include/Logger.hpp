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

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <libgen.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#define ERROR(msg) {char buf[] = __FILE__;std::cerr << "[ERROR] [" << basename(buf) << "::" << __FUNCTION__ << "] " << msg << std::endl;throw msg;}
#define WARN(msg) {char buf[] = __FILE__;std::cout << "[WARNING] [" << basename(buf) << "::" << __FUNCTION__ << "] " << msg << std::endl;}
#define LOG(msg) {char buf[] = __FILE__;std::cout << "[" << basename(buf) << "::" << __FUNCTION__ << "] " << msg << std::endl;}

template<class T>
std::string strInt(T integer)
{
  std::stringstream stream;
  stream << "0x" << std::setfill ('0') << std::setw(sizeof(T)*2) << std::hex << int(integer);
  return stream.str();
}

#endif
