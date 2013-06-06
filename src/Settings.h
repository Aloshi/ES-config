#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <string>
#include <map>
#include <vector>

class Settings
{
public:
	static Settings* getInstance();

	const std::string getScriptDir();
	const std::string getResourceDir();

	void setScriptDir(const std::string& dir);
	void setResourceDir(const std::string& dir);

	std::string resolveConfigPath(const std::string& origPath);
	void changeConfigPath(const std::string& origPath, const std::string& newPath);

	void loadFromFile(const std::string& file);

private:
	Settings();

	static Settings* sInstance;

	std::string mScriptDir;
	std::string mResourceDir;

	std::map<std::string, std::string> mConfigPathMap;
};

#endif