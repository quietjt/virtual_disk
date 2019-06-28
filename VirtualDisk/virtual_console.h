#pragma once

#include <map>
#include <string>
#include "singleton.h"
#include "command_parser.h"

class ICommand;

class VirtualConsole : public Singleton<VirtualConsole>
{
public:
	enum SeparatorType
	{
		Slash = 0,
		BackSlash = 1,
	};

	virtual ~VirtualConsole();
	virtual void onInit();

	void run();

	void getLine(std::string& outValue);

	char getDisplaySeparator();

	std::ostream& getOutput();

	void addCommand(const std::string& cmdName,  ICommand* cmd);

	ICommand* getCommand(const std::string& cmdName);
	
	void setCurDirectoryPath(const std::string& path);
	
	const std::string& getCurDirectoryPath()
	{
		return m_curDirectory;
	}

	void executeCmd(const std::string& cmdStr);

private:
	void printHeader();

private:
	std::string m_curDirectory;
	
	CommandParser m_cmdParser;
	
	std::map<std::string,  ICommand*> m_commands;

	SeparatorType m_separatorType;
};