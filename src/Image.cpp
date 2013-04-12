#include "Image.h"
#include <iostream>

extern SDL_Surface* sScreen;
extern std::string RESOURCE_PREFIX;

Image::Image(const std::string& path) : mPath(RESOURCE_PREFIX + path)
{
	mImage = NULL;
	load();
}

Image::~Image()
{
	if(mImage != NULL)
	{
		SDL_FreeSurface(mImage);
	}
}

void Image::load()
{
	if(mImage != NULL)
		SDL_FreeSurface(mImage);

	mImage = IMG_Load(mPath.c_str());
	if(mImage == NULL)
	{
		std::cout << "Error loading image " << mPath << "!\n";
		std::cout << "   " << IMG_GetError() << "\n";
	}
}

void Image::draw(int x, int y, bool center, char opacity)
{
	if(mImage == NULL)
		return;

	if(center)
	{
		x -= getWidth() / 2;
		y -= getHeight() / 2;
	}

	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = mImage->w; rect.h = mImage->h;
	SDL_SetAlpha(mImage, SDL_SRCALPHA, opacity);
	SDL_BlitSurface(mImage, NULL, sScreen, &rect);
}

void Image::drawSection(int sx, int sy, int sw, int sh, int x, int y, char opacity)
{
	if(mImage == NULL)
		return;

	SDL_Rect src, dest;
	src.x = sx; src.y = sy; src.w = sw; src.h = sh;
	dest.x = x; dest.y = y; dest.w = sw; dest.h = sh;
	SDL_SetAlpha(mImage, SDL_SRCALPHA, opacity);
	SDL_BlitSurface(mImage, &src, sScreen, &dest);
}

int Image::getWidth()
{
	if(mImage == NULL)
		return 0;

	return mImage->w;
}

int Image::getHeight()
{
	if(mImage == NULL)
		return 0;

	return mImage->h;
}
