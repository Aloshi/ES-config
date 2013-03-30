#ifndef _GUIMESSAGE_H_
#define _GUIMESSAGE_H_

#include "Gui.h"
#include "Window.h"
#include <string>

class GuiMessage : public Gui
{
public:
	GuiMessage(Window* window, const std::string* msg, int msgCount);

	void input(InputConfig* config, Input input);
	void update(int deltaTime) { };
	void render();

private:
	std::string* mMessages;
	int mMessageCount;
};

#endif