#include "cd_cmd.h"
#include "../predefine.h"
#include "../file_system/path.h"
#include "../file_system/virtual_node_info.h"

void CdCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::vector<std::string> paths;
	std::string path;
	while(true)
	{
		cmdParser.getNext(&path);
		if(path == "")
		{
			break;
		}

		paths.push_back(path);
	}
	
	std::ostream& output = virtualConsole.getOutput();
	if(paths.size() == 0)
	{
		const std::string& curDirPathName = virtualConsole.getCurDirectoryPath();
		output << curDirPathName;

		if(Path::isDrive(curDirPathName))
			output << virtualConsole.getDisplaySeparator();

		output << std::endl;
	}
	else if(paths.size() > 1)
	{
		output << "ϵͳ�Ҳ���ָ��·��" << std::endl;
	}
	else
	{
		const std::string cdPath = paths[0];
		std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), cdPath);
	
		VirtualNodeInfo info(absPath);
		if(!info.isExist())
		{
			output << "ϵͳ�Ҳ���ָ��·��" << std::endl;
		}
		else if(info.isFile())
		{
			output << "Ŀ¼������Ч" << std::endl;
		}
		else if(info.isSoftlink())
		{
			VirtualNodeInfo targetInfo = info.getFinalTargetLinkInfo();
			if(!targetInfo.isExist())
			{
				output << "ϵͳ�Ҳ���ָ�����ļ�" << std::endl;
			}
			else if(targetInfo.isFile())
			{
				output << "Ŀ¼������Ч" << std::endl;
			}
			else
			{
				std::string targetPath = info.getFullPath();
				Path::transformSeparator(targetPath, virtualConsole.getDisplaySeparator());
				virtualConsole.setCurDirectoryPath(targetPath);
			}
		}
		else
		{
			Path::transformSeparator(absPath, virtualConsole.getDisplaySeparator());
			virtualConsole.setCurDirectoryPath(absPath);
		}
	}
}