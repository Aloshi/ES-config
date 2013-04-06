#include "GuiTestConfigs.h"
#include "Renderer.h"
#include <iostream>

GuiTestConfigs::GuiTestConfigs(Window* window) : Gui(window)
{
	mHoldingBack = false;
	mBackTimer = 0;
}

void GuiTestConfigs::input(InputConfig* config, Input input)
{
	//input.value doesn't work for testing opposing hats
	//but it's not really critical, so...oh well.
	//probably the best fix for that would be on the InputManager::parseEvent level, sending out a value = 0 hat event whenever a hat changes significantly.
	if(input.value == 0)
	{
		for(unsigned int i = 0; i < mPressed.size(); i++)
		{
			if(config->isMappedTo(mPressed.at(i), input))
			{
				mPressed.erase(mPressed.begin() + i);
				i--;
			}
		}
	}else{
		std::vector<std::string> mappedNames = config->getMappedTo(input);
		for(unsigned int i = 0; i < mappedNames.size(); i++)
		{
			mPressed.push_back(mappedNames.at(i));
		}
	}

	if(config->isMappedTo("b", input))
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

void GuiTestConfigs::render()
{
	int y = (int)(Renderer::getFontSize() * 1.5);
	for(unsigned int i = 0; i < mPressed.size(); i++)
	{
		Renderer::drawText("[" + mPressed.at(i) + "]", 15, y, 0x000000FF);
		y += Renderer::getFontSize();
	}

	Renderer::drawCenteredText("Hold [B] to return", Renderer::getHeight() - Renderer::getFontSize() - 6, mHoldingBack ? 0xFF0000FF : 0xAA0000FF);
}
