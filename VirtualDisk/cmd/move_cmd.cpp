#include "move_cmd.h"
#include "../file_system/virtual_file_system.h"
#include "../file_system/virtual_node_info.h"
#include "../file_system/path.h"

void MoveCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> optionals;
	std::vector<std::string> positionalOptionals;

	cmdParser.parseArg(optionals, positionalOptionals);

	if(positionalOptionals.size() != 2)
	{
		output << "命令语法不正确" << std::endl;
		return;
	}

	std::string& srcPath = positionalOptionals[0];
	std::string absSrcPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), srcPath);
	if(!Path::exists(absSrcPath))
	{
		output << "系统找不到指定的路径" << std::endl;
		return;
	}

	std::string srcDirName = Path::getDirectoryName(absSrcPath);
	std::string srcPathName = Path::getPathName(absSrcPath);

	std::string& destPath = positionalOptionals[1];
	
	std::string destDirName;
	std::string destPathName;
		
	std::string absDestPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), destPath);
	std::string::iterator lastIt = absDestPath.end() - 1;

	if(*lastIt == '\\' || *lastIt == '/')
	{
		destDirName = Path::getDirectoryName(absDestPath);
		destPathName = srcPathName;
	}
	else
	{
		VirtualNodeInfo nodeInfo(absDestPath);

		if(nodeInfo.isSoftlink())
		{
			VirtualNodeInfo finalTargetLink = nodeInfo.getFinalTargetLinkInfo(virtualConsole.getCurDirectoryPath());
			if(finalTargetLink.isDirectory())
			{
				std::string finalTargetLinkName = finalTargetLink.getName();
				destDirName = Path::join(virtualConsole.getCurDirectoryPath(), finalTargetLinkName, Path::InnerSeparator());
				destPathName = srcPathName;
			}
		}
		else
		{
			destDirName = Path::getDirectoryName(absDestPath);
			destPathName = Path::getPathName(absDestPath);
		}
	}

	if(!Path::exists(destDirName))
	{
		output << "系统找不到指定的路径" << std::endl;
		return;
	}
	
	std::string fullPath = Path::join(destDirName, destPathName, Path::InnerSeparator());

	if(Path::exists(fullPath))
	{
		Path::transformSeparator(fullPath, virtualConsole.getDisplaySeparator());
		output << "已存在文件 " << fullPath << std::endl;
		return;
	}

	VirtualFileSystem::getInstPtr()->move(srcDirName, srcPathName,  destDirName, destPathName);
}