#include "EmulatorData.h"
#include "PugiXML/pugixml.hpp"
#include <iostream>
#include "FileWriter.h"

#define SCRIPT_INIT_SIGNATURE "void init(File@ f)"
#define SCRIPT_PERPLAYER_SIGNATURE "void perPlayer(File@ f, int PLAYER_NUMBER, int DEVICE_ID)"
#define SCRIPT_PERINPUT_SIGNATURE "void perInput(File@ f, string INPUT_NAME, InputType INPUT_TYPE, int INPUT_ID, int INPUT_VALUE, int DEVICE_ID)"

EmulatorData::EmulatorData(const std::string& path, asIScriptEngine* engine) : mScriptEngine(engine)
{
	mScriptModule = NULL;
	mScriptInitFunc = NULL;
	mScriptPerInputFunc = NULL;
	mScriptPerPlayerFunc = NULL;

	mLoaded = loadFile(path);
	if(!mLoaded)
	{
		std::cout << "Error loading file!\n";
	}
}

EmulatorData::~EmulatorData()
{
	for(unsigned int i = 0; i < mInputs.size(); i++)
	{
		delete mInputs.at(i);
	}

	if(mScriptModule != NULL)
		mScriptEngine->DiscardModule(mScriptModule->GetName());
}

std::string EmulatorData::getName() { return mName; }
std::string EmulatorData::getShortName() { return mShortName; }
std::string EmulatorData::getDescription() { return mDescription; }
bool EmulatorData::isLoaded() { return mLoaded; }

bool EmulatorData::loadFile(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file(path.c_str());

	if(!res)
	{
		std::cout << "   " << res.description() << "\n";
		return false;
	}

	pugi::xml_node root = doc.child("emulator");


	mName = root.child("name").text().get();
	if(mName.empty())
	{
		std::cout << "Name for EmulatorData at path \"" << path << "\" is empty!\n";
		return false;
	}

	mShortName = root.child("shortName").text().get();
	if(mShortName.empty())
		mShortName = mName;

	mDescription = root.child("desc").text().get();
	mConfigPath = root.child("configPath").text().get();

	
	//AS stuff
	int r; //error code
	std::string modName = (std::string("emulator_") + mName);
	CScriptBuilder builder;

	r = builder.StartNewModule(mScriptEngine, modName.c_str());
	if(r < 0)
	{
		std::cout << "Could not start new AS module!\n";
		return false;
	}

	//this is just a simple copy/paste into the top of the script
	r = builder.AddSectionFromMemory("declaration", root.child("declarationScript").text().get());
	if(r < 0)
	{
		std::cout << "Could not add declarationScript!\n";
		return false;
	}

	if(!addWrappedScript(builder, root.child("initScript").text().get(), "init", SCRIPT_INIT_SIGNATURE) || 
		!addWrappedScript(builder, root.child("perPlayerScript").text().get(), "perPlayer", SCRIPT_PERPLAYER_SIGNATURE) ||
		!addWrappedScript(builder, root.child("perInputScript").text().get(), "perInput", SCRIPT_PERINPUT_SIGNATURE))
	{
		return false;
	}

	for(pugi::xml_node inputNode = root.child("input"); inputNode; inputNode = inputNode.next_sibling())
	{
		if(!addInput(builder, inputNode.attribute("name").as_string(), inputNode.attribute("location").as_string(), inputNode.text().get(), 
			inputNode.attribute("require").as_bool(), inputNode.attribute("type").as_string(), inputNode.attribute("mirrorAxis").as_string()))
		{
			std::cout << "Could not add input!\n";
			return false;
		}
	}

	
	r = builder.BuildModule();
	if(r < 0)
	{
		std::cout << "Could not build AS module!\n";
		return false;
	}
	
	mScriptModule = mScriptEngine->GetModule(modName.c_str());
	mScriptInitFunc = mScriptModule->GetFunctionByDecl(SCRIPT_INIT_SIGNATURE);
	mScriptPerPlayerFunc = mScriptModule->GetFunctionByDecl(SCRIPT_PERPLAYER_SIGNATURE);
	mScriptPerInputFunc = mScriptModule->GetFunctionByDecl(SCRIPT_PERINPUT_SIGNATURE);
	//end AS stuff

	return true;
}

bool EmulatorData::addWrappedScript(CScriptBuilder& builder, const std::string& script, const char* sectionName, const char* funcDecl)
{
	std::string wrappedScript = funcDecl;
	wrappedScript += " {\n";
	wrappedScript += script;
	wrappedScript += "\n}";

	int r = builder.AddSectionFromMemory(sectionName, wrappedScript.c_str(), wrappedScript.length());

	if(r < 0)
	{
		std::cout << "Could not add " << sectionName << "!\n";
		return false;
	}else{
		return true;
	}
}

//yes this is slow and bad and doesnt get everything but it's not performance critical
std::string stripSpecialChars(const std::string& str)
{
	std::string ret;
	for(size_t i = 0; i < str.length(); i++)
	{
		if(str[i] == ' ' || str[i] == '!' || str[i] == '\\' || str[i] == '#' || str[i] == '*' || str[i] == '+' || str[i] == '-')
			ret += '_';
		else
			ret += str[i];
	}

	return ret;
}

