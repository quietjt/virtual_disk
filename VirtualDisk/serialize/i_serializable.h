#pragma once

#include "../predefine.h"

class IArchive;

class ISerializable 
{
public:
	virtual void serialize(IArchive& ar, const std::string& name = "") {}
	virtual void deserialize(IArchive& ar, const std::string& name = "") {}
};