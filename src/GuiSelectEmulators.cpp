#include "GuiSelectEmulators.h"
#include "Window.h"
#include "GuiConfigureInputs.h"

GuiSelectEmulators::GuiSelectEmulators(Window* window) : Gui(window), mUncheckedImage("unchecked.png"), mCheckedImage("checked.png"), mList(window)
{
	std::cout << "Starting GuiSelectEmulators\n";
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

void GuiSelectEmulators::loadSystemConfigs()
{
	clearSystems();

	//this needs to autopopulate somehow
	mSystems.push_back(new EmulatorData("retroarch_config.xml", mWindow->getScriptEngine()));
	mSystems.push_back(new EmulatorData("dgen_config.xml", mWindow->getScriptEngine()));

	if(mSystems.size() == 0)
	{
		std::cout << "WARNING: GuiSelectEmulator - no emulators found!\n";
	}

	populateList();
}

void GuiSelectEmulators::populateList()
{
	mList.clear();

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

	if(input.value != 0 && config->isMappedTo("a", input))
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
			//only one is DONE right now
			done();
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
