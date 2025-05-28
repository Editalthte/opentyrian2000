#include "opentyrian.hpp"
#include "rng.hpp"

#include "SDL2/SDL.h"


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

int main(int argc, char *argv[])
{
	// Initialize RNG
	RNG rng;

	// Print version, copyright info, etc.
	{
		using std::string;
		string opentyrian_str = "OpenTyrian2000";
		string opentyrian_version = "v2.1.???";

		printf("\nWelcome to... >> %s %s <<\n\n", opentyrian_str.c_str(), opentyrian_version.c_str());

		printf("Copyright (C) 2022 The OpenTyrian Development Team\n");
		printf("Copyright (C) 2022 Kaito Sinclaire\n\n");

		printf("This program comes with ABSOLUTELY NO WARRANTY.\n");
		printf("This is free software, and you are welcome to redistribute it\n");
		printf("under certain conditions.  See the file COPYING for details.\n\n");
	}

	// Initialize SDL
	if (SDL_Init(0))
	{
		printf("Failed to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}

	// Parse any arguments/options
	{
		// Christmas check

		
	}



}