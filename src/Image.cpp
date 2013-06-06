#include "Image.h"
#include <iostream>
#include "Settings.h"

extern SDL_Surface* sScreen;

Image::Image(const std::string& path) : mPath(Settings::getInstance()->getResourceDir() + path)
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

	SDL_Surface* temp = SDL_DisplayFormatAlpha(mImage);
	SDL_FreeSurface(mImage);
	mImage = temp;
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

void Image::drawSection(int sx, int sy, int sw, int sh, int x, int y)
{
	if(mImage == NULL)
		return;

	SDL_Rect src, dest;
	src.x = sx; src.y = sy; src.w = sw; src.h = sh;
	dest.x = x; dest.y = y; dest.w = sw; dest.h = sh;
	SDL_BlitSurface(mImage, &src, sScreen, &dest);
}

void Image::setAlpha(char alpha)
{
	Uint8* pixels = (Uint8*) mImage->pixels;
	const Uint8 bpp = mImage->format->BytesPerPixel;

	const Uint32 formatAlpha = (alpha >> mImage->format->Aloss) << mImage->format->Ashift;

	Uint16* p16;
	Uint32* p32;
	for(int i = 0; i < mImage->w * mImage->h * bpp; i += bpp)
	{
		switch(bpp)
		{
		case 2:
			p16 = ((Uint16*)&pixels[i]);
			*p16 = (*p16 & ~mImage->format->Amask) | formatAlpha;
			break;
		case 3:
		case 4:
			p32 = ((Uint32*)&pixels[i]);

			if((*p32 & mImage->format->Amask) != 0)
				*p32 = (*p32 & ~mImage->format->Amask) | formatAlpha;
			break;
		default:
			std::cout << "Can't set alpha for unsupported BPP\n";
			return;
		}

		
	}
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
