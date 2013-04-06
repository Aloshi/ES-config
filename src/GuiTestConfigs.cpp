#include "GuiTestConfigs.h"
#include "Renderer.h"
#include <iostream>

GuiTestConfigs::GuiTestConfigs(Window* window, std::vector<EmulatorData*> systems) : Gui(window), mSystems(systems)
{
	mPressed = new std::vector<std::string>*[mWindow->getInputManager()->getNumPlayers()];
	for(int i = 0; i < mWindow->getInputManager()->getNumPlayers(); i++)
	{
		mPressed[i] = new std::vector<std::string>();
	}

	mHoldingBack = false;
	mBackTimer = 0;
}

GuiTestConfigs::~GuiTestConfigs()
{
	for(int i = 0; i < mWindow->getInputManager()->getNumPlayers(); i++)
	{
		delete mPressed[i];
	}

	delete[] mPressed;
}

void GuiTestConfigs::input(InputConfig* config, Input input)
{
	//input.value doesn't work for testing opposing hats unless the hat goes neutral in between
	//but it's not really critical, so...oh well.
	//probably the best fix for that would be on the InputManager::parseEvent level, sending out a value = 0 hat event whenever a hat changes significantly.

	int player = config->getPlayerNum();
	if(player == -1)
		return;

	if(input.value == 0)
	{
		for(unsigned int i = 0; i < mPressed[player]->size(); i++)
		{
			if(config->isMappedTo(mPressed[player]->at(i), input))
			{
				mPressed[player]->erase(mPressed[player]->begin() + i);
				i--;
			}
		}
	}else{
		std::vector<std::string> mappedNames = config->getMappedTo(input);
		for(unsigned int i = 0; i < mappedNames.size(); i++)
		{
			mPressed[player]->push_back(mappedNames.at(i));
		}
	}

	if(config->isMappedTo(LOCATION_BACK, input))
	{
		if(input.value == 0)
		{
			mHoldingBack = false;
			mBackTimer = 0;
		}else{
			mHoldingBack = true;
		}
	}
}

void GuiTestConfigs::update(int deltaTime)
{
	if(mHoldingBack)
	{
		mBackTimer += deltaTime;

		if(mBackTimer >= HOLD_BACK_TIME)
		{
			delete this;
		}
	}
}

std::vector<std::string> GuiTestConfigs::getNamesFromLocation(const std::string& loc)
{
	std::vector<std::string> names;

	for(unsigned int s = 0; s < mSystems.size(); s++)
	{
		EmulatorData* system = mSystems.at(s);
		for(unsigned int i = 0; i < system->getInputCount(); i++)
		{
			if(toLower(system->getInput(i)->location) == toLower(loc))
			{
				names.push_back(system->getShortName() + " " + system->getInput(i)->name);
			}
		}
	}

	return names;
}

void GuiTestConfigs::render()
{
	int y = (int)(Renderer::getFontSize() * 1.5);

	for(int player = 0; player < mWindow->getInputManager()->getNumPlayers(); player++)
	{
		for(unsigned int i = 0; i < mPressed[player]->size(); i++)
		{
			//convert loc -> all inputs across all emulators
			std::string text;
			std::vector<std::string> maps = getNamesFromLocation(mPressed[player]->at(i));

			for(unsigned int k = 0; k < maps.size(); k++)
			{
				text += "[" + maps.at(k) + "] ";
			}

			Renderer::drawText(text, 10, y, getPlayerColor(player));
			y += Renderer::getFontSize();
		}
	}

	Renderer::drawCenteredText("Hold [B] to return", Renderer::getHeight() - Renderer::getFontSize() - 6, mHoldingBack ? 0xFF0000FF : 0xAA0000FF);
}
