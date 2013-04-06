#ifndef _GUICHANGEINPUT_H_
#define _GUICHANGEINPUT_H_

#include "Gui.h"
#include "Window.h"
#include "EmulatorData.h"

class GuiConfigureInputs;

class GuiChangeInput : public Gui
{
public:
	GuiChangeInput(Window* window, int playerNum, InputData* inputData, GuiConfigureInputs* updateOnDone);

	void input(InputConfig* config, Input input);
	void update(int deltaTime) { };
	void render();

private:
	GuiConfigureInputs* mUpdateOnDone;
	int mPlayerNum;
	InputData* mInputData;
};

#endif