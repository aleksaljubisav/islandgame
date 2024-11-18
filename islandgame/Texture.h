#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SDL.h>

class Texture
{
public:
	Texture();

	~Texture();

	bool loadTextureFromRenderedText(std::string textureText, SDL_Color textColor);

	void free();

	void render(int x, int y, SDL_Rect* clip = NULL); //, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }


private:
	SDL_Texture* texture;
	
	int mWidth;
	int mHeight;
};

#endif