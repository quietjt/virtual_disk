#include "dir_cmd.h"
#include "../file_system/path.h"
#include "../file_system/virtual_file_system.h"
#include "../virtual_console.h"
#include "../file_system/virtual_node_info.h"

#include <iostream>
#include <queue>

void DirCmd::outputNodeInfo(VirtualNodeInfo& nodeInfo, std::ostream& output)
{
	output << nodeInfo.getCreateDay() << "  " << nodeInfo.getCreateTime() << "  "
		<< nodeInfo.getMode()  << "        ";

	if(nodeInfo.isFile())
	{
		int size = nodeInfo.getSize();
		output << size << "  ";
	}

	output << nodeInfo.getName();

	if(nodeInfo.isSoftlink())
	{
		output << "  [" << nodeInfo.getTargetLinkPath() << "]";
	}

	output << "\n";
}

void DirCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::set<std::string> optionals;
	std::vector<std::string> positionalOptions;;

	cmdParser.parseArg(optionals, positionalOptions);

	bool isDeep = (optionals.find("/s") != optionals.end());
	bool isOnlyDir = (optionals.find("/ad") != optionals.end());

	std::vector<std::string> absPathNames;

	if(positionalOptions.size() == 0)
	{
		absPathNames.push_back(virtualConsole.getCurDirectoryPath());
	}
	else
	{
		for(std::vector<std::string>::iterator it = positionalOptions.begin(); it != positionalOptions.end(); it++)
		{
			const std::string& path = *it;
			std::string absPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), path);
			absPathNames.push_back(absPath);
		}
	}

	for(std::vector<std::string>::iterator it = absPathNames.begin(); it != absPathNames.end(); it++)
	{
		const std::string& curAbsPath = *it;

		if(isDeep)
		{
			displayPathDeep(curAbsPath, virtualConsole, isOnlyDir);
		}
		else
		{
			displayPath(curAbsPath, virtualConsole, isOnlyDir);
		}
	}
}

void DirCmd::displayPath(const std::string& absPath, VirtualConsole& virtualConsole, bool isOnlyDir)
{
	std::ostream& output = virtualConsole.getOutput();

	VirtualNodeInfo nodeInfo(absPath);

	if(!nodeInfo.isExist())
	{
		std::string dpath = absPath;
		Path::transformSeparator(dpath, virtualConsole.getDisplaySeparator());
		output << "找不到路径 : " << dpath << std::endl;
		return;
	}

	int totalSize = 0;
	int totalFile = 0;
	int totalDir = 0;

	std::string displayDir = absPath;
	Path::transformSeparator(displayDir, virtualConsole.getDisplaySeparator());
	output << " " << displayDir << virtualConsole.getDisplaySeparator() << " 的目录\n\n";


	if(nodeInfo.isSoftlink())
	{
		VirtualNodeInfo targetNodeInfo = nodeInfo.getFinalTargetLinkInfo();
		if(!targetNodeInfo.isExist())
		{
			output << "路径无效" << std::endl;
		}

		nodeInfo = targetNodeInfo;
	}

	if(nodeInfo.isFile())
	{
		displayNodeInfo(nodeInfo, output, isOnlyDir, &totalSize, &totalFile, &totalDir);
	}
	else if(nodeInfo.isDirectory())
	{
		if(nodeInfo.getChildCount() == 0)
		{
			output << "找不到文件\n";
			return;
		}
		else
		{
			for(VirtualNodeInfo::Iterator it = nodeInfo.begin(); it != nodeInfo.end(); it++)
			{
				VirtualNodeInfo subNodeInfo = *it;
				displayNodeInfo(subNodeInfo, output, isOnlyDir, &totalSize, &totalFile, &totalDir);
			}
		}
	}

	output << "                " << totalFile << "个文件" << std::endl;
	output << "                " << totalDir  << "个目录" << std::endl;
	output << std::endl;
}

void DirCmd::displayPathDeep(const std::string& absPath, VirtualConsole& virtualConsole, bool isOnlyDir)
{
	std::queue<std::string> dirs;
	dirs.push(absPath);

	while(!dirs.empty())
	{
		std::string curPath = dirs.front();
		displayPath(curPath, virtualConsole, isOnlyDir);

		VirtualNodeInfo nodeInfo(curPath);
		if(nodeInfo.isDirectory())
		{
			for(VirtualNodeInfo::Iterator it = nodeInfo.begin(); it != nodeInfo.end(); it++)
			{
				VirtualNodeInfo subNodeInfo = *it;
				if(subNodeInfo.isDirectory() && !subNodeInfo.isSpecialDirectory())
				{
					std::string absSubDirName = Path::join(curPath, subNodeInfo.getName(), Path::InnerSeparator());
					dirs.push(absSubDirName);
				}
			}
		}
		dirs.pop();
	}
}

void DirCmd::displayNodeInfo(VirtualNodeInfo& nodeInfo, std::ostream& output, bool isOnlyDir, int* totalSize, int *totalFile, int* totalDir)
{
	if(!isOnlyDir)
	{
		if(nodeInfo.isFile())
		{
			outputNodeInfo(nodeInfo, output);
			*totalFile += 1;
			*totalSize += nodeInfo.getSize();
		}
		else if(nodeInfo.isSoftlink())
		{
			outputNodeInfo(nodeInfo, output);
			*totalFile += 1;
		}
	}

	if(nodeInfo.isDirectory())
	{
		outputNodeInfo(nodeInfo, output);
		*totalDir += 1;
	}
}