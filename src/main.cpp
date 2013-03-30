#include <iostream>
#include <stdlib.h>
#include <angelscript.h>
#include <scriptarray/scriptarray.h>
#include <scriptstdstring/scriptstdstring.h>

#include "EmulatorData.h"
#include "FileWriter.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Window.h"
#include "GuiDetectDevice.h"

//used to print angelscript messages to the terminal
void MessageCallback(const asSMessageInfo *msg, void *param)
{
  const char *type = "ERR ";
  if( msg->type == asMSGTYPE_WARNING ) 
    type = "WARN";
  else if( msg->type == asMSGTYPE_INFORMATION ) 
    type = "INFO";

  std::cout << msg->section << " (" << msg->row << ", " << msg->col << ") : " << type << " : " << msg->message << "\n";
}

//used by scripts for debugging
void print(const std::string& in)
{
	std::cout << in << "\n";
}

asIScriptEngine* setUpScriptEngine()
{
	asIScriptEngine* engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

	int r;
	r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

	if(r < 0)
	{
		std::cout << "Could not set message callback on engine!\n";
	}

	//initialize array and string angelscript addons
	RegisterScriptArray(engine, true);
	RegisterStdString(engine);

	//register print function (for debugging)
	r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
	if(r < 0)
	{
		std::cout << "Could not register print function!\n";
	}

	//register file stuff
	r = engine->RegisterObjectType("File", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if(r < 0)
	{
		std::cout << "Could not register File object type!\n";
	}

	r = engine->RegisterObjectMethod("File", "void write(const string &in)", asMETHOD(FileWriter, write), asCALL_THISCALL);
	if(r < 0)
	{
		std::cout << "Could not register File.write!\n";
	}

	//register InputType enum
	r = engine->RegisterEnum("InputType");
	r = engine->RegisterEnumValue("InputType", "TYPE_AXIS", TYPE_AXIS);
	r = engine->RegisterEnumValue("InputType", "TYPE_BUTTON", TYPE_BUTTON);
	r = engine->RegisterEnumValue("InputType", "TYPE_HAT", TYPE_HAT);

	//register SDL_HAT "enum"
	r = engine->RegisterEnum("Hat");
	r = engine->RegisterEnumValue("Hat", "HAT_UP", SDL_HAT_UP);
	r = engine->RegisterEnumValue("Hat", "HAT_DOWN", SDL_HAT_DOWN);
	r = engine->RegisterEnumValue("Hat", "HAT_LEFT", SDL_HAT_LEFT);
	r = engine->RegisterEnumValue("Hat", "HAT_RIGHT", SDL_HAT_RIGHT);


	return engine;
}

int main(int argc, char *argv[])
{
	asIScriptEngine* engine = setUpScriptEngine();

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	Renderer::init();

	Window window(engine);
	window.getInputManager()->init();

	window.pushGui(new GuiDetectDevice(&window));

	SDL_Event ev;

	bool running = true;

	int deltaTime;
	long curTime = SDL_GetTicks();
	long lastTime;

	while(running)
	{
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			case SDL_JOYAXISMOTION:
			case SDL_JOYHATMOTION:
			case SDL_KEYDOWN:
				window.getInputManager()->parseEvent(ev);
				break;

			}
		}

		lastTime = curTime;
		curTime = SDL_GetTicks();
		deltaTime = (int)(curTime - lastTime);

		window.update(deltaTime);
		Renderer::clear();
		window.render();

		Renderer::swapBuffers();
	}



	/*EmulatorData test("retroarch_config.xml", engine);
	
	std::vector<InputConfig*> testConfigs;
	testConfigs.push_back(new InputConfig(0));

	testConfigs.at(0)->setInput("a", Input(TYPE_BUTTON, 0, 0, true));

	test.write(testConfigs);

	std::cout << "done\n";*/

	Renderer::deinit();

	SDL_Quit();
	return 0;
}