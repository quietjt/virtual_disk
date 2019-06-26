#include "virtual_node_info.h"
#include "virtual_file_system.h"
#include "info_impl/virtual_node_info_impl.h"

VirtualNodeInfo::VirtualNodeInfo(VirtualNode* node)
{
	m_impl = VirtualNodeInfoImpl::create(node);
	m_isExist = node == NULL ? false : true;
}

VirtualNodeInfo::VirtualNodeInfo(const std::string& path)
{
	VirtualNode* node = VirtualFileSystem::getInstPtr()->getNode(path);

	m_impl = VirtualNodeInfoImpl::create(node);
	m_isExist = node == NULL ? false : true;
}

VirtualNodeInfo::VirtualNodeInfo(const VirtualNodeInfo& other) : m_isExist(other.m_isExist)
{
	m_impl = VirtualNodeInfoImpl::create(other.m_impl->getNode());
}

VirtualNodeInfo& VirtualNodeInfo::operator=(const VirtualNodeInfo& other)
{
	m_isExist = other.m_isExist;

	if(m_impl)
	{
		delete m_impl;
		m_impl = NULL;
	}

	m_impl = VirtualNodeInfoImpl::create(other.m_impl->getNode());

	return *this;
}

VirtualNodeInfo::~VirtualNodeInfo()
{
	if(m_impl)
	{
		delete m_impl;
		m_impl = NULL;
	}
}

std::string VirtualNodeInfo::getName()
{
	return m_impl->getName();
}

std::string VirtualNodeInfo::getMode()
{
	return m_impl->getMode();
}

std::string VirtualNodeInfo::getCreateDay()
{
	return m_impl->getCreateDay();
}

std::string VirtualNodeInfo::getCreateTime()
{
	return m_impl->getCreateTime();
}

bool VirtualNodeInfo::isFile() 
{
	return m_impl->isFile();
}

bool VirtualNodeInfo::isDirectory()
{
	return m_impl->isDirectory();
}

bool VirtualNodeInfo::isSoftlink()
{
	return m_impl->isSoftlink();
}

bool VirtualNodeInfo::isEmpty()
{
	return m_impl->isEmpty();
}

int VirtualNodeInfo::getSize()
{
	return m_impl->getSize();
}

int  VirtualNodeInfo::getChildCount()
{
	return m_impl->getChildCount();
}

bool VirtualNodeInfo::isSpecialDirectory()
{
	return m_impl->isSpecialDirectory();
}

std::string VirtualNodeInfo::getTargetLinkPath()
{
	return m_impl->getTargetLinkPath();
}

VirtualNodeInfo VirtualNodeInfo::getFinalTargetLinkInfo(const std::string& curPath)
{
	return m_impl->getFinalTargetLinkInfo(curPath);
}

VirtualNodeInfo::Iterator VirtualNodeInfo::begin()
{
	return m_impl->begin();
}

VirtualNodeInfo::Iterator VirtualNodeInfo::end()
{
	return m_impl->end();
}