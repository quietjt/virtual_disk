#pragma once

#include "virtual_node_info_impl.h"
#include "../virtual_soft_link.h"

class VirtualSoftLinkInfoImpl : public VirtualNodeInfoImpl
{
public:
	VirtualSoftLinkInfoImpl(VirtualNode* node, const std::string& absPath) : VirtualNodeInfoImpl(node, absPath)
	{
		VirtualNode* selfNode = getNode();
		m_softlink = dynamic_cast<VirtualSoftLink*>(selfNode);
	}

	virtual bool isFile() 
	{ 
		return false; 
	}

	virtual bool isDirectory()
	{
		return false;
	}

	virtual bool isSoftlink()
	{
		return true;
	}

	virtual std::string getTargetLinkPath()
	{
		return m_softlink->getTargetLinkPath();
	}

	virtual VirtualNodeInfo getFinalTargetLinkInfo();

private:
	VirtualSoftLink* m_softlink;
};