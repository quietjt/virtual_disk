#pragma once

#include "virtual_node_info_impl.h"
#include "../virtual_soft_link.h"

class VirtualSoftLinkInfoImpl : public VirtualNodeInfoImpl
{
public:
	explicit VirtualSoftLinkInfoImpl(VirtualNode* node) : VirtualNodeInfoImpl(node)
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

	virtual VirtualNodeInfo getFinalTargetLinkInfo(const std::string& curPath);

private:
	VirtualSoftLink* m_softlink;
};