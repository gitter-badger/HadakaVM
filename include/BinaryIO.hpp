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

#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <string>
#include <fstream>
#include <stdint.h>
#include <iostream>

using namespace std;

namespace bintools
{
	streamoff filesize(string path);
};

class binfile
{
	public:
		binfile(string filename);
		binfile(string filename,ios_base::openmode mode);
		void close();

		//Seek, Skip - Read
		bool seekg(streamoff offset);
		bool skipg(streamoff offset);
		streampos tellg();

		//Seek, Skip - Write
		bool seekp(streamoff offset);
		bool skipp(streamoff offset);
		streampos tellp();

		//Read
		bool read(char* buffer, unsigned int length);
		bool read(unsigned char* buffer, unsigned int length);

		template <class T>
		bool read(T& val)
		{
			char buffer[sizeof(T)];
			unsigned char* u_buffer = reinterpret_cast<unsigned char*>(buffer);
			if (!read(u_buffer, sizeof(T))) return false;
			val = 0;
			for (uint32_t i = 0; i < sizeof(T); i++) val += u_buffer[i] << i * 8;
			return true;
		};

		//Write
		bool writenull(unsigned int length);

		template <class T>
		bool write(T* buffer,unsigned int length)
		{
			if (!stream.write(buffer, length)) return false;
			return true;
		};

		template <class T>
		bool write(T& val)
		{
			char buffer[sizeof(T)];
			for (uint32_t i = 0; i < sizeof(T); i++) {
				char byte = (val >> (8*i)) & 0xFF;
				buffer[i] = byte;
			}
			return write(buffer,sizeof(T));
		};

	private:
		fstream stream;
};

#endif
