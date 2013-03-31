#ifndef _GUISELECTEMULATORS_H_
#define _GUISELECTEMULATORS_H_

#include "Gui.h"
#include "SymbolList.h"
#include "EmulatorData.h"
#include <iostream>

class GuiSelectEmulators : public Gui
{
public:
	GuiSelectEmulators(Window* window);
	~GuiSelectEmulators();

	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();

private:
	void clearSystems();
	void searchDirForConfigs(const std::string& path, const char* matchExtension, bool recurse = true);
	void loadSystemConfigs();
	void populateList();

	void done();

	std::vector<EmulatorData*> mSystems;

	Image mUncheckedImage;
	Image mCheckedImage;

	SymbolList<EmulatorData*> mList;
};

#endif
