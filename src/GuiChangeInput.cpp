#include "GuiChangeInput.h"
#include "Renderer.h"
#include "GuiConfigureInputs.h"

GuiChangeInput::GuiChangeInput(Window* window, int playerNum, const std::string& inputName, GuiConfigureInputs* updateOnDone) : Gui(window), mPlayerNum(playerNum), 
	mInputName(inputName), mUpdateOnDone(updateOnDone)
{
}

void GuiChangeInput::input(InputConfig* config, Input input)
{
	if(input.value == 0 || config->getPlayerNum() != mPlayerNum)
		return;
	
	input.configured = true;
	config->setInput(mInputName, input);

	mUpdateOnDone->updateList();
	delete this;
}

void GuiChangeInput::render()
{
	int w;
	Renderer::sizeText("PRESS INPUT FOR", &w, NULL);

	int x = (Renderer::getWidth() - w) / 2;

	Renderer::drawRect(x - 15, Renderer::getHeight() / 3 - 10, w + 15, Renderer::getFontSize() * 4 + 10, 0x00000000);

	Renderer::drawCenteredText("PRESS INPUT FOR", Renderer::getHeight() / 3, 0xAAAAAAFF);
	Renderer::drawCenteredText("[" + mInputName + "]", (int)(Renderer::getHeight() / 3) + Renderer::getFontSize() * 2, 0x00AA00FF);
}
