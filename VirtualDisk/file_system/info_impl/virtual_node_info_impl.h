#pragma once

#include "../virtual_node.h"
#include "../virtual_node_info.h"


class VirtualNodeInfoImpl
{
public:
	static VirtualNodeInfoImpl* create(VirtualNode* node, const std::string& absPath);

	explicit VirtualNodeInfoImpl(VirtualNode* node = NULL, const std::string& absPath = "") : m_node(node), m_fullPath(absPath)
	{
	}

	std::string getName();

	std::string getMode();

	std::string getCreateDay();

	std::string getCreateTime();

	std::string getFullPath();

	virtual bool isFile();

	virtual bool isDirectory();

	virtual bool isSoftlink();

	virtual bool isEmpty();

	virtual int getSize();

	virtual int getChildCount();

	virtual bool isSpecialDirectory();

	virtual std::string getTargetLinkPath();

	virtual VirtualNodeInfo getFinalTargetLinkInfo();

	virtual VirtualNodeInfo::Iterator begin();

	virtual VirtualNodeInfo::Iterator end();

	VirtualNode* getNode() 
	{
		return m_node;
	}

private:
	VirtualNode* m_node;
	std::string m_fullPath;
};