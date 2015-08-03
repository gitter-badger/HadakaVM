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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "Logger.hpp"
#include "Message.hpp"
#include "Archive.hpp"
#include "Scenario.hpp"

int main()
{
	std::cout << "HadakaVM: Free implementation of Hadaka Shitsujis Game Engine" << std::endl
			 << "Copyright (C) 2015 imbatman/spycrab0" << std::endl
			 << "This program comes with ABSOLUTELY NO WARRANTY." << std::endl
			 << "Licensed under the GNU GPL v3+; see LICENSE" << std::endl << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING)==-1) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM","Failed to initalize SDL2",nullptr);
		return 1;
	}
	if (IMG_Init(IMG_INIT_PNG)==-1) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM","Failed to initalize SDL2_image",nullptr);
		SDL_Quit();
		return 1;
	}
	if (Mix_Init(MIX_INIT_OGG)==-1) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM","Failed to initalize SDL2_image",nullptr);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,2,1024)==-1) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM","Failed to open audio",nullptr);
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	try {
	//Simple msg test
	MessageFile msg("data/msg.dat");

	//Simple Archive test
	ArchiveFile mlink("data/mlink.dat");

	uint32_t size=0;
	char* bgm01 = mlink.get("BGM_01.ogg",size);
	if (bgm01 == nullptr) {
		ERROR("ArchiveFile Test: Couldn't get BGM_01.ogg");
	}

	ScenarioRunner sr("data/snr.dat");
	sr.start();

 } catch(string msg) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM",("Error: "+msg).c_str(),nullptr);

		IMG_Quit();
		Mix_CloseAudio();
		Mix_Quit();
		SDL_Quit();
		return 1;
 } catch(char const* msg) {
	 	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"HadakaVM",("Error: "+string(msg)).c_str(),nullptr);

	 	IMG_Quit();
	 	Mix_CloseAudio();
	 	Mix_Quit();
	 	SDL_Quit();
		return 1;
 }

	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
	return 0;
}
