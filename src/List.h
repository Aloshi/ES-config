#ifndef _LIST_H_
#define _LIST_H_

#include "InputConfig.h"
#include "Window.h"

#define SCROLL_SPEED 100
#define SCROLL_DELAY 360

//Mostly just scrolling logic.
template<typename ListType>
class List
{
public:
	List(Window* window) : mWindow(window)
	{
		mOffsetX = 0;
		mOffsetY = 0;
		mCentered = true;

		mCursor = 0;
		mScrolling = false;
		mScrollDir = 0;
		mScrollTimer = 0;
	}
	
	virtual void input(InputConfig* config, Input input)
	{
		if(getLength() == 0)
			return;

		if(input.value == 0)
		{
			//stop scrolling
			if(config->isMappedTo("up", input) || config->isMappedTo("down", input))
			{
				mScrollTimer = 0;
				mScrollDir = 0;
				mScrolling = false;
			}
		}else{
			if(config->isMappedTo("up", input))
			{
				mScrollDir = -1;
				scroll();
			}
			if(config->isMappedTo("down", input))
			{
				mScrollDir = 1;
				scroll();
			}
		}
	}

	virtual void update(int deltaTime)
	{
		if(getLength() == 0)
			return;

		if(mScrollDir != 0)
		{
			mScrollTimer += deltaTime;

			int minTime = (mScrolling ? SCROLL_SPEED : SCROLL_DELAY);
			if(mScrollTimer >= minTime)
			{
				mScrolling = true;
				mScrollTimer -= minTime;
				scroll();
			}
		}
	}

	virtual void render() = 0;

	void setPosition(int x, int y)
	{
		mOffsetX = x;
		mOffsetY = y;
	}

	int getOffsetX() { return mOffsetX; }
	int getOffsetY() { return mOffsetY; }

	void setCentered(bool center) { mCentered = center; };

	void setCursorColor(int color) { mCursorColor = color; };

	virtual int getLength() = 0;
	virtual void clear() = 0;

	virtual int getSelectedIndex() { return mCursor; };
	virtual void setSelectedIndex(int index)
	{
		if(index < 0)
			index = 0;
		if(index >= getLength())
			index = getLength() - 1;

		mCursor = index;
	}

protected:
	int mOffsetX;
	int mOffsetY;
	bool mCentered;

	int mCursorColor;

	bool mScrolling;
	int mScrollTimer;
	int mScrollDir;
	int mCursor;

	void scroll()
	{
		mCursor += mScrollDir;
		while(mCursor < 0)
			mCursor += getLength();
		while(mCursor >= getLength())
			mCursor -= getLength();
	}

	Window* mWindow;
};

#endif
