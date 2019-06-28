#include "cd_cmd.h"
#include "../predefine.h"
#include "../file_system/path.h"
#include "../file_system/virtual_node_info.h"
#include "../utils/glob.h"

CdCmd::CdCmd() : m_isSupportGlob(false)
{

}

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
		output << "系统找不到指定路径" << std::endl;
	}
	else
	{
		const std::string cdPath = paths[0];
		std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), cdPath);

		if(m_isSupportGlob)
		{
			std::vector<std::string> matchedPath;
			Glob::search(absPath, matchedPath);

			if(matchedPath.size() <= 0)
			{
				output << "文件名、目录名或卷标语法不正确。" << std::endl;
				return;
			}

			absPath = matchedPath[0];
		}

		VirtualNodeInfo info(absPath);
		if(!info.isExist())
		{
			output << "系统找不到指定路径" << std::endl;
		}
		else if(info.isFile())
		{
			output << "目录名称无效" << std::endl;
		}
		else if(info.isSoftlink())
		{
			VirtualNodeInfo targetInfo = info.getFinalTargetLinkInfo();
			if(!targetInfo.isExist())
			{
				output << "系统找不到指定的文件" << std::endl;
			}
			else if(targetInfo.isFile())
			{
				output << "目录名称无效" << std::endl;
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