#include <iostream>

#include "md_cmd.h"
#include "../predefine.h"
#include "../file_system/virtual_file_system.h"
#include "../virtual_console.h"
#include "../file_system/path.h"


void MdCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::vector<std::string> dirNames;
	std::set<std::string> options;

	cmdParser.parseArg(options, dirNames);

	if(dirNames.size() != 1)
	{
		output << "�����﷨����ȷ\n\n";
		return;
	}

	const std::string& path = dirNames[0];

	std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), path);
	
	if(!VirtualFileSystem::getInstPtr()->createDirectory(absPath))
	{
		output << "��Ŀ¼���ļ� " << path << " �Ѿ�����" << std::endl;
	}
}