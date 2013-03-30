#ifndef _GUIBASICCONFIG_H_
#define _GUIBASICCONFIG_H_

#include "Gui.h"

class GuiBasicConfig : public Gui
{
public:
	GuiBasicConfig(Window* window, int device);

	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();
private:
	int mDevice;
	int mCurInputId;
};

#endif
