#pragma once

#include "i_command.h"

class MoveCmd : public ICommand
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);
};