bool EmulatorData::addInput(CScriptBuilder& builder, const std::string& name, const std::string& location, const std::string& script, bool req, const std::string& types, const std::string& mirrorAxis)
{
	if(name.empty() || location.empty())
	{
		std::cout << "Name or Location for input is empty!\n";
		return false;
	}

	if(!mirrorAxis.empty() && mirrorAxis == location)
	{
		std::cout << "[" << name << "]'s mirrorAxis == [" << name << "]'s location!\n";
		return false;
	}

	std::string safeName = stripSpecialChars(name);
	if(safeName.empty())
	{
		std::cout << "Input name empty!\n";
		return false;
	}
	std::string funcSig = "void input_" + safeName + "(File@ f, string INPUT_NAME, InputType INPUT_TYPE, int INPUT_ID, int INPUT_VALUE, int DEVICE_ID)";

	bool success = addWrappedScript(builder, script, safeName.c_str(), funcSig.c_str());

	if(!success)
		return false;

	int typemask;
	std::stringstream stream(types);
	std::string type;
	while(getline(stream, type, '|'))
	{
		if(type == "AXIS")
			typemask |= (1 << TYPE_AXIS);
		else if(type == "BUTTON")
			typemask |= (1 << TYPE_BUTTON);
		else if(type == "HAT")
			typemask |= (1 << TYPE_HAT);
		else if(type == "KEY")
			typemask |= (1 << TYPE_KEY);
		
	}

	InputData* input = new InputData(name, location, funcSig, req, typemask, mirrorAxis);
	mInputs.push_back(input);
	return true;
}

bool EmulatorData::write(std::vector<InputConfig*> configs)
{
	asIScriptContext* cont = mScriptEngine->CreateContext();

	FileWriter file;
	if(!file.open(mConfigPath))
	{
		std::cout << "Could not open file to write config!\n";
		return false;
	}

	cont->Prepare(mScriptInitFunc);
	cont->SetArgAddress(0, &file);
	if(cont->Execute() != asEXECUTION_FINISHED)
	{
		std::cout << "Init script crashed!\n";
		return false;
	}

	for(unsigned int i = 0; i < configs.size(); i++)
	{
		InputConfig* config = configs.at(i);

		cont->Prepare(mScriptPerPlayerFunc);
		cont->SetArgAddress(0, &file);
		cont->SetArgDWord(1, config->getPlayerNum());
		cont->SetArgDWord(2, config->getDeviceId());
		if(cont->Execute() != asEXECUTION_FINISHED)
		{
			std::cout << "Per-Player script for player " << config->getPlayerNum() << " crashed!\n";
			return false;
		}

		for(unsigned int i = 0; i < mInputs.size(); i++)
		{
			InputData* inputData = mInputs.at(i);
			Input input = config->getInputByLocation(inputData->location);

			if(!input.configured)
				continue;

			std::cout << "[inputData '" << inputData->name << "']\n";

			cont->Prepare(mScriptPerInputFunc);
			cont->SetArgAddress(0, &file);
			cont->SetArgObject(1, (void*)&inputData->name);
			cont->SetArgDWord(2, input.type);
			cont->SetArgDWord(3, input.id);
			cont->SetArgDWord(4, input.value);
			cont->SetArgDWord(5, config->getDeviceId());
			if(cont->Execute() != asEXECUTION_FINISHED)
			{
				std::cout << "Per-Input script crashed on input named [" << inputData->name << "]!\n";
				return false;
			}

			cont->Prepare(inputData->getWriteFunction(mScriptModule));
			cont->SetArgAddress(0, &file);
			cont->SetArgObject(1, (void*)&inputData->name);
			cont->SetArgDWord(2, input.type);
			cont->SetArgDWord(3, input.id);
			cont->SetArgDWord(4, input.value);
			cont->SetArgDWord(5, config->getDeviceId());
			if(cont->Execute() != asEXECUTION_FINISHED)
			{
				std::cout << "Input [" << inputData->name << "] write script crashed!\n";
				return false;
			}
		}
	}

	cont->Release();

	return true;
}

unsigned int EmulatorData::getInputCount()
{
	return mInputs.size();
}

InputData* EmulatorData::getInput(unsigned int index)
{
	return mInputs.at(index);
}

bool EmulatorData::isValidMapping(InputConfig* config, std::string& errorMsg)
{
	for(unsigned int i = 0; i < mInputs.size(); i++)
	{
		InputData* data = mInputs.at(i);
		Input input = config->getInputByLocation(data->location);
		if(data->required && !input.configured)
		{
			errorMsg = "Required input [" + data->name + "] not mapped!";
			return false;
		}

		//typemasks up in here.
		if(input.configured && data->typemask != 0)
		{
			//convert input.type to a bitmask
			int type = 1 << input.type;
			if((data->typemask & type) == 0)
			{
				errorMsg = "Input [" + data->name + "] is an invalid type!";
				return false;
			}
		}

		if(!data->mirrorAxis.empty())
		{
			Input mirror = config->getInputByLocation(data->mirrorAxis);
			if((input.configured && input.type == TYPE_AXIS) || (mirror.configured && mirror.type == TYPE_AXIS))
			{
				//required
				if(input.id != mirror.id || input.value == mirror.value)
				{
					errorMsg = "Axes [" + data->location + "] and [" + data->mirrorAxis + "] must mirror each other!";
					return false;
				}
			}
		}
	}

	return true;
}