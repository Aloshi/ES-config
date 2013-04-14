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

			//need to check for duplicates here
			int dupeLocation = -1;
			for(int j = 0; j < mInputList.getLength(); j++)
			{
				if(toLower(mInputList.getItem(j)->location) == toLower(inputData->location))
				{
					dupeLocation = j;
					break;
				}
			}

			if(dupeLocation != -1)
			{
				if(mInputList.getText(dupeLocation).find(inputData->name) == std::string::npos)
					mInputList.changeText(dupeLocation, mInputList.getText(dupeLocation) + "/" + inputData->name);
				continue;
			}

			Image* img;
			if(config->getInputByLocation(inputData->location).configured)
			{
				img = &mImageDone;
			}else{
				img = &mImageNotDone;
			}

			mInputList.addItem(inputData, inputData->name, img);
			mMappedList.addItem(NULL, config->getInputByLocation(inputData->location).string(), NULL);
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

	mMappedList.changeSelectedText(curCfg->getInputByLocation(mInputList.getSelected()->location).string());

	if(curCfg->getInputByLocation(mInputList.getSelected()->location).configured)
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

	if(config->isMappedTo(LOCATION_BACK, input))
	{
		delete this;
	}

	if(config->isMappedTo(LOCATION_ACCEPT, input))
	{
		//selected an option
		if(mInputList.getSelected() == NULL)
		{
			if(mInputList.getSelectedText() == "TEST")
			{
				mWindow->pushGui(new GuiTestConfigs(mWindow, mSystems));
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
			mWindow->pushGui(new GuiChangeInput(mWindow, config->getPlayerNum(), mInputList.getSelected(), this));
		}
	}
}

void GuiConfigureInputs::done()
{
	std::string msg[1];

	std::vector<InputConfig*> configs;
	for(int i = 0; i < mWindow->getInputManager()->getNumPlayers(); i++)
	{
		InputConfig* cfg = mWindow->getInputManager()->getInputConfigByPlayer(i);
		
		if(cfg->getPlayerNum() != -1) //why is this if here
			configs.push_back(cfg);
	}


	bool success = true;
	for(unsigned int i = 0; i < mSystems.size(); i++)
	{
		EmulatorData* system = mSystems.at(i);

		for(unsigned int k = 0; k < configs.size(); k++)
		{
			if(!system->isValidMapping(configs.at(k), msg[0]))
			{
				success = false;
				break;
			}
		}

		if(!success)
			break;

		success = system->write(configs);

		if(!success)
		{
			msg[0] = "Script error!";
			break;
		}
	}

	
	if(success)
		msg[0] = "Done!";

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

	Renderer::drawCenteredText("ASSIGN INPUTS", 5, 0xFF00FFFF);

	std::stringstream stream;
	stream << "[P" << mCurrentPlayer + 1 << "]";
	std::string playerStr = stream.str();
	int plWidth = 0;
	Renderer::sizeText(playerStr, &plWidth, NULL);
	Renderer::drawText(playerStr, Renderer::getWidth() - plWidth - 2, 5, getPlayerColor(mCurrentPlayer));

	mInputList.render();
	mMappedList.render();
}
