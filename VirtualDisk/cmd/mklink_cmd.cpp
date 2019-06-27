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
		output << "�����﷨����ȷ\n\n";
		return;
	}

	std::string targetPath = positionalOptions[0];
	std::string softAbsPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), positionalOptions[1]);

	std::string absTargetPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), positionalOptions[0]);
	if(!Path::exists(absTargetPath))
	{
		output << "���ӵ��ļ������ڣ� �޷��������ӡ�" << std::endl;
		return;
	}

	if(!VirtualFileSystem::getInstPtr()->createSoftLink(softAbsPath, targetPath))
	{
		output << "���ļ��Ѵ���ʱ���޷��������ļ���" << std::endl;
	}
}