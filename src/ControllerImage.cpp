#include "ControllerImage.h"
#include "PugiXML/pugixml.hpp"
#include <iostream>
#include "InputManager.h"
#include "Settings.h"

std::map<std::string, ControllerImage*> ControllerImage::sCachedMap;

ControllerImage::ControllerImage()
{
	mImage = NULL;
	mImageTransparent = NULL;
}

ControllerImage::~ControllerImage()
{

}

bool ControllerImage::loadFile(const std::string& path)
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
	
	mImage = new Image(root.child("imagePath").text().get());
	mImageTransparent = new Image(root.child("imagePath").text().get());
	mImageTransparent->setAlpha((char)102);

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

void ControllerImage::addLocation(const std::string& location, int x, int y, int width, int height)
{
	mLocationMap[location] = LocationData(x, y, width, height);
}

void ControllerImage::draw(int offsetx, int offsety, const std::string& highlight)
{
	if(mImage == NULL)
		return;

	mImageTransparent->draw(offsetx, offsety, false);

	LocationData loc = mLocationMap[highlight];
	mImage->drawSection(loc.x, loc.y, loc.width, loc.height, offsetx + loc.x, offsety + loc.y);
}

ControllerImage* ControllerImage::getImageForDevice(InputManager* inputManager, int id)
{
	std::string path = Settings::getInstance()->getResourceDir() + "controllers/360esque.xml";

	if(sCachedMap[path] == NULL)
	{
		sCachedMap[path] = new ControllerImage();

		if(!sCachedMap[path]->loadFile(path))
		{
			delete sCachedMap[path];
			sCachedMap[path] = NULL;
		}
	}

	return sCachedMap[path];
}

int ControllerImage::getWidth() { return mImage->getWidth(); }
int ControllerImage::getHeight() { return mImage->getHeight(); }
