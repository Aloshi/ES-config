#include "Settings.h"
#include "PugiXML/pugixml.hpp"
#include <iostream>

//TODO:
//-add <include file="./retroarch.xml" elements="initScript|perPlayerScript|input" />
//-feel like a badass

Settings* Settings::sInstance = NULL;

Settings* Settings::getInstance()
{
	if(sInstance == NULL)
		sInstance = new Settings();

	return sInstance;
}

Settings::Settings()
{
	mResourceDir = "resources/";
	mScriptDir = "scripts/";
}

void Settings::setScriptDir(const std::string& dir) { mScriptDir = dir; }
void Settings::setResourceDir(const std::string& dir) { mResourceDir = dir; }
const std::string Settings::getResourceDir() { return mResourceDir; }
const std::string Settings::getScriptDir() { return mScriptDir; }

void Settings::changeConfigPath(const std::string& in, const std::string& out)
{
	mConfigPathMap[in] = out;
}

std::string Settings::resolveConfigPath(const std::string& in)
{
	if(mConfigPathMap[in].empty())
		return in;
	else
		return mConfigPathMap[in];
}

void Settings::loadFromFile(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());
	if(!result)
	{
		std::cout << "Error parsing Settings file!\n   " << result.description() << "\n";
		return;
	}

	std::string scriptDir = doc.child("scriptDir").text().get();
	std::string resourceDir = doc.child("resourceDir").text().get();

	if(!scriptDir.empty())
		setScriptDir(scriptDir);
	if(!resourceDir.empty())
		setResourceDir(resourceDir);

	for(pugi::xml_node node = doc.child("changeConfigPath"); node; node = node.next_sibling())
	{
		changeConfigPath(node.attribute("from").as_string(), node.attribute("to").as_string());
	}
}
