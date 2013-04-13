#include "Renderer.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

extern std::string RESOURCE_PREFIX;

static int sScreenWidth = 0;
static int sScreenHeight = 0;
static int sFontSize = 0;
SDL_Surface* sScreen = NULL; //not static because used by the current Image class implementation

static TTF_Font* sFont = NULL;

#define FONT_NAME "font.ttf"

void Renderer::init()
{
	sScreen = SDL_SetVideoMode(800, 600, 16, SDL_SWSURFACE | SDL_ANYFORMAT);
	sScreenWidth = sScreen->w;
	sScreenHeight = sScreen->h;

	if(sFont != NULL)
		TTF_CloseFont(sFont);

	sFont = TTF_OpenFont((RESOURCE_PREFIX + FONT_NAME).c_str(), (int)(sScreenHeight * 0.04));

	if(sFont == NULL)
	{
		std::cout << "Could not load font! Prepare for crash!\n";
	}
}

int Renderer::getFontSize()
{
	if(sFontSize == 0)
	{
		TTF_SizeText(sFont, "TEST", NULL, &sFontSize);
	}

	return sFontSize;
}

void Renderer::sizeText(const std::string& text, int* w, int* h)
{
	TTF_SizeText(sFont, text.c_str(), w, h);
}

void Renderer::deinit()
{
	SDL_FreeSurface(sScreen);
}

int Renderer::getWidth()
{
	return sScreenWidth;
}

int Renderer::getHeight()
{
	return sScreenHeight;
}

void Renderer::clear()
{
	SDL_FillRect(sScreen, NULL, SDL_MapRGBA(sScreen->format, 242, 242, 242, 0));
}

void Renderer::swapBuffers()
{
	SDL_Flip(sScreen);
}

void Renderer::drawRect(int x, int y, int w, int h, int color)
{
	SDL_Rect rect;
	rect.x = x; rect.y = y; rect.w = w; rect.h = h;
	int sdlColor = SDL_MapRGBA(sScreen->format, (color & 0xFF000000) / 0x1000000, (color & 0x00FF0000) / 0x10000, 
		(color & 0x0000FF00) / 0x100, (color & 0x000000FF));

	SDL_FillRect(sScreen, &rect, sdlColor);
}

SDL_Color intToSDLColor(int color)
{
	SDL_Color sdlc;
	sdlc.r = (color & 0xFF000000) / 0x1000000;
	sdlc.g = (color & 0x00FF0000) / 0x10000;
	sdlc.b = (color & 0x0000FF00) / 0x100;
	sdlc.unused = (color & 0x000000FF);

	return sdlc;
}

void Renderer::drawText(const std::string& text, int x, int y, int color)
{
	if(text.length() == 0)
		return;

	SDL_Color sdlc = intToSDLColor(color);
	SDL_Surface* src = TTF_RenderText_Solid(sFont, text.c_str(), sdlc);

	if(src == NULL)
	{
		std::cout << "Text render error:   " << TTF_GetError() << "\n";
		return;
	}

	SDL_Rect rect;
	rect.x = x; rect.y = y;
	rect.w = src->w; rect.h = src->h;

	SDL_BlitSurface(src, NULL, sScreen, &rect);
	SDL_FreeSurface(src);
}

void Renderer::drawCenteredText(const std::string& text, int y, int color)
{
	if(text.length() == 0)
		return;

	SDL_Color sdlc = intToSDLColor(color);
	SDL_Surface* src = TTF_RenderText_Solid(sFont, text.c_str(), sdlc);

	if(src == NULL)
	{
		std::cout << "Text render error:   " << TTF_GetError() << "\n";
		return;
	}

	int x = (sScreenWidth - src->w) / 2;

	SDL_Rect rect;
	rect.x = x; rect.y = y;
	rect.w = src->w; rect.h = src->h;

	SDL_BlitSurface(src, NULL, sScreen, &rect);
	SDL_FreeSurface(src);
}
