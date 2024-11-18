#ifndef G_VARIABLES_CONSTANTS_H
#define G_VARIABLES_CONSTANTS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "Texture.h"

#define MATRIX_SIZE 30

const int fieldSize = 30;

extern int gMaxHeight;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 900;

extern SDL_Window* gWindow;
extern SDL_Surface* gScreenSurface;
extern SDL_Surface* gIslandMap;

extern SDL_Renderer* gRenderer;

extern TTF_Font* gFont;
extern Texture gTextTextureLose;
extern Texture gTextTextureWin;
extern Texture gTextTextureRestart;


#endif