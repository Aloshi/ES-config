#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <SDL_image.h>
#include <string>

class Image
{
public:
	Image(const std::string& path);
	~Image();

	void draw(int x, int y, bool center = false);
	void drawSection(int sx, int sy, int sw, int sh, int x, int y);

	void setAlpha(char alpha);

	int getWidth();
	int getHeight();

private:
	void load();

	std::string mPath;
	SDL_Surface* mImage;
};

#endif
