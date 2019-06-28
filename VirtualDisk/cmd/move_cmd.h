#pragma once

#include "cmd_base.h"

class MoveCmd : public CommandBase
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);
protected:
	virtual void registerOptions();
};