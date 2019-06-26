#pragma once

#include "../predefine.h"
#include "i_command.h"

class LoadCmd : public ICommand
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);
};