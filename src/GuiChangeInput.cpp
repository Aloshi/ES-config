#include "GuiChangeInput.h"
#include "Renderer.h"
#include "GuiConfigureInputs.h"
#include "ControllerImage.h"

GuiChangeInput::GuiChangeInput(Window* window, int playerNum, InputData* inputData, GuiConfigureInputs* updateOnDone) : Gui(window),
	mUpdateOnDone(updateOnDone), mPlayerNum(playerNum), mInputData(inputData)
{
}

void GuiChangeInput::input(InputConfig* config, Input input)
{
	if(input.value == 0 || config->getPlayerNum() != mPlayerNum)
		return;
	
	input.configured = true;
	config->mapInput(mInputData->location, input);

	mUpdateOnDone->updateList();
	delete this;
}

void GuiChangeInput::render()
{
	ControllerImage* img = ControllerImage::getImageForDevice(mWindow->getInputManager(), mWindow->getInputManager()->getInputConfigByPlayer(mPlayerNum)->getDeviceId());

	int w = img->getWidth();
	int h = img->getHeight() + Renderer::getFontSize()*3;

	int padx = 20;
	int pady = 15;
	//Renderer::drawRect(Renderer::getWidth()/2 - w/2 - padx, Renderer::getHeight()/2 - pady - h/2, w + padx*2, h + pady*2, 0x00000000);
	
	img->draw(Renderer::getWidth()/2 - img->getWidth()/2, Renderer::getHeight()/2 - img->getHeight()/2, mInputData->location);

	Renderer::drawCenteredText("PRESS INPUT FOR", Renderer::getHeight()/2 - h/2, 0x000000FF);
	Renderer::drawCenteredText("[" + mInputData->name + "]", Renderer::getHeight()/2 - h/2 + Renderer::getFontSize() * 2, 0x00AA00FF);
}
