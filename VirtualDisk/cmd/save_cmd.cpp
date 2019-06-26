#include "save_cmd.h"
#include "../file_system/virtual_file_system.h"
#include "../file_system/path.h"
#include "../serialize/text_write_archive.h"

void SaveCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> optionals;
	std::vector<std::string> positionalOptionals;

	cmdParser.parseArg(optionals, positionalOptionals);

	if(positionalOptionals.size() != 1)
	{
			output << "命令语法不正确" << std::endl;
			return;
	}

	const std::string& path = positionalOptionals[0];

	if(Path::isVirtualPath(path))
	{
		output << "请使用磁盘路径" << std::endl;
		return;
	}

	TextWriteArchive ar;

	const std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), path);
	const std::string absRealPath = Path::getRealPath(absPath);

	if(!ar.open(absRealPath))
	{
		output << path << "不存在" << std::endl;
		return;
	}
	VirtualFileSystem::getInstPtr()->save(ar);
	ar.close();
}