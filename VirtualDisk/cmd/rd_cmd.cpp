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
		output << "命令语法不正确" << std::endl;
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
				output << path << ", " << "是否确认（Y/N）？";
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
			output << "系统找不到指定的文件" << std::endl;
			haveError = true;
		}
		else if(nodeInfo.isSoftlink() || nodeInfo.isFile())
		{
			output << "目录名称无效" << std::endl;
			haveError = true;
		}

		if(!isDelFullDir && !nodeInfo.isEmpty())
		{
			output << "目录不是空的" << std::endl;
			haveError = true;
		}

		if(!haveError)
		{
			VirtualFileSystem::getInstPtr()->remove(absPath);
		}
	}

}