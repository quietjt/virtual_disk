#include "virtual_file.h"
#include <memory>

VirtualFile::VirtualFile() : m_content(NULL)
{

}

VirtualFile::~VirtualFile()
{
	if(m_content)
	{
		delete[] m_content;
		m_content = NULL;
	}
}

void VirtualFile::setContentFromStream(std::fstream& stream)
{
	stream.seekg(0, std::ios::end);
	m_size = stream.tellg();
	stream.seekg(0, std::ios::beg);

	if(m_content != NULL)
		delete[] m_content;

	m_content = new char[m_size];

	stream.read(m_content, m_size);
}

void VirtualFile::copyFrom(const VirtualFile* otherFile)
{
	this->m_createTimeStamp = otherFile->m_createTimeStamp;
	if(this->m_content != NULL)
		delete[] this->m_content;

	this->m_size = otherFile->m_size;
	this->m_content = new char[this->m_size];

	this->m_name = otherFile->m_name;

	memcpy(this->m_content, otherFile->m_content, this->m_size);
}

bool VirtualFile::isEmpty()
{
	return m_size > 0 ? false : true;
}

void VirtualFile::serialize(IArchive& ar, const std::string& name /*= ""*/)
{
	VirtualNode::serialize(ar);
	ar.serialize(m_size, "size");
	ar.serialize(m_content, m_size, "content");
}

void VirtualFile::deserialize(IArchive& ar, const std::string& name /*= ""*/)
{
	VirtualNode::deserialize(ar);
	ar.serialize(m_size, "size");
	
	if(m_content != NULL)
	{		
		delete[] m_content;
		m_content = NULL;
	}

	m_content = new char[m_size];
	ar.serialize(m_content, m_size, "content");
}
