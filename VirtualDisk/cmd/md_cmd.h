#pragma once

#include "i_command.h"

class MdCmd : public ICommand
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);
};