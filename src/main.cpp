//Written by Alec "Aloshi" Lofquist.

#include <iostream>
#include <stdlib.h>
#include <angelscript.h>
#include <scriptarray/scriptarray.h>
#include <scriptstdstring/scriptstdstring.h>

#include "FileWriter.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Window.h"
#include "GuiDetectDevice.h"
#include <boost/filesystem.hpp>

//stuff passed by command-line that doesn't really change
std::string FORCED_SCRIPT_DIRECTORY = "";
std::string RESOURCE_PREFIX = "resources/";

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
	r = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);   assert(r >= 0);

	//initialize array and string angelscript addons
	RegisterScriptArray(engine, true);
	RegisterStdString(engine);

	//register print function (for debugging)
	r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);   assert(r >= 0);

	//register file stuff
	r = engine->RegisterObjectType("File", 0, asOBJ_REF | asOBJ_NOCOUNT);   assert(r >= 0);
	r = engine->RegisterObjectMethod("File", "void write(const string &in)", asMETHOD(FileWriter, write), asCALL_THISCALL);   assert(r >= 0);

	//register InputType enum
	r = engine->RegisterEnum("InputType");   assert(r >= 0);
	r = engine->RegisterEnumValue("InputType", "TYPE_AXIS", TYPE_AXIS);   assert(r >= 0);
	r = engine->RegisterEnumValue("InputType", "TYPE_BUTTON", TYPE_BUTTON);   assert(r >= 0);
	r = engine->RegisterEnumValue("InputType", "TYPE_HAT", TYPE_HAT);   assert(r >= 0);
	r = engine->RegisterEnumValue("InputType", "TYPE_KEY", TYPE_KEY);   assert(r >= 0);
	r = engine->RegisterEnumValue("InputType", "TYPE_COUNT", TYPE_COUNT);   assert(r >= 0);

	//register DEVICE_KEYBOARD constant
	r = engine->RegisterEnum("Device");   assert(r >= 0);
	r = engine->RegisterEnumValue("Device", "DEVICE_KEYBOARD", DEVICE_KEYBOARD);   assert(r >= 0);

	//register SDL_HAT "enum"
	r = engine->RegisterEnum("Hat");   assert(r >= 0);
	r = engine->RegisterEnumValue("Hat", "HAT_UP", SDL_HAT_UP);   assert(r >= 0);
	r = engine->RegisterEnumValue("Hat", "HAT_DOWN", SDL_HAT_DOWN);   assert(r >= 0);
	r = engine->RegisterEnumValue("Hat", "HAT_LEFT", SDL_HAT_LEFT);   assert(r >= 0);
	r = engine->RegisterEnumValue("Hat", "HAT_RIGHT", SDL_HAT_RIGHT);   assert(r >= 0);

	//register get key name function
	r = engine->RegisterGlobalFunction("string getKeyName(int id)", asFUNCTION(getKeyName), asCALL_CDECL);   assert(r >= 0);

	//register hat -> string function
	r = engine->RegisterGlobalFunction("string getHatDir(int val)", asFUNCTION(getHatDir), asCALL_CDECL);   assert(r >= 0);

	return engine;
}

bool parseArgs(int argc, char* argv[])
{
	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "--help") == 0)
		{
			std::cout << "\nES-config\n";
			std::cout << "A tool for configuring multiple emulators.\n";
			std::cout << "------------------------------------------\n";
			std::cout << "--scriptdir [path]	search for scripts at [path] (spaces? use quotes)\n";
			std::cout << "--resourcedir [path]	load images, etc from [path]\n";
			std::cout << "--help			pierce the heavens\n";
			return false;
		}else if(strcmp(argv[i], "--scriptdir") == 0 && i < argc - 1)
		{
			FORCED_SCRIPT_DIRECTORY = argv[i + 1];
			i += 2;
		}else if(strcmp(argv[i], "--resourcedir") == 0 && i < argc - 1)
		{
			RESOURCE_PREFIX = argv[i + 1];

			//must end in a slash
			if(RESOURCE_PREFIX[RESOURCE_PREFIX.length() - 1] != '/')
				RESOURCE_PREFIX += '/';
			i += 2;
		}
	}

	return true;
}

int main(int argc, char *argv[])
{
	if(!parseArgs(argc, argv))
		return 0;

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
			case SDL_KEYUP:
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

	Renderer::deinit();

	SDL_Quit();
	return 0;
}
