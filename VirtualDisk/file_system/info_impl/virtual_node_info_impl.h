#pragma once

#include "../virtual_node.h"
#include "../virtual_node_info.h"


class VirtualNodeInfoImpl
{
public:
	static VirtualNodeInfoImpl* create(VirtualNode* node);

	explicit VirtualNodeInfoImpl(VirtualNode* node = NULL) : m_node(node)
	{
	}

	std::string getName();

	std::string getMode();

	std::string getCreateDay();

	std::string getCreateTime();

	virtual bool isFile();

	virtual bool isDirectory();

	virtual bool isSoftlink();

	virtual bool isEmpty();

	virtual int getSize();

	virtual int getChildCount();

	virtual bool isSpecialDirectory();

	virtual std::string getTargetLinkPath();

	virtual VirtualNodeInfo getFinalTargetLinkInfo(const std::string& curPath);

	virtual VirtualNodeInfo::Iterator begin();

	virtual VirtualNodeInfo::Iterator end();

	VirtualNode* getNode() 
	{
		return m_node;
	}

private:
	VirtualNode* m_node;
};