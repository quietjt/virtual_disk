#include "virtual_node.h"

VirtualNode::VirtualNode() : m_name("")
{
	m_createTimeStamp = time(NULL);
}

static char* modeNames[] = {
	"directory",
	"file",
	"softlink"
};


std::string VirtualNode::modeToStr(const Mode& value)
{
	return modeNames[value];
}

VirtualNode::Mode VirtualNode::strToMode(const std::string& value)
{
	int size =sizeof(modeNames) / sizeof(modeNames[0]);
	for(int i = 0; i < size; i++)
	{
		if(value == modeNames[i])
			return (VirtualNode::Mode)i;
	}

	return VirtualNode::Unkown;
}

void VirtualNode::serialize(IArchive& ar, const std::string& name)
{
	std::string modeStr = modeToStr(getMode());
	ar.serialize(modeStr, "mode");

	ar.serialize(m_name, "name");
	ar.serialize(m_createTimeStamp, "create_timestamp");

 }

 void VirtualNode::deserialize(IArchive& ar, const std::string& name /*= ""*/)
 {
	ar.serialize(m_name, "name");

	time_t createTimestamp;
	ar.serialize(createTimestamp, "create_timestamp");
	m_createTimeStamp = createTimestamp;
 }


