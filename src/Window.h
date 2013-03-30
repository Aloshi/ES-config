#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Gui.h"
#include "InputManager.h"
#include <vector>
#include <angelscript.h>

class Window
{
public:
	Window(asIScriptEngine* engine);
	~Window();

	void pushGui(Gui* gui);
	void removeGui(Gui* gui);
	Gui* peekGui();

	void input(InputConfig* config, Input input);
	void update(int deltaTime);
	void render();

	InputManager* getInputManager();
	asIScriptEngine* getScriptEngine();

private:
	asIScriptEngine* mScriptEngine;
	InputManager* mInputManager;
	std::vector<Gui*> mGuiStack;
};

#endif