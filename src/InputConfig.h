#ifndef _INPUTCONFIG_H_
#define _INPUTCONFIG_H_

#include <map>
#include <vector>

enum InputType
{
	TYPE_AXIS,
	TYPE_BUTTON,
	TYPE_HAT,
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
		device = -1;
		configured = false;
		id = -1;
		value = -999;
		type = TYPE_COUNT;
	}

	Input(int dev, InputType t, int i, int val, bool conf) : device(dev), type(t), id(i), value(val), configured(conf)
	{
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