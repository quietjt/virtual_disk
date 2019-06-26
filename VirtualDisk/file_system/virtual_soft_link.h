#include "virtual_node.h"
#include <fstream>

class VirtualSoftLink : public VirtualNode
{
public:
	VirtualSoftLink();
	virtual ~VirtualSoftLink();

	virtual Mode getMode() const
	{
		return VirtualNode::SoftLink;
	}

	VirtualNode* getTargetLinkNode();

	std::string getTargetLinkPath();
	
	void setTargetLinkPath(const std::string& targetLinkPath);

	virtual void serialize(IArchive& ar, const std::string& name = "");

	virtual void deserialize(IArchive& ar, const std::string& name = "");

private:
	char* m_content;
};