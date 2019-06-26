#include "mklink_cmd.h"
#include "../file_system/path.h"
#include "../file_system/virtual_file_system.h"

void MklinkCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> options;
	std::vector<std::string> positionalOptions;

	cmdParser.parseArg(options, positionalOptions);

	if(positionalOptions.size() != 2)
	{
		output << "命令语法不正确\n\n";
		return;
	}

	std::string targetPath = positionalOptions[0];
	std::string softAbsPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), positionalOptions[1]);

	if(!VirtualFileSystem::getInstPtr()->createSoftLink(softAbsPath, targetPath))
	{
		output << "当文件已存在时，无法创建该文件。" << std::endl;
	}
}