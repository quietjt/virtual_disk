#include "glob.h"
#include "string_utils.h"
#include "../file_system/virtual_file_system.h"
#include "../file_system/path.h"

void Glob::search(const std::string& pattern, std::vector<std::string>& outValue)
{//TODO 磁盘路径通配符的实现
	outValue.clear();

	if(Path::isVirtualPath(pattern))
	{
		std::string dirName = Path::getDirectoryName(pattern);
		VirtualNode* node = VirtualFileSystem::getInstPtr()->getNode(dirName);

		if(node == NULL || node->getMode() != VirtualNode::Directory)
			return;

		std::string pathName = Path::getPathName(pattern);
		std::string lowerPathName = StringUtils::getLower(pathName);

		VirtualDirectory* dir =  (VirtualDirectory*) node;

		for(VirtualDirectory::Iterator it = dir->begin(); it != dir->end(); it++)
		{
			const std::string& key = it->first;
			if(isMatch(lowerPathName, key))
			{
				std::string fullPath = Path::join(dirName, it->second->getName(), Path::InnerSeparator());
				outValue.push_back(fullPath);
			}
		}
	}
	else
	{
		outValue.push_back(pattern);
	}
}

bool Glob::isMatch(const std::string& pattern, const std::string& value)
{
	std::string::const_iterator patternIt = pattern.begin();
	std::string::const_iterator valueIt = value.begin();

	bool isMatch = true;

	for(;  patternIt != pattern.end(); patternIt++, valueIt++)
	{
		if(*patternIt == '?')
		{
			continue;
		}
		else if(*patternIt == '*')
		{
			while(*patternIt == '*')
				patternIt++;

			if(patternIt == pattern.end())
			{
				isMatch = true;
				break;
			}

			for(; valueIt != value.end(); valueIt++)
			{
				if(*valueIt == *patternIt)
					break;
			}

			if(valueIt == value.end())
			{
				isMatch = false;
				break;
			}
		}
		else if(*patternIt != *valueIt)
		{
			isMatch = false;
			break;
		}
	}

	return isMatch;
}