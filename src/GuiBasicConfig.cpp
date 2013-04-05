#include "GuiBasicConfig.h"
#include "Window.h"
#include "Renderer.h"
#include "GuiSelectEmulators.h"
#include <iostream>

static int inputCount = 6;
static std::string inputName[6] = { "Up", "Down", "Left", "Right", "A", "B"};

GuiBasicConfig::GuiBasicConfig(Window* window, int device) : Gui(window), mDevice(device)
{
	mCurInputId = 0;
}

void GuiBasicConfig::update(int deltaTime)
{

}

void GuiBasicConfig::input(InputConfig* config, Input input)
{
	if(input.device != mDevice || input.value == 0)
		return;

	if(mCurInputId >= inputCount)
	{
		//done
		if(input.type == TYPE_BUTTON || input.type == TYPE_KEY)
		{
			std::cout << "done with basic config\n";
			mWindow->pushGui(new GuiSelectEmulators(mWindow));
			delete this;
		}
	}else{
		input.configured = true;
		std::cout << "[type " << input.type << ", id " << input.id << ", val " << input.value << "] -> " << inputName[mCurInputId] << "\n";

		config->setInput(inputName[mCurInputId], input);
		mCurInputId++;
	}
}

void GuiBasicConfig::render()
{
	if(mCurInputId < inputCount)
	{
		Renderer::drawCenteredText("Please press [" + inputName[mCurInputId] + "].", Renderer::getHeight() / 3, 0x000000FF);
	}else{
		Renderer::drawCenteredText("Basic config done!", Renderer::getHeight() / 3, 0x00CC00FF);
		Renderer::drawCenteredText("Press any button to continue.", (int)(Renderer::getHeight()*1.5) / 3, 0x000000FF);
	}
	
}
