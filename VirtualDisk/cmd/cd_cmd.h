#pragma once

#include "i_command.h"

class CdCmd : public ICommand
{
public:
	CdCmd();

	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);

private:
	bool m_isSupportGlob;
};