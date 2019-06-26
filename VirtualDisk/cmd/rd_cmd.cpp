#include "rd_cmd.h"
#include "../file_system/virtual_file_system.h"
#include "../file_system/path.h"
#include "../file_system/virtual_node_info.h"

void RdCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> optionals;
	std::vector<std::string> positionalOptionals;

	cmdParser.parseArg(optionals, positionalOptionals);

	bool isDelFullDir = (optionals.find("/s") != optionals.end());

	if(positionalOptionals.size() < 1)
	{
		output << "�����﷨����ȷ" << std::endl;
		return;
	}

	for(std::vector<std::string>::iterator it = positionalOptionals.begin(); it != positionalOptionals.end(); it++)
	{
		const std::string& path = *it;
		std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), path);
		
		if(isDelFullDir)
		{
			while(true)
			{
				output << path << ", " << "�Ƿ�ȷ�ϣ�Y/N����";
				std::string line;
				virtualConsole.getLine(line);
				if(line == "n" || line == "N")
					return;

				if(line == "Y" || line == "y")
					break;
			}
		}

		VirtualNodeInfo nodeInfo(absPath);

		bool haveError = false;

		if(!nodeInfo.isExist())
		{
			output << "ϵͳ�Ҳ���ָ�����ļ�" << std::endl;
			haveError = true;
		}
		else if(nodeInfo.isSoftlink() || nodeInfo.isFile())
		{
			output << "Ŀ¼������Ч" << std::endl;
			haveError = true;
		}

		if(!isDelFullDir && !nodeInfo.isEmpty())
		{
			output << "Ŀ¼���ǿյ�" << std::endl;
			haveError = true;
		}

		if(!haveError)
		{
			VirtualFileSystem::getInstPtr()->remove(absPath);
		}
	}

}