#ifndef _EMULATORDATA_H_
#define _EMULATORDATA_H_

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <string>
#include <vector>
#include "InputConfig.h"

struct InputData
{
public:
	const std::string name;
	const std::string location;
	const std::string funcSig;

	const bool required;
	const int typemask;
	const std::string mirrorAxis;

	asIScriptFunction* getWriteFunction(asIScriptModule* mod) { return mod->GetFunctionByDecl(funcSig.c_str()); };

	InputData(std::string n, std::string l, std::string funcName, bool req, int type, std::string mirror) : name(n), location(l), funcSig(funcName),
		required(req), typemask(type), mirrorAxis(mirror)
	{
	}
};

class EmulatorData
{
public:
	EmulatorData(const std::string& path, asIScriptEngine* engine);
	~EmulatorData();

	bool write(std::vector<InputConfig*> players);

	std::string getName();
	std::string getShortName();
	std::string getDescription();

	unsigned int getInputCount();
	InputData* getInput(unsigned int index);

	bool isLoaded();

	bool isValidMapping(InputConfig* config, std::string& errorMsg);
	
private:
	asIScriptEngine* mScriptEngine;
	asIScriptModule* mScriptModule;

	asIScriptFunction* mScriptInitFunc;
	asIScriptFunction* mScriptPerPlayerFunc;
	asIScriptFunction* mScriptPerInputFunc;

	bool loadFile(const std::string& path);
	bool addWrappedScript(CScriptBuilder& builder, const std::string& script, const char* sectionName, const char* funcDecl);
	bool addInput(CScriptBuilder& builder, const std::string& name, const std::string& location, const std::string& script, bool req, const std::string& types, const std::string& mirrorAxis);

	std::vector<InputData*> mInputs;

	std::string mName;
	std::string mShortName;
	std::string mDescription;
	std::string mConfigPath;

	bool mLoaded;
};

#endif
