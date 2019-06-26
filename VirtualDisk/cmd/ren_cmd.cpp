#include "ren_cmd.h"
#include "../file_system/path.h"
#include "../file_system/virtual_file_system.h"

void RenCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> optionals;
	std::vector<std::string> positionalOptionals;

	cmdParser.parseArg(optionals, positionalOptionals);

	if(positionalOptionals.size() != 2)
	{
		output << "�����﷨����ȷ" << std::endl;
		return;
	}

	const std::string& newName = positionalOptionals[1];
	size_t pos = newName.find(":/\\");
	if(pos != std::string::npos)
	{
		output << "�����﷨����ȷ" << std::endl;
		return;
	}

	const std::string& path = positionalOptionals[0];
	const std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), path);
	if(Path::isDrive(absPath))
	{
		output << "�����﷨����ȷ" << std::endl;
		return;
	}


	const std::string dirName = Path::getDirectoryName(absPath);
	if(!Path::exists(dirName))
	{
		output << "ϵͳ�Ҳ���ָ�����ļ�" << std::endl;
		return;
	}

	const std::string newPath = Path::join(dirName, newName, Path::InnerSeparator());
	if(Path::exists(newPath))
	{
		output << "����һ�������ļ��������Ҳ����ļ���" << std::endl;
		return;
	}

	if(!VirtualFileSystem::getInstPtr()->rename(absPath, newName))
	{
		output << "�����﷨����ȷ" << std::endl;
	}
}