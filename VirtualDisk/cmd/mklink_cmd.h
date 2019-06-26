#include "../predefine.h"
#include "i_command.h"

class MklinkCmd : public ICommand
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);
};