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
#include <map>

#include "Scenario.hpp"
#include "Logger.hpp"
#include "BinaryIO.hpp"
#include "Operation.hpp"

ScenarioRunner::~ScenarioRunner()
{ delete[] buffer; }

ScenarioRunner::ScenarioRunner(std::string path) : file(path)
{
	uint32_t hs_files,hs_total,ss_size;
	hs_files=0;
	hs_total=0;

	fsize = bintools::filesize(path);
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
	ss_size = *reinterpret_cast<uint16_t*>(&buffer[0x12]); //Read start scenario name

	if (hs_files > hs_total)
		ERROR("Value error (hs_files > hs_total)");
	if (fsize < hs_total)
		ERROR("File to small (fsize < hs_total)");

	if (fsize < 0x18+ss_size) //Filesize check
		ERROR("File to small (fsize < 0x18+ss_size)");

	for (uint32_t i = 0;i < ss_size;i++)
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
		case p_type::DWORD_5:
		case p_type::DWORD_6:
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
			ERROR("Unknown Parametercode; This indicates a parsing error");
		break;
	}
	return p;
}

void ScenarioRunner::runScenario(std::string name)
{
	Scenario scenario;
	for (Scenario s : scenarios) if (s.name == name) scenario = s;
	if (scenario.name=="")
			ERROR("Unknown scenario '"+name+"'");

	c_scenario = scenario;

	uint32_t scenarioSize = *reinterpret_cast<uint32_t*>(&buffer[scenario.offset]);

	LOG("Parsing scenario '"+scenario.name+"'");

	for (pc = scenario.offset+4;pc < scenario.offset+4+scenarioSize;) {
		uint16_t opcode = *reinterpret_cast<uint16_t*>(&buffer[pc]);
		pc+=2;

		std::map<uint16_t,void(*)(ScenarioRunner* sr)> opcodes;

		opcodes[0x0001] = Operation::jump;
		opcodes[0x0005] = Operation::jump_conditinal;
		opcodes[0x0081] = Operation::archive_graphics;
		opcodes[0x0082] = Operation::archive_music;
		opcodes[0x0083] = Operation::archive_sound;
		opcodes[0x0084] = Operation::scale_down;
		opcodes[0x0085] = Operation::window_open;
		opcodes[0x0086] = Operation::window_icon;
		opcodes[0x0087] = Operation::window_title;
		opcodes[0x0088] = Operation::scenario_title_set;
		opcodes[0x0096] = Operation::set;
		opcodes[0x0099] = Operation::add;
		opcodes[0x00A3] = Operation::scenario_enter;
		opcodes[0x00FA] = Operation::play_bgm;
		opcodes[0x00FC] = Operation::play_uncategorized;
		opcodes[0x00FE] = Operation::play_sfx;
		opcodes[0x0111] = Operation::choice_add;
		opcodes[0x0112] = Operation::choices_show;
		opcodes[0x012F] = Operation::sleep;
		opcodes[0xFFFF] = Operation::stop;

		if (opcodes.find(opcode)!=opcodes.end())
		{
			LOG("OPC/ARR: "+strInt(opcode));
			opcodes[opcode](this);
			continue;
		}
		else
		{
			LOG("Couldn't find OPCODE;CONT.");
		}

			uint16_t p_count;
			switch (opcode) {
					case 0x0085: case 0x00CB: case 0x0102: case 0x010A: case 0x012D: case 0x012E:
					case 0x0134: case 0x0135: case 0x0136: case 0x0137: case 0x0162: case 0xFFFF:
					case 0x0132:
						p_count = 0; break;

					case 0x0001: case 0x0081: case 0x0082: case 0x0083: case 0x0084: case 0x0086:
					case 0x0087: case 0x0088: case 0x008C: case 0x0093: case 0x0094: case 0x0095:
					case 0x00A3: case 0x00FB: case 0x00FF: case 0x0103: case 0x0112: case 0x0116:
					case 0x0127: case 0x0129: case 0x0133: case 0x0138: case 0x0161:
						p_count = 1; break;

					case 0x0002: case 0x0003: case 0x008D: case 0x0090: case 0x0096: case 0x0098:
					case 0x0099: case 0x009C: case 0x009E: case 0x00A0: case 0x00A2: case 0x00A6:
					case 0x00CA: case 0x00D6: case 0x00D9: case 0x00FA: case 0x0104: case 0x0107:
					case 0x0111: case 0x011D: case 0x0121: case 0x012F: case 0x015E: case 0x0190:
						p_count = 2; break;

					case 0x00C9: case 0x00D5: case 0x00D7: case 0x00DA: case 0x00FC: case 0x00FE:
					case 0x0109: case 0x0126: case 0x0131:
					 p_count = 3; 	break;

					case 0x0004: case 0x0005: case 0x0089: case 0x008A: case 0x008E: case 0x008F:
					case 0x00A4: case 0x00D4: case 0x0110: case 0x0118: case 0x0120: case 0x0123:
					case 0x015F: case 0x0160:
					 p_count = 4; break;

					case 0x0092:
					 p_count = 5;	break;

					case 0x008B: case 0x0100: case 0x011C:
					 p_count = 6; break;

					case 0x00C8: case 0x00D0: case 0x010E: case 0x0113: case 0x0119: case 0x011A:
					case 0x011B: case 0x0128:
					 p_count = 7; break;

					case 0x011E: case 0x011F: case 0x012A: case 0x012B:
					 p_count = 8; break;

					default:
						ERROR("Unimplemented Operation: "+strInt(opcode));
				};
				WARN("Stub Operation: "+strInt(opcode)+"; Length = "+strInt(p_count));
				for (uint16_t i=0;i < p_count;i++) {
					getParameter();
				}
				cout << endl;
				if (window.update().type==UpdateEventType::EXIT)
					Operation::exit(this);
	}
	if (name == ss_name)
		ERROR("Reached end of start scenario, this shouldn't happen :/");
}

void ScenarioRunner::start() {runScenario(ss_name);}

std::string ScenarioRunner::getString() {return getParameter().value.string;}
uint32_t ScenarioRunner::getDWORD() {return getParameter().value.dword;}

ResourceManager* ScenarioRunner::getResourceManager() {return &r_manager;}
Window* ScenarioRunner::getWindow() {return &window;}

void ScenarioRunner::setPC(uint32_t pc) {this->pc = pc;}
uint32_t ScenarioRunner::getPC() {return pc;}

Scenario* ScenarioRunner::getScenario() {return &c_scenario;}
void ScenarioRunner::setScenario(Scenario s) {c_scenario=s;}

void ScenarioRunner::setTitleScenario(std::string s) {reg_str["TITLE_SCENARIO"]=s;}

void ScenarioRunner::setLogicVar(uint32_t key,uint32_t value) {reg_logic[key] = value;}
uint32_t ScenarioRunner::getLogicVar(uint32_t key) {return reg_logic[key];}

void ScenarioRunner::exit() {}
