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

#include <string>
#include <fstream>

#include "BinaryIO.hpp"

/*
	bintools
*/

streamoff bintools::filesize(string filename) {
	ifstream str(filename, ios::ate | ios::binary);
	return str.tellg();
}

/*
	binfile
*/

binfile::binfile(std::string filename) : stream(filename, ios::in | ios::out | ios::binary) {
	//stream = fstream(filename, ios::in | ios::out | ios::binary);
	stream.seekg(0, ios::beg);
}

binfile::binfile(std::string filename,ios_base::openmode mode) :stream(filename,mode) {
	//stream = fstream(filename.c_str(), mode);
	stream.seekg(0, ios::beg);
}

//Close
void binfile::close() {
	stream.close();
}

//Read
bool binfile::read(char* buffer, unsigned int length) {
	for (unsigned int i = 0; i < length; i++) { if (!stream.get(buffer[i])) return false; }
	return true;
}

bool binfile::read(unsigned char* buffer, unsigned int length) {
	return read(reinterpret_cast<char*>(buffer), length);
}

//Write
bool binfile::writenull(unsigned int length) {
	for (unsigned int i = 0;i < length;i++) { if (!stream.put(0x0)) return false; }
	return true;
}

//Seek, Skip - Read
bool binfile::seekg(streamoff offset) {
	stream.seekg(offset);
	if (stream.fail()) { return false; } else { return true; }
}
bool binfile::skipg(streamoff offset) {  return seekg(stream.tellg() + offset); }
streampos binfile::tellg() { return stream.tellg();  }

//Seek, Skip - Write
bool binfile::seekp(streamoff offset) {
	stream.seekp(offset);
	if (stream.fail()) { return false; } else { return true; }
}
bool binfile::skipp(streamoff offset) { return seekp(stream.tellp() + offset); }
streampos binfile::tellp() { return stream.tellp(); }
