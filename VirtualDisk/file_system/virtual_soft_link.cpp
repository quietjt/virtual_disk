#include "virtual_soft_link.h"
#include <cstring>
#include "virtual_file_system.h"
#include <sstream>

VirtualSoftLink::VirtualSoftLink() : m_content(NULL)
{

}

VirtualSoftLink::~VirtualSoftLink()
{
	if(m_content)
	{
		delete[] m_content;
		m_content = NULL;
	}
}

std::string VirtualSoftLink::getTargetLinkPath()
{
	return m_content;
}

void VirtualSoftLink::setTargetLinkPath(const std::string& targetLinkPath)
{
	const char* cTargetLinkPath = targetLinkPath.c_str();

	if(m_content != NULL)
		delete[] m_content;

	m_content = new char[strlen(cTargetLinkPath) + 1];
	memcpy(m_content, cTargetLinkPath, strlen(cTargetLinkPath) + 1);
}

VirtualNode* VirtualSoftLink::getTargetLinkNode()
{
	return VirtualFileSystem::getInstPtr()->getNode(m_content);
}

void VirtualSoftLink::serialize(IArchive& ar, const std::string& name /*= ""*/)
{
	VirtualNode::serialize(ar);

	if(m_content == NULL)
	{
		std::string empty = "";
		ar.serialize(empty, "content");
	}
	else
	{
		std::string content = m_content;
		ar.serialize(content, "content");
	}
}

void VirtualSoftLink::deserialize(IArchive& ar, const std::string& name /*= ""*/)
{
	VirtualNode::deserialize(ar);
	std::string content;
	ar.serialize(content, "content");

	if(m_content != NULL)
	{
		delete[] m_content;
		m_content = NULL;
	}

	m_content = new char[content.size() + 1];
	memcpy(m_content, content.c_str(), sizeof(char) * content.size());
	m_content[content.size()] = '\0';
}
