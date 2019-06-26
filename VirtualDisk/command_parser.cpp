#include "command_parser.h"

void CommandParser::setCommandLine(const std::string& value)
{
	m_curPos = 0;
	m_commandLine = value;

}

void CommandParser::getNext(std::string* outValue)
{
	skipSpace();
	int curI = m_curPos;
	
	while(m_commandLine[curI] != ' ' && m_commandLine[curI] != '\n' && 
			m_commandLine[curI] != '\0')
	{
		curI++;
	}

	*outValue = m_commandLine.substr(m_curPos, curI - m_curPos);
	m_curPos = curI;
}

void CommandParser::skipSpace()
{
	while(m_commandLine[m_curPos] == ' ')
	{
		m_curPos++;
	}
}

void CommandParser::parseArg(std::set<std::string>& outOpitions, std::vector<std::string>& outPositionalOpitions)
{
	outOpitions.clear();
	outPositionalOpitions.clear();
	std::string arg;
	while(true)
	{
		getNext(&arg);
		if(arg == "")
			break;
		if(arg[0] == '/')
			outOpitions.insert(arg);
		else
			outPositionalOpitions.push_back(arg);
	}
}