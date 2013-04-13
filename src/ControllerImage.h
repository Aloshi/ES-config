#ifndef _CONTROLLERIMAGE_H_
#define _CONTROLLERIMAGE_H_

#include <string>
#include <map>
#include "Image.h"

class InputManager;

class ControllerImage
{
public:
	ControllerImage();
	~ControllerImage();

	bool loadFile(const std::string& path);
	void draw(int offsetx, int offsety, const std::string& highlight);

	int getWidth();
	int getHeight();

	static ControllerImage* getImageForDevice(InputManager* inputManager, int id);
private:
	void addLocation(const std::string& location, int x, int y, int width, int height);

	struct LocationData
	{
		int x;
		int y;
		int width;
		int height;

		LocationData(int px, int py, int w, int h) : x(px), y(py), width(w), height(h)
		{
		}

		LocationData() : x(0), y(0), width(0), height(0)
		{
		}
	};

	std::map<std::string, LocationData> mLocationMap;

	static std::map<std::string, ControllerImage*> sCachedMap;

	Image* mImageTransparent;
	Image* mImage;
};

#endif
