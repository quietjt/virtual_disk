#pragma once

#include "i_command.h"
#include "../predefine.h"

class CommandBase : public ICommand
{
public:
	virtual void initialize();

protected:
	virtual void registerOptions() {};
	bool checkOptions(std::ostream& output, const std::set<std::string>& options);

protected:
	std::set<std::string> m_supportOptions;
};