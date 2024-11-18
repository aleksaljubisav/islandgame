#include <iostream>

#include "Texture.h"
#include "gVariablesConstants.h"

Texture::Texture()
{
	texture = nullptr;
	mWidth = 0;
	mHeight = 0;
}

void Texture::free()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

Texture::~Texture()
{
	free();
}


bool Texture::loadTextureFromRenderedText(std::string textureText, SDL_Color textColor)
{
	bool success = true;

	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		std::cout << "Unable to render text surface: " << TTF_GetError() << std::endl;
		success = false;
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (texture == NULL)
		{
			std::cout << "Unable to create texture from text surface: " << TTF_GetError() << std::endl;
			success = false;
		}
		else
		{
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return success;
}

void Texture::render(int x, int y, SDL_Rect * clip)
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(gRenderer, texture, clip, &renderQuad);

}