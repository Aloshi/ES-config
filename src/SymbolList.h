#ifndef _SYMBOLLIST_H_
#define _SYMBOLLIST_H_

#include "List.h"
#include "Image.h"
#include <vector>
#include "Renderer.h"

template<typename ListType>
class SymbolList : public List<ListType>
{
public:
	SymbolList(Window* window) : List<ListType>(window) { };

	virtual void render()
	{
		if(getLength() == 0)
			return;

		int entrySize = Renderer::getFontSize() + 8;
		int imgPadding = 4; //x padding between image and text
		int screenCount = (Renderer::getHeight() - mOffsetY) / entrySize;
		int startEntry = 0;

		if((int)mEntries.size() >= screenCount)
		{
			startEntry = mCursor - (int)(screenCount * 0.5);
			if(startEntry < 0)
				startEntry = 0;
			if(startEntry >= (int)mEntries.size() - screenCount)
				startEntry = mEntries.size() - screenCount;
		}

		int listCutoff = startEntry + screenCount;
		if(listCutoff > (int)mEntries.size())
			listCutoff = mEntries.size();

		int y = mOffsetY;

		for(int i = startEntry; i < listCutoff; i++)
		{
			if(mCursor == i)
			{
				Renderer::drawRect(0, y - 4, Renderer::getWidth(), entrySize, 0x2222FFFF);
			}

			ListEntry* entry = &mEntries.at((unsigned int)i);

			if(entry->image != NULL)
				entry->image->draw(mOffsetX + (entry->getImgWidth() / 2), y + (Renderer::getFontSize() / 2), true);

			Renderer::drawText(entry->text, mOffsetX + entry->getImgWidth() + imgPadding, y, 0x000000FF);

			y += entrySize;
		}
	}

	void addItem(ListType item, const std::string& text, Image* img)
	{
		mEntries.push_back(ListEntry(item, text, img));
	}

	ListType getItem(int i) { return mEntries.at(i).item; }
	Image* getImage(int i) { return mEntries.at(i).image; }

	ListType getSelected()
	{
		return mEntries.at(mCursor).item;
	}

	Image* getSelectedImage()
	{
		return mEntries.at(mCursor).image;
	}

	std::string getSelectedText()
	{
		return mEntries.at(mCursor).text;
	}

	void changeSelectedImage(Image* img)
	{
		mEntries.at(mCursor).image = img;
	}

	int getLength() { return mEntries.size(); }
	void clear() { mEntries.clear(); }

private:
	struct ListEntry
	{
		std::string text;
		ListType item;
		Image* image;

		ListEntry(ListType itm, std::string txt, Image* img) : item(itm), text(txt), image(img) { };

		int getImgWidth()
		{
			if(image != NULL)
				return image->getWidth();
			else
				return 0;
		}

		int getImgHeight()
		{
			if(image != NULL)
				return image->getHeight();
			else
				return 0;
		}
	};

	std::vector<ListEntry> mEntries;
};

#endif
