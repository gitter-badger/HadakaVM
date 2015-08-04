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

#include <fstream>

#include "Config.hpp"
#include "Logger.hpp"

ConfigFile::ConfigFile(std::string path)
{
  this->path = path;
  std::ifstream* ifs = new std::ifstream(path);
  if (!ifs->good()) {
    ifs->close();
    delete ifs;
    ifs = new std::ifstream(path,std::fstream::app);
    if (!ifs->good()) ERROR("Failed to open "+path);
  }
  ifs->seekg(0, std::ios::beg);

  std::string line,prefix="";
  for (uint32_t lineNr = 1;ifs->good();lineNr++) {
    getline(*ifs,line);

    if (line.size()<1) {
      continue;
    } else {
      if (line.substr(0,1)==";") continue;
      if (line.substr(0,1)=="[")
      {
        prefix="";
        for (char c : line.substr(1))
        {
          if (c==']') {
            prefix+=".";
            break;
          }
          prefix+=c;
        }
        continue;
      }

      std::string key="",s_value="";

      for (uint32_t i = 0;i < line.size();i++) {
        if (line[i]=='=') {
          key = line.substr(0,i);
          s_value = line.substr(i+1);
          break;
        }
        if (i==line.size()-1) ERROR("Missing equals sign / no value assigned at line "+std::to_string(lineNr));
      }

      LOG(prefix+key+"="+s_value);
      entries[prefix+key] = std::stoi(s_value,nullptr);
    }
  }
  ifs->close();
  LOG("Loaded "+std::to_string(entries.size())+" pair(s) from "+path);
}

uint32_t ConfigFile::get(std::string key) {return entries[key];}
void ConfigFile::set(std::string key,uint32_t value) {entries[key] = value;}
bool ConfigFile::isSet(std::string key) {return entries.find(key)!=entries.end();}

std::map<std::string,std::uint32_t>& ConfigFile::getEntries() {return entries;}

void ConfigFile::write()
{
  std::ofstream ofs(path);
  std::string c_prefix = "";
  for (auto& pair : entries) {

    //Get prefix
    std::string prefix = "";
    for (char c : pair.first) {
      prefix+=c;
      if (c=='.') break;
    }

    //If there's now path/prefix seperator, there is no prefix
    if (prefix[prefix.size()-1] != '.') prefix="";

    if (prefix!=c_prefix)
      ofs << "[" << prefix.substr(0,prefix.size()-1) << "]\r\n";

    c_prefix=prefix;

    //Get key
    std::string key = pair.first.substr(prefix.size());
    std::string value = std::to_string(pair.second);

    //Write pair
    ofs << key << "=" << value << "\r\n";
  }
  ofs.flush();
  ofs.close();
  LOG("Wrote "+std::to_string(entries.size())+" pair(s)");
}
