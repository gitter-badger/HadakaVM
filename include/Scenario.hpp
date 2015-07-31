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
	{ DWORD_0 = 0x0,STRING = 0x2,DWORD_3 = 0x3,ADDR = 0x40 };

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
enum class I_Operation : uint16_t
{
	DRAW_IMAGE = 0x00C8,
	LOAD_SGF = 0x0118,
	SET_INGAME_MENU = 0x011A,
	SET_CONFIG = 0x011B,
	SET_SAVE_LOAD = 0x011E,
	SET_SAVE_LOAD_TOPBAR = 0x12A,
	SET_LOG_BTN = 0x12B,
	LOAD_OGG = 0x0126,
	SET_LOG = 0x015E,
};

class ScenarioRunner
{
	public:
		ScenarioRunner(std::string path);
		~ScenarioRunner();
		void runScenario(std::string name);
		std::string getString(std::string name);
		uint32_t getDWORD(std::string name);
		void start();
	private:
		Parameter getParameter();
		char* buffer;
		uint32_t pc;
		std::string ss_name,ts_name;
		uint32_t hs_files,hs_total,s_count;
		streamoff bufferLength;

		//I should probably use map's instead
		std::map<std::string,uint32_t> reg_dword;
		std::map<std::string,std::string> reg_str;
		std::map<std::string,Rectangle> reg_rect;
		std::map<std::string,Point> reg_point;
		std::map<std::string,Texture> reg_texture;
		std::map<std::string,Button> reg_btn;

		//Temporary variables for storing stuff (Will be stored elsewhere later on)

		//Save preview
		Rectangle savePreview_save,savePreview_load;

		//savePreview_load
		uint32_t saveCount,saveConst;

		//Text Box
		Texture textBox_frame,textBox_bg;
		//Name Box
		Texture nameBox_frame,nameBox_bg;
		//Choice Box
		std::string choiceBox_frame,choiceBox_bg;
		std::string choiceBox_frame_ON,choiceBox_bg_ON;

		//Text Bounds
		Rectangle textbox_textBounds;
		Rectangle namebox_textBounds;
		Rectangle choicebox_textBounds;

		vector<Scenario> scenarios;
		Window window;
		ResourceManager r_manager;
		binfile file;
};

#endif
