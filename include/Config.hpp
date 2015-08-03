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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <stdint.h>
#include <string>
#include <map>
#include <fstream>

class ConfigFile
{
  public:
    ConfigFile(std::string path);
    std::map<std::string,std::uint32_t>& getEntries();
    uint32_t get(std::string key);
    void set(std::string key,uint32_t value);
    bool isSet(std::string key);
    void write();
  private:
    std::string path;
    std::map<std::string,std::uint32_t> entries;
};

#endif