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

#include "Message.hpp"
#include "Logger.hpp"
#include <string>

MessageFile::~MessageFile()
{
    delete[] buffer;
}

MessageFile::MessageFile(std::string path) : file(path)
{
  fsize = bintools::filesize(path);
  buffer = new char[fsize];
  file.read(buffer,fsize);

  if (fsize < 0x11) ERROR("File to small (fsize < 0x11)");
  if (*reinterpret_cast<uint32_t*>(&buffer[0]) != 0x204F4D48) ERROR("Invalid signature");
  if (*reinterpret_cast<uint16_t*>(&buffer[0x4]) != 0x64) ERROR("Invalid version");
  entrycount = reinterpret_cast<uint32_t*>(&buffer[0xA]);

  LOG("Loaded "+path);
}

std::string MessageFile::get(uint32_t id) {
    //This looks very messy, and in fact: it is
    uint32_t* entrylength = reinterpret_cast<uint32_t*>(&buffer[(0x12+*entrycount*4)+*reinterpret_cast<uint32_t*>(&buffer[0x12] + id*4)]);
    std::string msg="";
    for (uint32_t i=0;i < *entrylength;i++) {
      char* msg_char = reinterpret_cast<char*>(&buffer[(0x12+*entrycount*4)+*reinterpret_cast<uint32_t*>(&buffer[0x12] + id*4)]+4+i);
      msg+=~*msg_char;
    }
    LOG(to_string(id)+": "+msg);
    return msg;
}
