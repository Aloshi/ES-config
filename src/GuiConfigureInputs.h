#ifndef _GUICONFIGUREINPUTS_H_
#define _GUICONFIGUREINPUTS_H_

#include "Gui.h"
#include "Window.h"
#include <vector>
#include "EmulatorData.h"
#include "SymbolList.h"

class GuiConfigureInputs : public Gui
{
public:
	GuiConfigureInputs(Window* window, std::vector<EmulatorData*> systems);

	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();

private:
	Image mImageDone;
	Image mImageNotDone;

	SymbolList<InputData*> mInputList;
	SymbolList<Input*> mMappedList;

	void populateList();

	void done();

	std::vector<EmulatorData*> mSystems;
	int mCurrentPlayer;
};

#endif
