#pragma once
#include "../predefine.h"
#include "i_serializable.h"

class IArchive 
{
public:
	virtual void serialize(ISerializable* value, const char* name = "")
	{
		value->serialize(*this, name);
	}

	virtual void serialize(int& value, const std::string& name = "") = 0;

	virtual void serialize(time_t& value, const std::string& name = "") = 0;

	virtual void serialize(std::string& value, const std::string& name = "") = 0;

	virtual void serialize(char*& value, int size, const std::string& name = "") = 0;
};