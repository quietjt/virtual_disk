#pragma once

#include "virtual_node_info_impl.h"
#include "../virtual_directory.h"

class VirtualDirectoryInfoImpl : public VirtualNodeInfoImpl
{
public:
	explicit VirtualDirectoryInfoImpl(VirtualNode* node) : VirtualNodeInfoImpl(node)
	{
		VirtualNode* selfNode = getNode();
		m_dir = dynamic_cast<VirtualDirectory*>(selfNode);
	}

	virtual bool isFile() 
	{ 
		return false; 
	}

	virtual bool isDirectory()
	{
		return true;
	}

	virtual bool isSoftlink()
	{
		return false;
	}

	virtual int getChildCount()
	{
		return m_dir->getChildCount();
	}

	VirtualNodeInfo::Iterator begin();

	VirtualNodeInfo::Iterator end();


	virtual bool isEmpty();

	virtual bool isSpecialDirectory();

private:
	VirtualDirectory* m_dir;
};