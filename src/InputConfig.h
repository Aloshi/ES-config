#ifndef _INPUTCONFIG_H_
#define _INPUTCONFIG_H_

#include <map>
#include <vector>
#include <string>
#include <SDL.h>

#define DEVICE_KEYBOARD -1

enum InputType
{
	TYPE_AXIS,
	TYPE_BUTTON,
	TYPE_HAT,
	TYPE_KEY,
	TYPE_COUNT
};

struct Input
{
public:
	int device;
	InputType type;
	int id;
	int value;
	bool configured;

	Input()
	{
		device = DEVICE_KEYBOARD;
		configured = false;
		id = -1;
		value = -999;
		type = TYPE_COUNT;
	}

	Input(int dev, InputType t, int i, int val, bool conf) : device(dev), type(t), id(i), value(val), configured(conf)
	{
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

	std::string string()
	{
		if(!configured)
			return "";

		std::string str;
		switch(type)
		{
			case TYPE_BUTTON:
				str = "Button ";
				str += id;
				break;
			case TYPE_AXIS:
				str = "Axis ";
				str += id;
				str += value > 0 ? "+" : "-";
				break;
			case TYPE_HAT:
				str = "Hat ";
				str += id + " " + getHatDir(value);
				break;
			case TYPE_KEY: //might want to use SDL_GetKeyName(SDLKey key), not sure if we can, will return gibberish if val < 0 (non-alphanum key)
				str = "Key ";
				str += (char)value;
			default:
				str = "Input to string error";
				break;
		}

		return str;
	}
};

class InputConfig
{
public:
	InputConfig(int deviceId);

	void setInput(const std::string& name, Input input);
	void setPlayerNum(int num);

	int getPlayerNum();
	int getDeviceId();
	Input getInputByName(const std::string& name);

	bool isMappedTo(const std::string& name, Input input);
	std::vector<std::string> getMappedTo(Input input);

private:
	std::map<std::string, Input> mInputMap;
	int mDeviceId;
	int mPlayerNum;
};

#endif
