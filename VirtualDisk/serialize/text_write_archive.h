#pragma once
#include "i_archive.h"
#include <fstream>

class TextWriteArchive : public IArchive
{
public:
	bool open(const std::string& path);
	void close();

	virtual void serialize(int& value, const std::string& name = "");
	virtual void serialize(time_t& value, const std::string& name = "");

	virtual void serialize(std::string& value, const std::string& name = "");


	virtual void serialize(ISerializable* value, const char* name = "")
	{
		IArchive::serialize(value, name);
		m_stream << std::endl;
	}

	virtual void serialize(char*& value, int size, const std::string& name = "");

private:
	template<typename T>
	void serializeValue(T& value, const std::string& name = "")
	{
		if(name != "")
		{
			m_stream << name << ":" << value << std::endl;
		}
		else
		{
			m_stream << value << std::endl;
		}
	}

	std::fstream m_stream;
};