#include "GuiConfigureInputs.h"
#include "GuiTestConfigs.h"
#include "GuiMessage.h"
#include "GuiChangeInput.h"
#include <SDL.h>

GuiConfigureInputs::GuiConfigureInputs(Window* window, std::vector<EmulatorData*> systems) : Gui(window), 
	mImageDone("done.png"), mImageNotDone("notdone.png"), mList(window), mSystems(systems)
{
	mCurrentPlayer = 0;

	mList.setPosition(5, 40);
	populateList();
}

void GuiConfigureInputs::populateList()
{
	mList.clear();

	for(unsigned int i = 0; i < mSystems.size(); i++)
	{
		EmulatorData* system = mSystems.at(i);

		for(unsigned int k = 0; k < system->getInputCount(); k++)
		{
			InputData* inputData = system->getInput(k);

			Image* img;
			if(mWindow->getInputManager()->getInputConfig(mCurrentPlayer)->getInputByName(inputData->name).configured)
			{
				img = &mImageDone;
			}else{
				img = &mImageNotDone;
			}

			mList.addItem(inputData, inputData->name, img);
		}
	}

	mList.addItem(NULL, "TEST", NULL);
	mList.addItem(NULL, "SAVE", NULL);
	mList.addItem(NULL, "QUIT", NULL);
}

void GuiConfigureInputs::input(InputConfig* config, Input input)
{
	mList.input(config, input);

	if(input.value == 0)
		return;

	if(config->isMappedTo("b", input))
	{
		delete this;
	}

	if(config->isMappedTo("a", input))
	{
		//selected an option
		if(mList.getSelected() == NULL)
		{
			if(mList.getSelectedText() == "TEST")
			{
				mWindow->pushGui(new GuiTestConfigs(mWindow));
			}else if(mList.getSelectedText() == "SAVE")
			{
				done();
			}else if(mList.getSelectedText() == "QUIT")
			{
				//push a quit event to the SDL stack
				SDL_Event* quit = new SDL_Event();
				quit->type = SDL_QUIT;
				SDL_PushEvent(quit);
			}
		}else{
			//selected an input to change
			mWindow->pushGui(new GuiChangeInput(mWindow, config->getPlayerNum(), mList.getSelected()->name));
			mList.changeSelectedImage(&mImageDone);
		}
	}
}

void GuiConfigureInputs::done()
{
	std::vector<InputConfig*> configs;
	for(int i = 0; i < mWindow->getInputManager()->getNumDevices(); i++)
	{
		if(mWindow->getInputManager()->getInputConfig(i)->getPlayerNum() != -1)
		{
			configs.push_back(mWindow->getInputManager()->getInputConfig(i));
		}
	}

	for(unsigned int i = 0; i < mSystems.size(); i++)
	{
		EmulatorData* system = mSystems.at(i);
		system->write(configs);
	}

	std::string msg[1] = {"Done!"};
	mWindow->pushGui(new GuiMessage(mWindow, msg, 1));
}

void GuiConfigureInputs::update(int deltaTime)
{
	mList.update(deltaTime);
}

void GuiConfigureInputs::render()
{
	if(mWindow->peekGui() != this)
		return;

	Renderer::drawCenteredText("ASSIGN INPUTS", 2, 0xFF00FFFF);
	mList.render();
}
