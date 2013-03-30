#ifndef _FILEWRITER_H_
#define _FILEWRITER_H_

#include <string>
#include <fstream>

class FileWriter
{
public:
	~FileWriter();

	bool open(const std::string& path);
	void write(const std::string& in);
	void close();

private:
	std::ofstream mFile;
};

#endif