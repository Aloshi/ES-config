#include "GuiConfigureInputs.h"
#include "GuiTestConfigs.h"
#include "GuiMessage.h"
#include "GuiChangeInput.h"
#include <SDL.h>
#include "Util.h"

GuiConfigureInputs::GuiConfigureInputs(Window* window, std::vector<EmulatorData*> systems) : Gui(window), 
	mImageDone("done.png"), mImageNotDone("notdone.png"), mInputList(window), mMappedList(window), mSystems(systems)
{
	mCurrentPlayer = 0;

	mInputList.setPosition(5, 40);
	mMappedList.setPosition(Renderer::getWidth() / 2 - 5, 40);

	populateList();
}

void GuiConfigureInputs::populateList()
{
	int restoreIndex = mInputList.getSelectedIndex();

	mInputList.clear();
	mMappedList.clear();

	InputConfig* config = mWindow->getInputManager()->getInputConfigByPlayer(mCurrentPlayer);

	for(unsigned int i = 0; i < mSystems.size(); i++)
	{
		EmulatorData* system = mSystems.at(i);

		for(unsigned int k = 0; k < system->getInputCount(); k++)
		{
			InputData* inputData = system->getInput(k);

			Image* img;
			if(config->getInputByName(inputData->name).configured)
			{
				img = &mImageDone;
			}else{
				img = &mImageNotDone;
			}

			mInputList.addItem(inputData, inputData->name, img);
			mMappedList.addItem(NULL, config->getInputByName(inputData->name).string(), NULL);
		}
	}

	mInputList.addItem(NULL, "TEST", NULL);
	mInputList.addItem(NULL, "SAVE", NULL);
	mInputList.addItem(NULL, "QUIT", NULL);

	mMappedList.addItem(NULL, "", NULL);
	mMappedList.addItem(NULL, "", NULL);
	mMappedList.addItem(NULL, "", NULL);

	mInputList.setCursorColor(getPlayerColor(mCurrentPlayer));
	mMappedList.setCursorColor(getPlayerColor(mCurrentPlayer));

	mInputList.setSelectedIndex(restoreIndex);
	mMappedList.setSelectedIndex(restoreIndex);
}

void GuiConfigureInputs::updateList()
{
	InputConfig* curCfg = mWindow->getInputManager()->getInputConfigByPlayer(mCurrentPlayer);

	mMappedList.changeSelectedText(curCfg->getInputByName(mInputList.getSelected()->name).string());

	if(curCfg->getInputByName(mInputList.getSelected()->name).configured)
		mInputList.changeSelectedImage(&mImageDone);
	else
		mInputList.changeSelectedImage(&mImageNotDone);
}

void GuiConfigureInputs::input(InputConfig* config, Input input)
{
	if(config->getPlayerNum() != -1 && config->getPlayerNum() != mCurrentPlayer)
	{
		mCurrentPlayer = config->getPlayerNum();
		populateList();
	}

	mInputList.input(config, input);
	mMappedList.input(config, input);

	if(input.value == 0)
		return;

	if(config->isMappedTo("b", input))
	{
		delete this;
	}

	if(config->isMappedTo("a", input))
	{
		//selected an option
		if(mInputList.getSelected() == NULL)
		{
			if(mInputList.getSelectedText() == "TEST")
			{
				mWindow->pushGui(new GuiTestConfigs(mWindow));
			}else if(mInputList.getSelectedText() == "SAVE")
			{
				done();
			}else if(mInputList.getSelectedText() == "QUIT")
			{
				//push a quit event to the SDL stack
				SDL_Event* quit = new SDL_Event();
				quit->type = SDL_QUIT;
				SDL_PushEvent(quit);
			}
		}else{
			//selected an input to change
			mWindow->pushGui(new GuiChangeInput(mWindow, config->getPlayerNum(), mInputList.getSelected()->name, this));
		}
	}
}

void GuiConfigureInputs::done()
{
	std::vector<InputConfig*> configs;
	for(int i = 0; i < mWindow->getInputManager()->getNumPlayers(); i++)
	{
		InputConfig* cfg = mWindow->getInputManager()->getInputConfigByPlayer(i);
		
		if(cfg->getPlayerNum() != -1)
			configs.push_back(cfg);
	}


	bool success = true;
	for(unsigned int i = 0; i < mSystems.size(); i++)
	{
		EmulatorData* system = mSystems.at(i);
		success = system->write(configs);

		if(!success)
			break;
	}

	std::string msg[1];
	if(success)
		msg[0] = "Done!";
	else
		msg[0] = "Error!!";

	mWindow->pushGui(new GuiMessage(mWindow, msg, 1));
}

void GuiConfigureInputs::update(int deltaTime)
{
	mInputList.update(deltaTime);
	mMappedList.update(deltaTime);
}

void GuiConfigureInputs::render()
{
	if(mWindow->peekGui() != this)
		return;

	Renderer::drawCenteredText("ASSIGN INPUTS", 2, 0xFF00FFFF);
	mInputList.render();
	mMappedList.render();
}
