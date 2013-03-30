#include "FileWriter.h"
#include <iostream>

FileWriter::~FileWriter()
{
	if(mFile.is_open())
		mFile.close();
}

bool FileWriter::open(const std::string& path)
{
	if(mFile.is_open())
		mFile.close();

	mFile.clear();
	mFile.open(path.c_str());

	return mFile.is_open();
}

void FileWriter::close()
{
	mFile.close();
}

void FileWriter::write(const std::string& in)
{
	if(!mFile.is_open())
		return;
	else
		mFile << in << "\n";
}
