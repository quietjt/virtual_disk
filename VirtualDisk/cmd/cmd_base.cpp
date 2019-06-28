#include "cmd_base.h"

void CommandBase::initialize()
{
	registerOptions();
}

bool CommandBase::checkOptions(std::ostream& output, const std::set<std::string>& options)
{
	if(m_supportOptions.size() <= 0)
		return true;

	for(std::set<std::string>::const_iterator it = options.begin(); it != options.end(); it++)
	{
		std::set<std::string>::iterator findIt = m_supportOptions.find(*it);

		if(findIt == m_supportOptions.end())
		{
			output << "无效开关 - " << "\"" << *it << "\"" << std::endl;
			return false;
		}
	}

	return true;
}