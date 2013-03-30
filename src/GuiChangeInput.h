#ifndef _GUICHANGEINPUT_H_
#define _GUICHANGEINPUT_H_

#include "Gui.h"
#include "Window.h"

class GuiChangeInput : public Gui
{
public:
	GuiChangeInput(Window* window, int playerNum, const std::string& inputName);

	void input(InputConfig* config, Input input);
	void update(int deltaTime) { };
	void render();

private:
	int mPlayerNum;
	std::string mInputName;
};

#endif