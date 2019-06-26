#pragma once
#include "virtual_node_info_impl.h"
#include "../virtual_file.h"

class VirtualFileInfoImpl : public VirtualNodeInfoImpl
{
public:
	VirtualFileInfoImpl(VirtualNode* node, const std::string& absPath) : VirtualNodeInfoImpl(node, absPath)
	{
		VirtualNode* selfNode = getNode();
		m_file = dynamic_cast<VirtualFile*>(selfNode);
	}

	virtual bool isFile() 
	{ 
		return true; 
	}

	virtual bool isDirectory()
	{
		return false;
	}

	virtual bool isSoftlink()
	{
		return false;
	}

	virtual int getSize()
	{
		return m_file->getSize();
	}


	virtual bool isEmpty()
	{
		return m_file->isEmpty();
	}

private:
	VirtualFile* m_file;
};