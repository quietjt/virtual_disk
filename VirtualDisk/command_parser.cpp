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
	
	outValue->clear();

	bool waitNextQuot = false;

	char ch;
	while(true)
	{
		ch = m_commandLine[curI];
		if(ch == '\n' || ch == '\0')
			break;

		if(waitNextQuot)
		{
			if(ch != '\"')
				outValue->push_back(ch);
			else
				waitNextQuot = false;
		}
		else
		{
			if(ch == '\"')
				waitNextQuot = true;
			else if(ch != ' ')
				outValue->push_back(ch);
			else
				break;
		}

		curI++;
	}
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
			outOpitions.insert(arg.substr(1));
		else
			outPositionalOpitions.push_back(arg);
	}
}