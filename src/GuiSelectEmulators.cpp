#include "GuiSelectEmulators.h"
#include "Window.h"
#include "GuiConfigureInputs.h"
#include <boost/filesystem.hpp>

extern std::string FORCED_SCRIPT_DIRECTORY;
extern std::string RESOURCE_PREFIX;

namespace fs = boost::filesystem;

GuiSelectEmulators::GuiSelectEmulators(Window* window) : Gui(window), mUncheckedImage("unchecked.png"), mCheckedImage("checked.png"), mList(window)
{
	mList.setPosition(0, 40);

	loadSystemConfigs();
}

GuiSelectEmulators::~GuiSelectEmulators()
{
	clearSystems();
}

void GuiSelectEmulators::clearSystems()
{
	for(unsigned int i = 0; i < mSystems.size(); i++)
		delete mSystems.at(i);
	mSystems.clear();
}

//recursive search directory
void GuiSelectEmulators::searchDirForConfigs(const std::string& path, const char* matchExtension, bool recurse)
{
	for(fs::directory_iterator end, dir(path); dir != end; ++dir)
	{
		fs::path filePath = (*dir).path();

		if(fs::is_directory(*dir) && recurse)
		{
			searchDirForConfigs(filePath.string(), matchExtension);
		}

		if(filePath.extension() == matchExtension)
		{
			EmulatorData* sys = new EmulatorData(filePath.string(), mWindow->getScriptEngine());
			if(sys->isLoaded())
				mSystems.push_back(sys);
			else
				delete sys;
		}
	}
}

void GuiSelectEmulators::loadSystemConfigs()
{
	clearSystems();

	const int pathCount = 2;
	std::string path[pathCount] = { "scripts/", FORCED_SCRIPT_DIRECTORY };

	for(int i = 0; i < pathCount; i++)
	{
		if(path[i].empty() || !fs::is_directory(path[i]))
			continue;

		searchDirForConfigs(path[i], ".xml", true);
	}

	if(mSystems.size() == 0)
	{
		std::cout << "WARNING: GuiSelectEmulator - no emulators found!\n";
	}

	populateList();
}

void GuiSelectEmulators::populateList()
{
	mList.clear();

	if(mSystems.size() == 0)
	{
		mList.addItem(NULL, "No emulator scripts found!", NULL);
	}

	for(unsigned int i = 0; i < mSystems.size(); i++)
	{
		mList.addItem(mSystems.at(i), mSystems.at(i)->getName(), &mUncheckedImage);
	}

	mList.addItem(NULL, "NEXT", NULL);
}

void GuiSelectEmulators::done()
{
	//create list of selected systems
	std::vector<EmulatorData*> systems;
	for(int i = 0; i < mList.getLength(); i++)
	{
		if(mList.getImage(i) == &mCheckedImage)
		{
			systems.push_back(mList.getItem(i));
		}
	}

	if(systems.size() == 0)
		return;

	mWindow->pushGui(new GuiConfigureInputs(mWindow, systems));
}

void GuiSelectEmulators::input(InputConfig* config, Input input)
{
	mList.input(config, input);

	if(input.value != 0 && config->isMappedTo(LOCATION_ACCEPT, input))
	{
		if(mList.getSelected() != NULL)
		{
			//selected an emulator to toggle
			if(mList.getSelectedImage() == &mUncheckedImage)
				mList.changeSelectedImage(&mCheckedImage);
			else
				mList.changeSelectedImage(&mUncheckedImage);
		}else{
			//selected an option
			if(mList.getSelectedText() == "NEXT")
			{
				done();
			}
		}
	}
}

void GuiSelectEmulators::update(int deltaTime)
{
	mList.update(deltaTime);
}

void GuiSelectEmulators::render()
{
	//don't bother rendering if we're not on top; nothing shows through to us
	if(mWindow->peekGui() != this)
		return;

	Renderer::drawCenteredText("SELECT EMULATORS", 2, 0xFF00FFFF);
	mList.render();
}
