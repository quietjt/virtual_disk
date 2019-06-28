#include "move_cmd.h"
#include "../file_system/virtual_file_system.h"
#include "../file_system/virtual_node_info.h"
#include "../file_system/path.h"

void MoveCmd::registerOptions()
{
	m_supportOptions.insert("y");
}

void MoveCmd::execute(CommandParser& cmdParser, VirtualConsole& virtualConsole)
{
	std::ostream& output = virtualConsole.getOutput();

	std::set<std::string> optionals;
	std::vector<std::string> positionalOptionals;

	cmdParser.parseArg(optionals, positionalOptionals);

	if(!checkOptions(output, optionals))
	{
		return;
	}

	if(positionalOptionals.size() != 2)
	{
		output << "�����﷨����ȷ" << std::endl;
		return;
	}

	std::string& srcPath = positionalOptionals[0];
	std::string absSrcPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), srcPath);
	if(!Path::exists(absSrcPath))
	{
		output << "ϵͳ�Ҳ���ָ����·��" << std::endl;
		return;
	}

	std::string srcDirName = Path::getDirectoryName(absSrcPath);
	std::string srcPathName = Path::getPathName(absSrcPath);

	std::string& destPath = positionalOptionals[1];
	
	std::string destDirName;
	std::string destPathName;
		
	std::string absDestPath = Path::getAbsPath(virtualConsole.getCurDirectoryPath(), destPath);
	
	std::string::iterator lastIt = destPath.end() - 1;

	VirtualNodeInfo nodeInfo(absDestPath);
	if(nodeInfo.isSoftlink())
	{
		nodeInfo = nodeInfo.getFinalTargetLinkInfo();
	}

	if(*lastIt == '\\' || *lastIt == '/' || *lastIt == '.')
	{
		bool haveError = false;
		if(!nodeInfo.isExist())
		{
			output << "ϵͳ�Ҳ���ָ����·��" << std::endl;
			haveError = true;
		}
		else if(nodeInfo.isFile())
		{
			output << "��������" << std::endl;
			haveError = true;
		}

		if(haveError)
			return;

		destDirName = nodeInfo.getFullPath();
		destPathName = srcPathName;
	}
	else
	{
		if(nodeInfo.isDirectory())
		{
			destDirName = nodeInfo.getFullPath();
			destPathName = srcPathName;
		}
		else
		{
			destDirName = Path::getDirectoryName(absDestPath);
			destPathName = Path::getPathName(absDestPath);
		}
	}

	if(!Path::exists(destDirName))
	{
		output << "ϵͳ�Ҳ���ָ����·��" << std::endl;
		return;
	}
	
	std::string fullPath = Path::join(destDirName, destPathName, Path::InnerSeparator());

	bool isForceOverride = (optionals.find("y") != optionals.end());

	bool isOverride = false;

	if(isForceOverride)
	{
		isOverride = true;
	}
	else
	{
		if(Path::exists(fullPath))
		{
			Path::transformSeparator(fullPath, virtualConsole.getDisplaySeparator());
			std::string line;
			while(true)
			{
				output << "���� " << fullPath << " ��? ��Yes/No����";
				virtualConsole.getLine(line);
				line = StringUtils::getLower(line);
				if(line == "yes")
				{
					isOverride = true;
					break;
				}
				else if(line == "no")
				{
					isOverride = false;
					return;
				}
			}
		}
	}

	if(isOverride)
	{
		VirtualFileSystem::getInstPtr()->remove(fullPath);
	}

	std::string finalSrcPath = Path::join(srcDirName, srcPathName, Path::InnerSeparator());
	std::string finalDestPath = Path::join(destDirName, destPathName, Path::InnerSeparator());

	VirtualFileSystem::getInstPtr()->move(finalSrcPath, finalDestPath);
}
