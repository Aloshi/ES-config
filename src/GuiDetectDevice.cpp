#include "GuiDetectDevice.h"
#include "Window.h"
#include "Renderer.h"
#include "GuiBasicConfig.h"
#include <iostream>
#include <string>
#include<sstream>

GuiDetectDevice::GuiDetectDevice(Window* window) : Gui(window)
{
	mCurrentPlayer = 0;
}

void GuiDetectDevice::input(InputConfig* config, Input input)
{
	if((input.type == TYPE_BUTTON || input.type == TYPE_KEY) && input.value)
	{
		config->setPlayerNum(mCurrentPlayer);
		mCurrentPlayer++;

		//done?
		if(mCurrentPlayer >= mWindow->getInputManager()->getNumDevices())
		{
			mWindow->pushGui(new GuiBasicConfig(mWindow, input.device));
			delete this;
		}
	}
}

void GuiDetectDevice::update(int deltaTime)
{

}

void GuiDetectDevice::render()
{
	std::string playerString;
	std::stringstream stream;
	stream << (mCurrentPlayer + 1);
	stream >> playerString;

	Renderer::drawCenteredText("Press a button on the device for", Renderer::getHeight() / 3, 0x000000FF);
	Renderer::drawCenteredText("PLAYER " + playerString, (int)(Renderer::getHeight()*1.5) / 3, 0x00CC00FF);

	if(mWindow->getInputManager()->getNumDevices() == 0)
	{
		Renderer::drawCenteredText("No joysticks detected!", Renderer::getHeight()-(Renderer::getFontSize()*2), 0xFF0000FF);
		Renderer::drawCenteredText("Press F4 to quit.", Renderer::getHeight()-Renderer::getFontSize(), 0x000000FF);
	}
}
