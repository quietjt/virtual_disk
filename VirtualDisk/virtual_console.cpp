#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

#include "virtual_console.h"
#include "cmd/i_command.h"
#include "file_system/virtual_file_system.h"
#include "file_system/path.h"
#include "utils/string_utils.h"

static const std::string s_quietCommand = "quit";

VirtualConsole::~VirtualConsole() 
{
	for(std::map<std::string, ICommand*>::iterator it = m_commands.begin(); it != m_commands.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	m_commands.clear();
}

void VirtualConsole::onInit()
{
	const std::string rootDirectory = VirtualFileSystem::getInstPtr()->getDefaultDriveName();
	m_inst->setCurDirectoryPath(rootDirectory);
	m_inst->m_separatorType = BackSlash;
}

char VirtualConsole::getDisplaySeparator()
{
	static char* rets = "/\\";
	return rets[m_separatorType];
}

void VirtualConsole::setCurDirectoryPath(const std::string& path)
{
	m_curDirectory = path;
}

void VirtualConsole::run() 
{
	std::string input;
	std::string cmdName;
	while(true)
	{
		printHeader();

		getLine(input);

		if(input == "\n")
			continue;

		m_cmdParser.setCommandLine(input);
		m_cmdParser.getNext(&cmdName);

		std::string lowerCmdName = StringUtils::getLower(cmdName);
		//if(lowerCmdName == s_quietCommand)
		//{
		//	break;
		//}

		ICommand* cmd = getCommand(lowerCmdName);
		if(cmd)
		{
			cmd->execute(m_cmdParser, *this);
		}
		else
		{
			getOutput() << "Unknown command : " << cmdName << "\n";
		}

		getOutput() << "\n";
	}
}

void VirtualConsole::getLine(std::string& outValue)
{
	getline(std::cin, outValue);
}

std::ostream& VirtualConsole::getOutput()
{
	return std::cout;
}

void VirtualConsole::addCommand(const std::string& cmdName, ICommand* cmd)
{
	std::map<std::string, ICommand *>::iterator it = m_commands.find(cmdName);

	if(it != m_commands.end())
	{
		getOutput() << "already have command : " << cmdName << "\n";
	}

	if(it == m_commands.end())
		m_commands.insert(std::make_pair(cmdName, cmd));
}

ICommand* VirtualConsole::getCommand(const std::string& cmdName)
{
	std::map<std::string, ICommand *>::iterator it = m_commands.find(cmdName);

	if(it == m_commands.end())
		return NULL;

	return it->second;
}

void VirtualConsole::printHeader()
{
	getOutput() << m_curDirectory;
	if(Path::isDrive(m_curDirectory))
	{
		getOutput() << getDisplaySeparator();
	}
	getOutput() << ">";
}