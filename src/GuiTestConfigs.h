#ifndef _GUITESTCONFIGS_H_
#define _GUITESTCONFIGS_H_

#include "Gui.h"
#include "Window.h"
#include <string>

#define HOLD_BACK_TIME 1500

class GuiTestConfigs : public Gui
{
public:
	GuiTestConfigs(Window* window);

	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();

private:
	std::vector<std::string> mPressed;

	int mBackTimer;
	bool mHoldingBack;
};

#endif