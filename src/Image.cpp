#include "Image.h"
#include <iostream>

extern SDL_Surface* sScreen;

Image::Image(const std::string& path) : mPath(path)
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

void Image::draw(int x, int y, bool center)
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
	SDL_BlitSurface(mImage, NULL, sScreen, &rect);
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
