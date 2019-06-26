#pragma once

#include "virtual_node.h"
#include <fstream>

class VirtualFile : public VirtualNode
{
public:
	VirtualFile();
	virtual ~VirtualFile();

	virtual Mode getMode() const
	{
		return VirtualNode::File;
	}

	int getSize() const { return m_size;}

	void setContentFromStream(std::fstream& stream);

	void copyFrom(const VirtualFile* otherFile);

	bool isEmpty();

	virtual void serialize(IArchive& ar, const std::string& name = "");

	virtual void deserialize(IArchive& ar, const std::string& name = "");

private:
	int m_size;
	char* m_content;
};