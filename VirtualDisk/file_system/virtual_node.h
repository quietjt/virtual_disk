#pragma once

#include "../predefine.h"
#include <ctime>
#include "../utils/string_utils.h"

#include "../serialize/i_archive.h"

class VirtualNode : public ISerializable
{
public:
	enum Mode 
	{
		Unkown = -1,
		Directory = 0,
		File,
		SoftLink,
	};

	VirtualNode();

	virtual ~VirtualNode() {};

	inline void setName(const std::string& name)
	{
		m_name = name;
	}

	inline const std::string& getName() const
	{
		return m_name;
	}

	inline time_t getCreateTimeStamp() const
	{
		return m_createTimeStamp;
	}

	virtual Mode getMode() const
	{
		return Unkown;
	}

	virtual void serialize(IArchive& ar, const std::string& name = "");

	virtual void deserialize(IArchive& ar, const std::string& name = "");

	static std::string modeToStr(const Mode& value);

	static Mode strToMode(const std::string& value);

protected:
	std::string m_name;
	time_t m_createTimeStamp;
};