#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include <SDL.h>
#include <vector>
#include <map>

class InputConfig;
class Window;

//you should only ever instantiate one of these, by the way
class InputManager
{
public:
	InputManager(Window* window);
	~InputManager();

	void init();
	void deinit();

	int getNumDevices();

	void parseEvent(const SDL_Event& ev);

	InputConfig* getInputConfig(int device);

private:
	static const int DEADZONE = 23000;

	Window* mWindow;

	int mNumJoysticks;
	SDL_Joystick** mJoysticks;
	InputConfig** mInputConfigs;
	InputConfig* mKeyboardInputConfig;
	std::map<int, int>* mPrevAxisValues;
};

#endif
