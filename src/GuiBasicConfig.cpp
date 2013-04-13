#include "GuiBasicConfig.h"
#include "Window.h"
#include "Renderer.h"
#include "GuiSelectEmulators.h"
#include <iostream>

static int inputCount = 6;
static std::string inputName[6] = { "Up", "Down", "Left", "Right", "A", "B"};
static std::string inputLoc[6] = {LOCATION_UP, LOCATION_DOWN, LOCATION_LEFT, LOCATION_RIGHT, LOCATION_ACCEPT, LOCATION_BACK};

GuiBasicConfig::GuiBasicConfig(Window* window, InputConfig* target) : Gui(window), mTargetConfig(target), mCheckedImage("done.png")
{
	mCurInputId = 0;

	mControllerImage.loadFile("controllers/8button.xml");
}

void GuiBasicConfig::update(int deltaTime)
{

}

void GuiBasicConfig::input(InputConfig* config, Input input)
{
	if(config != mTargetConfig || input.value == 0)
		return;

	if(mCurInputId >= inputCount)
	{
		//done
		if(input.type == TYPE_BUTTON || input.type == TYPE_KEY)
		{
			if(mTargetConfig->getPlayerNum() < mWindow->getInputManager()->getNumPlayers() - 1)
			{
				mWindow->pushGui(new GuiBasicConfig(mWindow, mWindow->getInputManager()->getInputConfigByPlayer(mTargetConfig->getPlayerNum() + 1)));
			}else{
				mWindow->pushGui(new GuiSelectEmulators(mWindow));
			}
			delete this;
		}
	}else{
		if(config->getMappedTo(input).size() > 0)
		{
			mErrorMsg = "Already mapped!";
			return;
		}

		input.configured = true;
		std::cout << "[" << input.string() << "] -> " << inputName[mCurInputId] << "\n";
		
		config->mapInput(inputLoc[mCurInputId], input);
		mCurInputId++;
		mErrorMsg = "";
	}
}

void GuiBasicConfig::render()
{
	mControllerImage.draw(Renderer::getWidth()/2 - mControllerImage.getWidth()/2, (int)(Renderer::getHeight()/2.5) - mControllerImage.getHeight()/2, inputLoc[mCurInputId]);

	std::stringstream stream;
	stream << "PLAYER " << mTargetConfig->getPlayerNum() + 1 << ", press...";
	Renderer::drawText(stream.str(), 10, 10, getPlayerColor(mTargetConfig->getPlayerNum()));

	int y = 14 + Renderer::getFontSize();
	for(int i = 0; i < mCurInputId; i++)
	{
		mCheckedImage.draw(10 + (mCheckedImage.getWidth() / 2), y + (Renderer::getFontSize() / 2), true);
		Renderer::drawText(inputName[i], mCheckedImage.getWidth() + 14, y, 0x000000FF);
		y += mCheckedImage.getHeight() + 5;
	}

	if(mCurInputId >= inputCount)
	{
		Renderer::drawCenteredText("Basic config done!", (int)(Renderer::getHeight() * 0.6), 0x00CC00FF);
		Renderer::drawCenteredText("Press any button to continue.", (int)(Renderer::getHeight() * 0.6) + Renderer::getFontSize() + 4, 0x000000FF);
	}else{
		Renderer::drawText(inputName[mCurInputId], mCheckedImage.getWidth() + 14, y, 0x000000FF);
	}

	if(!mErrorMsg.empty())
		Renderer::drawCenteredText(mErrorMsg, Renderer::getHeight() - Renderer::getFontSize() - 10, 0xFF0000FF);
}
