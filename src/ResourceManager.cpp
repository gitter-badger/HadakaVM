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

#include "ResourceManager.hpp"
#include "Logger.hpp"
#include <string>

ResourceManager::~ResourceManager()
{
  for(std::map<std::string,ArchiveFile*>::iterator it = archives.begin(); it != archives.end(); ++it) {
    delete it->second;
  }
}

ResourceManager::ResourceManager() : msg("data/msg.dat") {}

std::string ResourceManager::getMessage(uint32_t id) { return msg.get(id); }

char* ResourceManager::getFile(std::string name,std::string file)
{
  if (archives.count(name) == 0)
    ERROR("Requested unloaded '"+name+"'");
  return archives[name]->get(file);
}

void ResourceManager::loadArchive(std::string name,std::string archive) { archives[name] = new ArchiveFile(archive); }
