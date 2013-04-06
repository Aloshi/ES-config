#ifndef _GUIBASICCONFIG_H_
#define _GUIBASICCONFIG_H_

#include "Gui.h"
#include <string>
#include "Image.h"

class GuiBasicConfig : public Gui
{
public:
	GuiBasicConfig(Window* window, InputConfig* target);

	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();

private:
	std::string mErrorMsg;
	InputConfig* mTargetConfig;
	int mCurInputId;
	Image mCheckedImage;
};

#endif
