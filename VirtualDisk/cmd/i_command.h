#pragma once

#include "../command_parser.h"
#include "../virtual_console.h"

class ICommand
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole) = 0;
};