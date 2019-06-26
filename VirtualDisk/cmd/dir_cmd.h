#pragma once

#include "../predefine.h"
#include "i_command.h"

class VirtualNodeInfo;

class DirCmd : public ICommand
{
public:
	virtual void execute(CommandParser& cmdParser, VirtualConsole& virtualConsole);

private:
	void displayPath(const std::string& absPath, VirtualConsole& virtualConsole, bool isOnlyDir);
	void displayPathDeep(const std::string& absPath, VirtualConsole& virtualConsole, bool isOnlyDir);

	void displayNodeInfo(VirtualNodeInfo& nodeInfo, std::ostream& output, bool isOnlyDir, int* totalSize, int *totalFile,  int* totalDir);

	void outputNodeInfo(VirtualNodeInfo& nodeInfo, std::ostream& output);
};