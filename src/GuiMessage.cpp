#include "GuiMessage.h"
#include "Renderer.h"

GuiMessage::GuiMessage(Window* window, const std::string* msg, int msgCount) : Gui(window)
{
	mMessageCount = msgCount;
	mMessages = new std::string[msgCount];
	for(int i = 0; i < msgCount; i++)
	{
		mMessages[i] = msg[i];
	}
}

void GuiMessage::input(InputConfig* config, Input input)
{
	if(input.value != 0 && config->isMappedTo("a", input))
	{
		delete this;
	}
}

void GuiMessage::render()
{
	int y = Renderer::getHeight() / 3;
	
	int top = y - (int)(Renderer::getHeight() * 0.025);
	int bot = y + (Renderer::getFontSize() * (mMessageCount + 1)) + (int)(Renderer::getHeight() * 0.025);
	Renderer::drawRect(0, top, Renderer::getWidth(), bot - top, 0x222222FF);

	for(int i = 0; i < mMessageCount; i++)
	{
		Renderer::drawCenteredText(mMessages[i], y, 0x0000FFFF);
		y += Renderer::getFontSize();
	}

	Renderer::drawCenteredText("[A]", y + 2, 0x00AA00FF);
}
