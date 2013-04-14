#include "GuiDetectDevice.h"
#include "Window.h"
#include "Renderer.h"
#include "GuiBasicConfig.h"
#include <iostream>
#include <string>
#include <sstream>
#include "Util.h"

GuiDetectDevice::GuiDetectDevice(Window* window) : Gui(window)
{
	//clear any player information from the InputManager
	for(int i = 0; i < mWindow->getInputManager()->getNumPlayers(); i++)
	{
		InputConfig* cfg = mWindow->getInputManager()->getInputConfigByPlayer(i);
		cfg->setPlayerNum(-1);
		cfg->clear();
	}
	mWindow->getInputManager()->setNumPlayers(0);

	mCurrentPlayer = 0;
	mHoldingFinish = false;
}

void GuiDetectDevice::input(InputConfig* config, Input input)
{
	if((input.type == TYPE_BUTTON || input.type == TYPE_KEY))
	{
		if(config->getPlayerNum() != -1)
		{
			if(config->getPlayerNum() == 0)
			{
				if(input.value)
				{
					mFinishTimer = 0;
					mHoldingFinish = true;
				}else{
					mHoldingFinish = false;
				}
			}
			return;
		}

		if(!input.value)
			return;

		config->setPlayerNum(mCurrentPlayer);
		mWindow->getInputManager()->setNumPlayers(mWindow->getInputManager()->getNumPlayers() + 1); //inc total number of players
		mCurrentPlayer++;

		//mapped everything we possibly can?
		if(mCurrentPlayer >= mWindow->getInputManager()->getNumJoysticks() + 1) //+1 for keyboard
		{
			done();
		}
	}
}

void GuiDetectDevice::done()
{
	mWindow->pushGui(new GuiBasicConfig(mWindow, mWindow->getInputManager()->getInputConfigByPlayer(0)));
	delete this;
}

void GuiDetectDevice::update(int deltaTime)
{
	if(mHoldingFinish)
	{
		mFinishTimer += deltaTime;

		if(mFinishTimer > 750)
			done();
	}
}

void GuiDetectDevice::render()
{
	std::string playerString;
	std::stringstream stream;
	stream << (mCurrentPlayer + 1);
	stream >> playerString;

	Renderer::drawCenteredText("Press a button on the device for", Renderer::getHeight() / 3, 0x000000FF);
	Renderer::drawCenteredText("PLAYER " + playerString, (int)(Renderer::getHeight()*1.5) / 3, getPlayerColor(mCurrentPlayer));

	if(mWindow->getInputManager()->getNumPlayers() > 0)
	{
		Renderer::drawCenteredText("(P1 - hold a button to finish)", (int)(Renderer::getHeight()*2) / 3, (mHoldingFinish ? 0x0000FFFF : 0x000066FF));
	}

	if(mWindow->getInputManager()->getNumJoysticks() == 0)
	{
		Renderer::drawCenteredText("No joysticks detected!", Renderer::getHeight()-(Renderer::getFontSize()*2)-10, 0xFF0000FF);
	}

	Renderer::drawCenteredText("Press F4 to quit.", Renderer::getHeight()-Renderer::getFontSize() - 2 , 0x000000FF);
}
