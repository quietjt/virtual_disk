#include "del_cmd.h"
#include "../file_system/path.h"
#include "../file_system/virtual_file_system.h"
#include "../file_system/virtual_node_info.h"
#include <queue>

void DelCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> optionals;
	std::vector<std::string> positionalOptionals;

	cmdParser.parseArg(optionals, positionalOptionals);

	bool isRecursive = (optionals.find("/s") != optionals.end());

	if(positionalOptionals.size() < 1)
	{
		output << "命令语法不正确\n";
		return;
	}

	bool isAllPathNotExists = true;

	std::vector<std::string> notExistPaths;

	for(std::vector<std::string>::iterator it = positionalOptionals.begin(); it != positionalOptionals.end(); it++)
	{
		const std::string& path = *it;
		std::string& absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), path);

		VirtualNodeInfo nodeInfo(absPath);
		if(nodeInfo.isExist())
		{
			isAllPathNotExists = false;
			if(nodeInfo.isFile() || nodeInfo.isSoftlink())
			{
				if(!isRecursive)
				{
					VirtualFileSystem::getInstPtr()->remove(absPath);
				}
				else
				{
					std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), path);
					std::string dirName = Path::getDirectoryName(absPath);
					std::string pathName = Path::getPathName(absPath);

					std::queue<std::string> dirs;
					dirs.push(dirName);

					std::vector<std::string> subDirs;
					while(!dirs.empty())
					{
						std::string& curDirName = dirs.front();

						std::string absPath = Path::join(curDirName, pathName, Path::InnerSeparator());
						if(VirtualFileSystem::getInstPtr()->remove(absPath))
						{
							output << "删除文件 -  " << absPath << std::endl;
						}

						VirtualNodeInfo info(curDirName);
						if(info.isDirectory())
						{
							for(VirtualNodeInfo::Iterator it = info.begin();  it != info.end(); it++)
							{
								VirtualNodeInfo subInfo = *it;
								if(subInfo.isDirectory() && !subInfo.isSpecialDirectory())
								{
									std::string subDir = Path::join(curDirName,  subInfo.getName(), Path::InnerSeparator());
									dirs.push(subDir);
								}
							}
						}
						
						dirs.pop();
					}
				}
			}
			else
			{
				std::string dirName = Path::join(absPath, "*", virtualConsole.getDisplaySeparator());
				while(true)
				{
					output << dirName << "，是否确认（Y/N）？";
					std::string line;
					virtualConsole.getLine(line);
					if(line == "Y" || line == "y")
					{
						VirtualFileSystem::getInstPtr()->removeAllFilesInDir(absPath);
						break;
					}
					else if(line == "N" || line == "n")
					{
						break;
					}
				}
			}
		}
		else
		{
			notExistPaths.push_back(absPath);
		}
	}

	if(isAllPathNotExists && notExistPaths.size() >= 1)
	{
		std::string notExistPath = notExistPaths[0];
		Path::transformSeparator(notExistPath, virtualConsole.getDisplaySeparator());
		output << "找不到 " << notExistPath << std::endl;
	}

}