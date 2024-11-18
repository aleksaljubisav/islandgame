#include "gVariablesConstants.h"

int gMaxHeight = 0;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gIslandMap = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;
Texture gTextTextureLose;
Texture gTextTextureWin;
Texture gTextTextureRestart;