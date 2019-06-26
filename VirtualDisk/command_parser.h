#pragma once
#include "predefine.h"

class CommandParser
{
public:
	void setCommandLine(const std::string& value);
	void getNext(std::string* outValue);

	void parseArg(std::set<std::string>& outOpitions, std::vector<std::string>& outPositionalOpitions);

private:
	void skipSpace();

private:
	std::string m_commandLine;
	int m_curPos;
};