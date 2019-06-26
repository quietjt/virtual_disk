#pragma once
#include "i_archive.h"
#include <fstream>
#include "../predefine.h"

class TextReadArchive : public IArchive
{
public:
	bool open(const std::string& path);
	void close();


	virtual void serialize(int& value, const std::string& name = "");

	virtual void serialize(time_t& value, const std::string& name = "");

	virtual void serialize(std::string& value, const std::string& name = "");

	virtual void serialize(ISerializable* value, const char* name = "");

	virtual void serialize(char*& value, int size, const std::string& name = "");

private:
	void parseNext(std::pair<std::string, std::string>& pair);

private:
	std::fstream m_stream;
};