#include "text_write_archive.h"

bool TextWriteArchive::open(const std::string& path)
{
	if(m_stream.is_open())
		m_stream.close();

	m_stream.open(path.c_str(), std::ios::out | std::ios::binary);

	if(!m_stream)
		return false;

	return true;
}

void TextWriteArchive::close()
{
	if(m_stream.is_open())
		m_stream.close();
}

void TextWriteArchive::serialize(int& value, const std::string& name /*= ""*/)
{
	m_stream << name << ":" << value << std::endl;
}

void TextWriteArchive::serialize(time_t& value, const std::string& name /*= ""*/)
{
	m_stream << name << ":" << value << std::endl;
}

void TextWriteArchive::serialize(std::string& value, const std::string& name /*= ""*/)
{
	m_stream << name << ":" << value << std::endl;
}

void TextWriteArchive::serialize(char*& value, int size, const std::string& name /*= ""*/)
{
	m_stream << name << ":";
	m_stream.write(value, size);
	m_stream << std::endl;
}
