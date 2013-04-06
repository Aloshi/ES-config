#include "Util.h"
#include <SDL.h>

int getPlayerColor(int player)
{
	switch(player)
	{
	case 0:
		return 0x00CC00FF;
	case 1:
		return 0x0066AAFF;
	case 2:
		return 0xCCCC00FF;
	case 3:
		return 0xCC00CCFF;

	default:
		return 0xFF0000FF;
	}
}

std::string getHatDir(int value)
{
	if(value & SDL_HAT_UP)
		return "up";
	if(value & SDL_HAT_DOWN)
		return "down";
	if(value & SDL_HAT_RIGHT)
		return "right";
	if(value & SDL_HAT_LEFT)
		return "left";

	return "neutral?";
}

std::string getKeyName(int key)
{
	return SDL_GetKeyName((SDLKey)key);
}

std::string toLower(const std::string& name)
{
	std::string lowerName = name;
	for(size_t i = 0; i < name.length(); i++)
	{
		lowerName[i] = tolower(name[i]);
	}

	return lowerName;
}
