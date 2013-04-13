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

	asIScriptFunction* getWriteFunction(asIScriptModule* mod) { return mod->GetFunctionByDecl(funcSig.c_str()); };

	InputData(std::string n, std::string l, std::string funcName) : name(n), location(l), funcSig(funcName)
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

	bool isValid();
	
private:
	asIScriptEngine* mScriptEngine;
	asIScriptModule* mScriptModule;

	asIScriptFunction* mScriptInitFunc;
	asIScriptFunction* mScriptPerPlayerFunc;
	asIScriptFunction* mScriptPerInputFunc;

	bool loadFile(const std::string& path);
	bool addWrappedScript(CScriptBuilder& builder, const std::string& script, const char* sectionName, const char* funcDecl);
	bool addInput(CScriptBuilder& builder, const std::string& name, const std::string& location, const std::string& script);

	std::vector<InputData*> mInputs;

	std::string mName;
	std::string mShortName;
	std::string mDescription;
	std::string mConfigPath;

	bool mValid;
};

#endif