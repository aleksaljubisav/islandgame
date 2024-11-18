#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "gVariablesConstants.h"
#include "helpers.h"


bool init()
{
	bool success = true;

	// Initialize
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL initialization error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		// Initialize TTF
		if (TTF_Init() == -1)
		{
			std::cout << "TTF initialization error: " << TTF_GetError() << std::endl;
			success = false;
		}
		else
		{

			// Create window
			gWindow = SDL_CreateWindow("Nordeus Challenge - Island Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (gWindow == NULL)
			{
				std::cout << "Error when creating window: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
				if (gRenderer == NULL)
				{
					std::cout << "Error when creating renderer: " << SDL_GetError() << std::endl;
					success = false;
				}

				// Get window surface
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}

	}

	return success;
}

bool loadMedia()
{
	bool success = true;
	

	gFont = TTF_OpenFont("fonts/gameFont.ttf", 28);
	if (gFont == NULL)
	{
		std::cout << "Failed to load ttf font: " << TTF_GetError() << std::endl;
		success = false;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0 };
		if (!gTextTextureLose.loadTextureFromRenderedText("You failed to find the highest island.", textColor))
		{
			std::cout << "Failed to render text" << std::endl;
			success = false;
		}
		if(!gTextTextureWin.loadTextureFromRenderedText("You found the highest island!", textColor))
		{
			std::cout << "Failed to render text" << std::endl;
			success = false;
		}
		if (!gTextTextureRestart.loadTextureFromRenderedText("Press R to restart.", textColor))
		{
			std::cout << "Failed to render text" << std::endl;
			success = false;
		}

	}

	return success;
}

void close()
{
	gTextTextureLose.free();
	gTextTextureWin.free();
	gTextTextureRestart.free();

	TTF_CloseFont(gFont);
	gFont = NULL;


	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	TTF_Quit();
	SDL_Quit();
}