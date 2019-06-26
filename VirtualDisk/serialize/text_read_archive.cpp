#include "text_read_archive.h"

#include <iostream>
#include <cstdlib>

bool TextReadArchive::open(const std::string& path)
{
	if(m_stream.is_open())
		m_stream.close();

	m_stream.open(path.c_str(), std::ios::in | std::ios::binary);

	if(!m_stream)
		return false;

	return true;
}

void TextReadArchive::close()
{
	if(m_stream.is_open())
		m_stream.close();
}

void TextReadArchive::parseNext(std::pair<std::string, std::string>& pair)
{
	std::string line;
	std::getline(m_stream, line);

	size_t pos = line.find_first_of(':');
	
	pair.first = line.substr(0, pos);
	pair.second = line.substr(pos + 1);
}

void TextReadArchive::serialize(int& value, const std::string& name /*= ""*/)
{
	std::pair<std::string, std::string> pair;
	parseNext(pair);
	if(pair.first != name)
	{
		std::cerr << "Can not find : " << name << std::endl;
		return;
	}

	const char* cValue = pair.second.c_str();
	value = (int) atoi(cValue);
	
}

void TextReadArchive::serialize(time_t& value, const std::string& name /*= ""*/)
{
	std::pair<std::string, std::string> pair;
	parseNext(pair);
	if(pair.first != name)
	{
		std::cerr << "Can not find : " << name << std::endl;
		return;
	}

	const char* cValue = pair.second.c_str();
	value = (time_t) atol(cValue);
}

void TextReadArchive::serialize(std::string& value, const std::string& name /*= ""*/)
{
	std::pair<std::string, std::string> pair;
	parseNext(pair);
	if(pair.first != name)
	{
		std::cerr << "Can not find : " << name << std::endl;
		return;
	}
	
	value = pair.second;
}

void TextReadArchive::serialize(ISerializable* value, const char* name /*= ""*/)
{
	value->deserialize(*this);
	std::string line;
	getline(m_stream, line);
}

void TextReadArchive::serialize(char*& value, int size, const std::string& name /*= ""*/)
{
	std::string key;
	char c;
	while(m_stream.get(c))
	{
		if(c == ':')
			break;
		key.push_back(c);
	}

	if(key != name)
	{
		std::cerr << "Can not find : " << name << std::endl;

		std::string line;
		getline(m_stream, line);
		return;
	}
	m_stream.get(c);

	m_stream.read(value, size);
}
