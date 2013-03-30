#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <string>

namespace Renderer
{
	void init();
	void deinit();

	void clear();
	void swapBuffers();

	int getWidth();
	int getHeight();

	int getFontSize();
	void sizeText(const std::string& text, int* w, int* h);

	void drawRect(int x, int y, int w, int h, int color);
	void drawText(const std::string& text, int x, int y, int color);
	void drawCenteredText(const std::string& text, int y, int color);
};

#endif