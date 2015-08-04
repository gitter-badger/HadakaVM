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

#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include <string>
#include <vector>
#include <utility>
#include <stdint.h>

#include "BinaryIO.hpp"
#include "ResourceManager.hpp"
#include "Utility.hpp"
#include "Window.hpp"

struct Scenario
{
	std::string name="";
	uint32_t offset=0;
};


enum class p_type : uint8_t
	{ DWORD_0 = 0x0,STRING = 0x2,DWORD_3 = 0x3,DWORD_5=0x5,DWORD_6=0x6,ADDR = 0x40};

struct Parameter
{
	p_type type;
	struct
	{
		uint32_t dword;
		std::string string;
	} value;
};


/*
32 OPCodes

no comment - No implementation

Unknown - Implemented; Purpose/Parameter's purpose not fully/at all known
Implemented - A basic Parameter Echo, does nothing;might reflect a basic understanding of the parameters
Untested - Implementation that should work, but were never tested.
Working - A Working Implementation that was tested

Next up:
Discover 5,1,116,10E,113's purpose.

*/

class ScenarioRunner
{
	public:
		ScenarioRunner(std::string path);
		~ScenarioRunner();
		void runScenario(std::string name);

		std::string getString();
		uint32_t getDWORD();

		ResourceManager* getResourceManager();
		Window* getWindow();

		void setPC(uint32_t pc);
		uint32_t getPC();

		Scenario* getScenario();
		void setScenario(Scenario s); //Don't use this method for running Scenarios, use runScenario instead!

		void setTitleScenario(std::string s);

		void setLogicVar(uint32_t key,uint32_t value);
		uint32_t getLogicVar(uint32_t key);

		void start();
		void exit();
	private:
		Parameter getParameter();
		char* buffer;
		uint32_t pc;
		std::string ss_name,ts_name;
		streamoff fsize;

		std::map<uint32_t,uint32_t> reg_logic;
		std::map<std::string,uint32_t> reg_dword;
		std::map<std::string,std::string> reg_str;
		std::map<std::string,Rectangle> reg_rect;
		std::map<std::string,Point> reg_point;
		std::map<std::string,Texture> reg_texture;
		std::map<std::string,Button> reg_btn;

		vector<Scenario> scenarios;
		Scenario c_scenario;
		Window window;
		ResourceManager r_manager;
		binfile file;
};

#endif
