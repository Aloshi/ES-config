#ifndef _INPUTCONFIG_H_
#define _INPUTCONFIG_H_

#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include <sstream>
#include "Util.h"

#define DEVICE_KEYBOARD -1

#define LOCATION_ACCEPT "accept"
#define LOCATION_BACK "back"
#define LOCATION_UP "up"
#define LOCATION_DOWN "down"
#define LOCATION_RIGHT "right"
#define LOCATION_LEFT "left"

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

	std::string string()
	{
		if(!configured)
			return "";

		std::stringstream stream;
		switch(type)
		{
			case TYPE_BUTTON:
				stream << "Button " << id;
				break;
			case TYPE_AXIS:
				stream << "Axis " << id << (value > 0 ? "+" : "-");
				break;
			case TYPE_HAT:
				stream << "Hat " << id << " " << getHatDir(value);
				break;
			case TYPE_KEY:
				stream << "Key " << SDL_GetKeyName((SDLKey)id);
				break;
			default:
				stream << "Input to string error";
				break;
		}

		return stream.str();
	}
};

class InputConfig
{
public:
	InputConfig(int deviceId);

	void clear();
	void mapInput(const std::string& location, Input input);
	void setPlayerNum(int num);

	int getPlayerNum();
	int getDeviceId();

	//Returns the input mapped to this location.
	Input getInputByLocation(const std::string& location);

	//Returns true if Input is mapped to this location, false otherwise.
	bool isMappedTo(const std::string& location, Input input);

	//Returns a list of locations this input is mapped to.
	std::vector<std::string> getMappedTo(Input input);

private:
	std::map<std::string, Input> mLocationMap;
	const int mDeviceId;
	int mPlayerNum;
};

#endif
