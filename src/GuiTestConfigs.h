#ifndef _GUITESTCONFIGS_H_
#define _GUITESTCONFIGS_H_

#include "Gui.h"
#include "Window.h"
#include <string>
#include "EmulatorData.h"

#define HOLD_BACK_TIME 1500

class GuiTestConfigs : public Gui
{
public:
	GuiTestConfigs(Window* window, std::vector<EmulatorData*> systems);
	~GuiTestConfigs();

	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();

private:
	std::vector<std::string>** mPressed;
	std::vector<EmulatorData*> mSystems;

	std::vector<std::string> getNamesFromLocation(const std::string& loc);

	int mBackTimer;
	bool mHoldingBack;
};

#endif