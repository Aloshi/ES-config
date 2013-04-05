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
		int screenCount = (Renderer::getHeight() - this->getOffsetY()) / entrySize;
		int startEntry = 0;

		if((int)mEntries.size() >= screenCount)
		{
			startEntry = this->mCursor - (int)(screenCount * 0.5);
			if(startEntry < 0)
				startEntry = 0;
			if(startEntry >= (int)mEntries.size() - screenCount)
				startEntry = mEntries.size() - screenCount;
		}

		int listCutoff = startEntry + screenCount;
		if(listCutoff > (int)mEntries.size())
			listCutoff = mEntries.size();

		int y = this->getOffsetY();

		for(int i = startEntry; i < listCutoff; i++)
		{
			if(this->mCursor == i)
			{
				Renderer::drawRect(0, y - 4, Renderer::getWidth(), entrySize, 0x2222FFFF);
			}

			ListEntry* entry = &mEntries.at((unsigned int)i);

			if(entry->image != NULL)
				entry->image->draw(this->getOffsetX() + (entry->getImgWidth() / 2), y + (Renderer::getFontSize() / 2), true);

			Renderer::drawText(entry->text, this->getOffsetX() + entry->getImgWidth() + imgPadding, y, 0x000000FF);

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
		return mEntries.at(this->mCursor).item;
	}

	Image* getSelectedImage()
	{
		return mEntries.at(this->mCursor).image;
	}

	std::string getSelectedText()
	{
		return mEntries.at(this->mCursor).text;
	}

	void changeSelectedImage(Image* img)
	{
		mEntries.at(this->mCursor).image = img;
	}

	int getLength() { return mEntries.size(); }
	void clear()
	{
		this->mCursor = 0;
		mEntries.clear();
	}

private:
	struct ListEntry
	{
		ListType item;
		std::string text;
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
