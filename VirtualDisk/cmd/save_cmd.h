#pragma once

#include "../predefine.h"
#include "i_command.h"

class SaveCmd : public ICommand
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);
};