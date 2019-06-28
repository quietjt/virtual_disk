#pragma once

#include "../predefine.h"
#include "cmd_base.h"

class VirtualNodeInfo;

class DelCmd : public CommandBase
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);

	virtual void registerOptions();
};