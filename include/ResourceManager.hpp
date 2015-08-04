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

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <string>
#include <stdint.h>
#include <map>

#include "Archive.hpp"
#include "Message.hpp"
#include "Config.hpp"

class ResourceManager
{
  public:
    ResourceManager();
    ~ResourceManager();
    void loadArchive(std::string name,std::string archive);
    char* getFile(std::string name,std::string file,uint32_t& size);
    std::string getMessage(uint32_t id);
  private:
    void initConfig();
    ConfigFile hadaka_cfg,vm_cfg;
    MessageFile msg;
    std::map<std::string,ArchiveFile*> archives;
};

#endif
