#include "virtual_node_info_impl.h"
#include "virtual_directory_info_impl.h"
#include "virtual_file_info_impl.h"
#include "virtual_soft_link_info_impl.h"
#include "../virtual_node_info.h"
#include <iostream>

static const int MAX_TIME_STR_BUFFER = 32;

VirtualNodeInfoImpl* VirtualNodeInfoImpl::create(VirtualNode* node, const std::string& absPath)
{
	if(node == NULL)
		return new VirtualNodeInfoImpl();
	else if(node->getMode() == VirtualNode::Directory)
		return new VirtualDirectoryInfoImpl(node, absPath);
	else if(node->getMode() == VirtualNode::File)
		return new VirtualFileInfoImpl(node, absPath);
	else if(node->getMode() == VirtualNode::SoftLink)
		return new VirtualSoftLinkInfoImpl(node, absPath);

	return new VirtualNodeInfoImpl();
}

std::string VirtualNodeInfoImpl::getName()
{
	if(m_node)
	{
		return m_node->getName();
	}

	return "";
}

std::string VirtualNodeInfoImpl::getMode()
{
	if(!m_node)
		return NULL;

	switch(m_node->getMode())
	{
	case VirtualNode::Directory:
		return "<DIR>";
	case VirtualNode::File:
		return "";
	case VirtualNode::SoftLink:
		return "<SYMLINK>";
	default:
		std::cerr << "unknown file mode" << std::endl;
		return "";
	}

	return "";
}

std::string VirtualNodeInfoImpl::getCreateDay()
{
	if(m_node == NULL)
		return "";

	time_t timestamp = m_node->getCreateTimeStamp();
	tm info;
	localtime_s(&info, &timestamp);

	char buff[MAX_TIME_STR_BUFFER] = { '\0' };
	strftime(buff, MAX_TIME_STR_BUFFER, "%Y/%m/%d", &info);
	return std::string(buff);
}

std::string VirtualNodeInfoImpl::getCreateTime()
{
	if(m_node == NULL)
		return "";

	time_t timestamp = m_node->getCreateTimeStamp();
	tm info;
	localtime_s(&info, &timestamp);

	char buff[MAX_TIME_STR_BUFFER] = { '\0' };
	strftime(buff, MAX_TIME_STR_BUFFER, "%H:%M", &info);
	return std::string(buff);
}

std::string VirtualNodeInfoImpl::getFullPath()
{
	return m_fullPath;
}

bool VirtualNodeInfoImpl::isFile()
{
	return false;
}

bool VirtualNodeInfoImpl::isDirectory()
{
	return false;
}

bool VirtualNodeInfoImpl::isSoftlink()
{
	return false;
}

bool VirtualNodeInfoImpl::isEmpty()
{
	return true;
}

int VirtualNodeInfoImpl::getSize()
{
	return 0;
}

int VirtualNodeInfoImpl::getChildCount()
{
	return 0;
}

bool VirtualNodeInfoImpl::isSpecialDirectory()
{
	return false;
}

std::string VirtualNodeInfoImpl::getTargetLinkPath()
{
	return "";
}

VirtualNodeInfo VirtualNodeInfoImpl::getFinalTargetLinkInfo()
{
	return VirtualNodeInfo();
}

VirtualNodeInfo::Iterator VirtualNodeInfoImpl::begin()
{
	return VirtualNodeInfo::Iterator();
}

VirtualNodeInfo::Iterator VirtualNodeInfoImpl::end()
{
	return VirtualNodeInfo::Iterator();
}