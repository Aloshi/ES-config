#ifndef _GUICHANGEINPUT_H_
#define _GUICHANGEINPUT_H_

#include "Gui.h"
#include "Window.h"

class GuiConfigureInputs;

class GuiChangeInput : public Gui
{
public:
	GuiChangeInput(Window* window, int playerNum, const std::string& inputName, GuiConfigureInputs* updateOnDone);

	void input(InputConfig* config, Input input);
	void update(int deltaTime) { };
	void render();

private:
	GuiConfigureInputs* mUpdateOnDone;
	int mPlayerNum;
	std::string mInputName;
};

#endif