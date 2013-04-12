#include "ControllerData.h"
#include "PugiXML/pugixml.hpp"
#include <iostream>

ControllerData::ControllerData()
{
	mControllerImage = NULL;
}

ControllerData::~ControllerData()
{

}

bool ControllerData::loadFile(const std::string& path)
{
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file(path.c_str());
	if(!res)
	{
		std::cout << "Could not load ControllerData file \"" << path << "\" - " << res.description() << "\n";
		return false;
	}

	mLocationMap.clear();

	pugi::xml_node root = doc.child("controller");
	for(pugi::xml_node node = root.child("location"); node; node = node.next_sibling("location"))
	{
		std::string loc = node.attribute("name").as_string();
		int x = node.attribute("x").as_int();
		int y = node.attribute("y").as_int();
		int w = node.attribute("width").as_int();
		int h = node.attribute("height").as_int();

		addLocation(loc, x, y, w, h);
	}

	return true;
}

void ControllerData::addLocation(const std::string& location, int x, int y, int width, int height)
{
	mLocationMap[location] = LocationData(x, y, width, height);
}

void ControllerData::draw(int offsetx, int offsety, const std::string& highlight)
{
	if(mControllerImage == NULL)
		return;

	mControllerImage->draw(offsetx, offsety, false, 100);

	LocationData loc = mLocationMap[highlight];
	mControllerImage->drawSection(loc.x, loc.y, loc.width, loc.height, offsetx + loc.x, offsety + loc.y, 255);
}
