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
#include <locale.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Logger.hpp"
#include "Message.hpp"
#include "Archive.hpp"
#include "Scenario.hpp"

using namespace std;

int main()
{
	cout << "HadakaVM: Free implementation of Hadaka Shitsujis Game Engine" << endl
			 << "Copyright (C) 2015 imbatman/spycrab0" << endl
			 << "This program comes with ABSOLUTELY NO WARRANTY." << endl
			 << "Licensed under the GNU GPL v3; see LICENSE" << endl << endl;

	if (SDL_Init(SDL_INIT_EVERYTHING)==-1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM","Failed to initalize SDL2",nullptr);
		return 1;
	}
	if (IMG_Init(IMG_INIT_PNG)==-1)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM","Failed to initalize SDL2_image",nullptr);
		SDL_Quit();
		return 1;
	}

	try {
	//Simple msg test
	MessageFile msg("data/msg.dat");

	//Simple Archive test
	ArchiveFile mlink("data/mlink.dat");

	char* bgm01 = mlink.get("BGM_01.ogg");
	if (bgm01 == nullptr)
	{
		cout << "Couldn't get BGM_01.ogg" << endl;
		SDL_Quit();
		return 1;
	}

	ScenarioRunner sr("data/snr.dat");
	sr.start();

 } catch(string msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM",("Error: "+msg).c_str(),nullptr);
		SDL_Quit();
		return 1;
 } catch(char const* msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM",("Error: "+string(msg)).c_str(),nullptr);
 		SDL_Quit();
		return 1;
 }

	SDL_Quit();
	return 0;
}
