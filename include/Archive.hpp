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

#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include <string>
#include <vector>
#include <stdint.h>

#include "BinaryIO.hpp"

struct ArchiveEntry
{
	std::string name;
	uint32_t offset;
	uint32_t size;
};

class ArchiveFile
{
	public:
		ArchiveFile(std::string path);
		char* get(std::string name);
		vector<ArchiveEntry>& getFiles();
	private:
		vector<ArchiveEntry> files;
		binfile file;
};

#endif
