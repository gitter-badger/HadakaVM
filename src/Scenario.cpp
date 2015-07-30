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
#include <iostream>
#include <stdint.h>
#include "Logger.hpp"
#include "BinaryIO.hpp"
#include "Scenario.hpp"

ScenarioRunner::~ScenarioRunner()
{ delete[] buffer; }

ScenarioRunner::ScenarioRunner(std::string path) : file(path)
{
	hs_files=0;
	hs_total=0;

	std::streamoff fsize = bintools::filesize(path);
	buffer = new char[fsize];
	file.read(buffer,fsize);
	if (fsize < 0x14) //Filesize check (Static header contents)
	 ERROR("File to small");

	//Check Signature
	if (*reinterpret_cast<uint32_t*>(&buffer[0x0]) != 0x204C5348) ERROR("Invalid signature: "+strInt(*reinterpret_cast<uint32_t*>(&buffer[0x4])));
	//Check Version
	if (*reinterpret_cast<uint16_t*>(&buffer[0x4]) != 0x64) ERROR("Invalid version");

	hs_files = *reinterpret_cast<uint32_t*>(&buffer[0xA]); //Read filename header size
	hs_total = *reinterpret_cast<uint32_t*>(&buffer[0xE]); 	//Read total header size
	uint16_t ss_size = *reinterpret_cast<uint16_t*>(&buffer[0x12]); //Read start scenario name

	if (hs_files > hs_total)
		ERROR("Value error (hs_files > hs_total)");
	if (fsize < hs_total)
		ERROR("File to small (fsize < hs_total)");

	if (fsize < 0x18+ss_size) //Filesize check
		ERROR("File to small (fsize < 0x18+ss_size)");

	for (int i = 0;i < ss_size;i++)
	{
		if (buffer[0x14+i] == '\0')
			{ss_name.resize(i);break;}
		else
			{ss_name += buffer[0x14+i];}
	}

	uint32_t s_count = *reinterpret_cast<uint32_t*>(&buffer[0x14+ss_size]); 	//Read scenario count

	pc = 0x18+ss_size;

	if (fsize < pc+s_count*2)
		ERROR("File to small (fsize < pc+s_count)");
	for (uint32_t i=0;i < s_count;i++) {
		Scenario s;

		uint16_t size = *reinterpret_cast<uint16_t*>(&buffer[pc]); //Read scenario name

		for (int i = 0;i < size;i++)
		{
			if (buffer[pc+2+i] == '\0')
				{s.name.resize(i);break;}
			else
				{s.name += buffer[pc+2+i];}
		}

		s.offset = *reinterpret_cast<uint32_t*>(&buffer[hs_files+i*4]); //Read scenario offset

		scenarios.push_back(s);
		pc+=2+size;
	}

	LOG("Loaded "+path)
}

Parameter ScenarioRunner::getParameter()
{
	Parameter p;
	p.type = *reinterpret_cast<p_type*>(&buffer[pc++]);
	switch (p.type) {
		case p_type::STRING:
		{
			uint16_t str_size = *reinterpret_cast<uint16_t*>(&buffer[pc]);
			for (uint16_t i = 0;i < str_size;i++) {
				if (buffer[pc+2+i] == '\0')
					{p.value.string.resize(i);break;}
				else
					{p.value.string += buffer[pc+2+i];}
			}
			pc+=2+str_size;
			cout << "\"" << p.value.string << "\"" << "; ";
		}
		break;
		case p_type::DWORD_0:
		case p_type::DWORD_3:
			p.value.dword = *reinterpret_cast<uint32_t*>(&buffer[pc]);
			cout << p.value.dword << "; ";
			pc+=4;
		break;
		case p_type::ADDR:
			p.value.dword = *reinterpret_cast<uint32_t*>(&buffer[pc]);
			cout << "(ADDR)" << strInt(p.value.dword) << "; ";
			pc+=4;
		break;
		default:
			ERROR("Unknown Parametercode "+strInt(*reinterpret_cast<uint8_t*>(p.type))+";This indicates a parsing error");
		break;
	}
	return p;
}


