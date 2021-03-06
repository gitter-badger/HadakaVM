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

#include <iostream>
#include <string>
#include <stdint.h>
#include <algorithm>

#include "Logger.hpp"
#include "Archive.hpp"

ArchiveFile::ArchiveFile(std::string path) : file(path)
{
	uint32_t pc = 0;
	uint16_t filecount;
	char buffer[18];
	char* s_buffer = reinterpret_cast<char*>(buffer);

  if (!file.read(buffer,8)) ERROR("Failed to read magic");

  //This could be done in a easier manner (e.g. strcmp, std::string::compare), but this doesn't really work with MinGW
  for (char c : {0x47,0x4C,0x4E,0x4B,0x6E,0x00})
		if (c!=buffer[pc++]) ERROR("Invalid magic");

  //Header Filecount
  filecount = *reinterpret_cast<uint16_t*>(&buffer[6]);

    /*
     In this part that we skip here, there are the following "values":
     A static version number; Checked by the game; Can be simply ignored
     Some kind of checksum; Not checked by the game either (?)
     */
    if (!file.skipg(10)) ERROR("Failed to skip end of file header");

    //Reading the file entries
    for (int i = 0;i < filecount;i++) {
			ArchiveEntry a_file;

			//Read file name
			unsigned char nameLength;
      if (!file.read(s_buffer[0])) ERROR("Failed to read "+to_string(i)+"'s name length")

      nameLength=buffer[0];
			for (uint8_t c = 0;c < nameLength;c++) {
        if (!file.read(s_buffer[0])) ERROR("Failed to read "+to_string(i)+"'s name")
				a_file.name+=buffer[0];
			}

			//Read file offset
			if (!file.read(a_file.offset)) ERROR("Failed to read "+a_file.name+"'s offset");
			//Read file size
			if (!file.read(a_file.size)) ERROR("Failed to read "+a_file.name+"'s size");

			//Skipping yet antoher unknown value that is also ignored by the engine
			if (!file.skipg(4)) ERROR("Failed to skip to "+a_file.name+"'s offset");
			files.push_back(a_file);
	}
	LOG("Loaded "+path);
}

vector<ArchiveEntry>& ArchiveFile::getFiles() {return files;}

char* ArchiveFile::get(std::string name,uint32_t& size)
{
	for (ArchiveEntry& f : files) {
		std::string c_name = f.name;
		std::transform(c_name.begin(), c_name.end(), c_name.begin(), ::tolower);
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		if (c_name == name) {
			file.seekg(f.offset);
			char* buffer = new char[f.size];
			file.read(buffer,f.size);
			size=f.size;
			return buffer;
		}
	}
	ERROR("Failed to locate '"+name+"'");
}