//A few shortcuts
#define STR getParameter().value.string
#define DWORD getParameter().value.dword

#define NOTHING {}

void ScenarioRunner::runScenario(std::string name)
{
	Scenario scenario;
	for (Scenario s : scenarios) if (s.name == name) scenario = s;
	if (scenario.name=="")
			ERROR("Unknown scenario '"+name+"'");

	uint32_t scenarioSize = *reinterpret_cast<uint32_t*>(&buffer[scenario.offset]);

	LOG("Parsing scenario '"+scenario.name+"'");

	for (pc = scenario.offset+4;pc < scenario.offset+4+scenarioSize;) {
		uint16_t opcode = *reinterpret_cast<uint16_t*>(&buffer[pc]);
		pc+=2;

		bool did=false;

		//Operations that would be to big for the table and destroy its layout

		//Textbox
		#define OP_TEXTBOX textBox_frame.texture=STR;textBox_bg.texture=STR;\
											 textBox_frame.autoSize=textBox_bg.autoSize=true;\
											 textBox_frame.position.x=textBox_bg.position.x=DWORD;\
											 textBox_frame.position.y=textBox_bg.position.y=DWORD;
		#define OP_TEXTBOX_BOUNDS textbox_textBounds.x=DWORD;textbox_textBounds.y=DWORD;\
															textbox_textBounds.width=DWORD;textbox_textBounds.height=DWORD;
	  //Namebox
		#define OP_NAMEBOX nameBox_frame.texture=STR;nameBox_bg.texture=STR;\
											 nameBox_frame.autoSize=nameBox_bg.autoSize=true;\
											 nameBox_frame.position.x=nameBox_bg.position.x=~DWORD;\
											 nameBox_frame.position.y=nameBox_bg.position.y=DWORD;
		#define OP_NAMEBOX_BOUNDS namebox_textBounds.x=DWORD;namebox_textBounds.y=DWORD;\
									 						namebox_textBounds.width=DWORD;namebox_textBounds.height=DWORD;
    //Choicebox
		#define OP_CHOICEBOX reg_str["CB_FRAME"]=STR;reg_str["CB_BG"]=STR;\
												 reg_rect["CB_TEXT"].x=DWORD;reg_rect["CB_TEXT"].y=DWORD;\
												 reg_rect["CB_TEXT"].width=DWORD;reg_rect["CB_TEXT"].height=DWORD;
		#define OP_CHOICEBOX_ON choiceBox_frame_ON=STR;choiceBox_bg_ON=STR;
		//Save/Load Preview
		#define OP_SL_PREVIEW_XY savePreview_save.x=DWORD;savePreview_save.y=DWORD;savePreview_load.x=DWORD;savePreview_load.y=DWORD;
		#define OP_SL_PREVIEW_WH savePreview_save.width=savePreview_load.width=DWORD;savePreview_save.height=savePreview_load.height=DWORD;
		//Save/Load Buttons
		#define OP_BTN(name,isLong) string id = string(name)+"_BTN_"+to_string(DWORD);\
		                       DWORD;reg_btn[id].position.x=DWORD;reg_btn[id].position.y=DWORD;\
													 reg_btn[id].t_default=STR;reg_btn[id].t_hover=STR;reg_btn[id].t_click=STR;if (isLong) reg_btn[id].t_bg=STR;

		//Variable Stuff
		#define OP_ADD reg_dword[to_string(DWORD)]+=DWORD;
		#define OP_SUB reg_dword[to_string(DWORD)]-=DWORD;
		#define OP_SET reg_dword[to_string(DWORD)]=DWORD;

		//Jumps
		#define OP_SCENARIO uint32_t myPC=pc;runScenario(STR);pc=myPC
		#define OP_JUMP pc=scenario.offset+4+DWORD

		#define OP_JUMP_IF uint32_t a = DWORD,opr = DWORD, b = DWORD;bool cond=false;\
			switch (opr) {case 0x0: cond=(a==b); break;case 0x1: break;case 0x3: break;case 0x4:break;case 0x5: break;default: ERROR("Unknown Jump Code: "+to_string(opr));};\
			if (cond) {OP_JUMP;} else {DWORD;};

		#define op(name,code,operation) if (code==opcode) {LOG(name);operation;cout<<endl;did=true;}
			op("JUMP"            ,0x0001,OP_JUMP                              ) //ADDR
			op("UNKNOWN (0x0005)",0x0005,OP_JUMP_IF                           ) //COND. JUMP (ADDR,CMP,VALUE,JMP_TARGET)
			op("GRAPHICS"        ,0x0081,r_manager.loadArchive("graphics",STR)) //STR
			op("SOUND"           ,0x0082,r_manager.loadArchive("sound",STR)   ) //STR
			op("MUSIC"           ,0x0083,r_manager.loadArchive("music",STR)   ) //STR
			op("SCALE_DOWN"      ,0x0084,DWORD                                ) //Constant to scale down window contents(?)
			op("OPEN_WINDOW"     ,0x0085,window.init()                        ) //Init Window
			op("WINDOW_ICON"     ,0x0086,STR                                  ) //STR(ICON);Useless as SDL2_image can't load .ico files
			op("WINDOW_TITLE"    ,0x0087,window.setTitle(STR)                 ) //Set Window title
			op("TITLE_SCENARIO"  ,0x0088,reg_str["TITLE_SCENARIO"]=STR        ) //TITLE_SCENARIO
			op("TEXTBOX"         ,0x0089,OP_TEXTBOX                           )
			op("TEXTBOX_BOUNDS"  ,0x008A,OP_TEXTBOX_BOUNDS                    )
			op("CHOICEBOX"       ,0x008B,OP_CHOICEBOX                         )
			op("SAVE_COUNT"			 ,0x008C,reg_dword["SAVE_COUNT"]=DWORD        ) //SAVE_COUNT
			op("SL_PREVIEW_WH"   ,0x008D,OP_SL_PREVIEW_WH                     )
			op("NAMEBOX"         ,0x008E,OP_NAMEBOX														)
			op("NAMEBOX_BOUNDS"  ,0x008F,OP_NAMEBOX_BOUNDS										)
			op("CHOICEBOX_ON"		 ,0x0090,OP_CHOICEBOX_ON                      )
			op("UNKNOWN (0x0093)",0x0093,DWORD                                ) //Investigate
			op("UNKNOWN (0x0094)",0x0094,DWORD                                ) //Investigate
			op("SET"             ,0x0096,OP_SET                               ) //ADDR,VAL
			op("ADD"             ,0x0099,OP_ADD                               ) //ADDR,2ADD
			op("SCENARIO"				 ,0x00A3,OP_SCENARIO                          ) //Implemented;See OP_SCENARIO
			op("PLAY_BGM"        ,0x00FA,STR;DWORD                            ) //Implement (OGG;0)
			op("PLAY"            ,0x00FC,STR;DWORD;DWORD                      )
			op("PLAY_SE"         ,0x00FE,STR;DWORD;DWORD                      ) //Implement (OGG;0;1)
			op("CHOICE"          ,0x0111,STR;DWORD                            ) //Implement
			op("SHOW_CHOICES"    ,0x0112,STR                                  ) //Implement
			op("UNKNOWN (0x0116)",0x0116,DWORD                                )
			op("WIN_BTN"         ,0x0119,OP_BTN("WIN",false)                  )
			op("SUBMENU_BTN"     ,0x011A,OP_BTN("SUBMENU",false)              )
			op("CONFIG_BTN"      ,0x011B,OP_BTN("CONFIG",false)               )
			op("SAVESTATE_BTN"   ,0x011E,OP_BTN("SAVESTATE",true)             ) //Implement (NUMBER;?;X;Y;TEXTURE;TEXTURE;TEXTURE;TEXTURE)
			op("SL_PREVIEW_XY"   ,0x0123,OP_SL_PREVIEW_XY                     )
			op("SL_PAGE_BTN"     ,0x012A,OP_BTN("SL_PAGE",true)               )
			op("LOG_BTN"         ,0x012B,OP_BTN("LOG",true)                   )
			op("SLEEP"           ,0x012F,window.sleep(DWORD,DWORD)            ) //Implement (SLEEP IN MS;SKIPPABLE (BOOL))
			op("LOG_SCROLL"      ,0x015F,DWORD;DWORD;DWORD;DWORD              ) //Implement (?;SCROLL X;SCROLL Y MIN;SCROLL Y MAX)
			op("UNKNOWN (0x0161)",0x0161,DWORD                                ) //Constant (Has something to do with the log)
			op("STOP"            ,0xFFFF,LOG("Stop Signal");return            )
		#undef op

		if (did) {
			//ERROR("It worked!");
			continue;
		}

			//Not safe at all, but probably the best available option
			uint16_t p_count;
			switch (opcode) {
					case 0x0085: case 0x00CB: case 0x0102: case 0x010A: case 0x012D: case 0x012E:
					case 0x0132: case 0x0134: case 0x0135: case 0x0136: case 0x0137: case 0x0162: case 0xFFFF:
						p_count = 0; break;

					case 0x0001: case 0x0081: case 0x0082: case 0x0083: case 0x0084: case 0x0086: case 0x0087: case 0x0088: case 0x008C:
					case 0x0093: case 0x0094: case 0x0095: case 0x00A3: case 0x00FB: case 0x00FF:
					case 0x0103: case 0x0112: case 0x0116: case 0x0127: case 0x0129: case 0x0133: case 0x0138: case 0x0161:
						p_count = 1; break;

					case 0x0002: case 0x0003: case 0x008D:
					case 0x0090: case 0x0096: case 0x0098: case 0x0099: case 0x009C: case 0x009E:
					case 0x00A0: case 0x00A2: case 0x00A6: case 0x00CA: case 0x00D6: case 0x00D9: case 0x00FA:
					case 0x0104: case 0x0107: case 0x0111: case 0x011D: case 0x0121: case 0x012F: case 0x015E: case 0x0190:
						p_count = 2; break;

					case 0x00C9: case 0x00D5: case 0x00D7: case 0x00DA:
					case 0x00FC: case 0x00FE: case 0x0109: case 0x0126: case 0x0131:
						p_count = 3; 	break;

					case 0x0004: case 0x0005: case 0x0089: case 0x008A: case 0x008E: case 0x008F:
					case 0x00A4: case 0x00D4: case 0x0110: case 0x0118: case 0x0120: case 0x0123: case 0x015F: case 0x0160:
						p_count = 4; break;

					case 0x0092: p_count = 5;	break;

					case 0x008B: case 0x0100: case 0x011C: p_count = 6; break;

					case 0x00C8: case 0x00D0: case 0x010E: case 0x0113: case 0x0119:
					case 0x011A: case 0x011B: case 0x0128: p_count = 7; break;

					case 0x011E: case 0x011F: case 0x012A: case 0x012B: p_count = 8; break;

					default:
						ERROR("Unimplemented Operation: "+strInt(opcode));
				};
				WARN("Stub Operation: "+strInt(opcode)+"; Length = "+strInt(p_count));
				for (uint16_t i=0;i < p_count;i++) {
					getParameter();
				}
				cout << endl;
				if (!window.update())
				{
					ERROR("Execution halted by user");
				}
	}
	if (name == ss_name)
		ERROR("Reached end of start scenario, this shouldn't happen :/");
}

void ScenarioRunner::start() {runScenario(ss_name);}

std::string ScenarioRunner::getString(std::string name) {return reg_str[name];}
uint32_t ScenarioRunner::getDWORD(std::string name) {return reg_dword[name];}
