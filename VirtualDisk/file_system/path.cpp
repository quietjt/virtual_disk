#include "path.h"
#include "../virtual_console.h"
#include "virtual_file_system.h"
#include <algorithm>
#include <stack>
#include <sstream>

void Path::split(const std::string& path, std::vector<std::string>& outValue)
{
	outValue.clear();
	std::string::const_iterator beg = path.begin();
	std::string::const_iterator cur = path.begin();

	for(; cur != path.end(); cur++)
	{
		if(*cur == '\\' || *cur == '/')
		{
			std::string pathlet = path.substr(beg - path.begin(), cur - beg);
			outValue.push_back(pathlet);
			cur++;
			beg = cur;

			if(cur == path.end())
				break;
		}
	}

	std::string pathlet = path.substr(beg - path.begin(), cur - beg);
	outValue.push_back(pathlet);
}

bool Path::isDrive(const std::string& value)
{
	if(value.size() <= 0)
		return false;
	std::string::const_iterator lastIt = value.end() - 1;
	return *lastIt == ':';
}

bool Path::isVirtualPath(const std::string& value)
{
	return !(value.size() > 1 && value[0] == '@');
}

std::string Path::getNormalizedPath(const std::string& value)
{
	std::string retValue = value;
	for(std::string::iterator it = retValue.begin(); it != retValue.end(); it++)
	{
		if(*it == '\\' || *it == '/')
		{
			*it = InnerSeparator();
		}
	}
	return retValue;
}

void Path::transformSeparator(std::string& value, char separator)
{
	for(std::string::iterator it = value.begin(); it != value.end(); it++)
	{
		if(*it == '\\' || *it == '/')
		{
			*it = separator;
		}
	}
}

bool Path::isAbsPath(const std::string& value)
{
	const std::string normalPath = getNormalizedPath(value);

	std::string::const_iterator it = normalPath.begin();

	// 查找':'
	for(; it != normalPath.end(); it++)
	{
		if(*it == '/' || *it == '.')
		{
			return false;
		}
		else if(*it == ':')
		{
			it++;
			break;
		}
	}

	// 查找路径中是否含有"/../"
	for(; it != normalPath.end(); it++)
	{
		if(*it == '/' && *(it+1) == '.' && 
			*(it+2) == '.' && *(it+3) == '/')
		{
			return false;
		}
	}

	return true;
}

bool Path::exists(const std::string& absPath)
{
	VirtualNode* node = VirtualFileSystem::getInstPtr()->getNode(absPath);
	return node == NULL ? false : true;
}

std::string Path::getAbsPath(const std::string&curPath, const std::string& path)
{
	if(path.size() <= 0)
		return "";

	std::vector<std::string> pathlets;
	split(path, pathlets);

	std::vector<std::string> absPathVec;

	if(pathlets[0] == ".")
	{
		split(curPath, absPathVec);
	}
	else if(pathlets[0] == "..")
	{
		split(curPath, absPathVec);
		if(absPathVec.size() > 1)
		{
			absPathVec.erase(absPathVec.end() - 1);
		}
	}
	else if(!isDrive(pathlets[0]))
	{
		split(curPath, absPathVec);
		absPathVec.push_back(pathlets[0]);
	}
	else
	{
		absPathVec.push_back(pathlets[0]);
	}

	for(std::vector<std::string>::iterator it = pathlets.begin() + 1; it != pathlets.end(); it++)
	{
		if(*it == ".")
		{
			continue;
		}
		else if(*it == "..")
		{
			if(absPathVec.size() > 1)
			{
				absPathVec.erase(absPathVec.end() - 1);
			}
		}
		else
		{
			absPathVec.push_back(*it);
		}
	}
	
	std::stringstream ss;

	for(std::vector<std::string>::iterator it = absPathVec.begin(); it != absPathVec.end(); it++)
	{
		ss << *it;
		if(it != absPathVec.end() - 1)
			ss << Path::InnerSeparator();
	}

	std::string absPath = ss.str();

	return absPath;
}

std::string Path::getDirectoryName(const std::string& value)
{
	std::size_t found = value.find_last_of("/\\");
	std::string ret = value.substr(0, found);
	return ret;
}

std::string Path::getPathName(const std::string& value)
{
	std::size_t found = value.find_last_of("/\\");

	if(found == std::string::npos)
		return "";

	std::string ret = value.substr(found + 1);
	return ret;
}

std::string Path::getRealPath(const std::string& value)
{
	std::string ret = value.substr(1);
	return ret;
}

std::string Path::join(const std::string& dir, const std::string& fileName , char separator)
{
	std::stringstream ss;
	ss << dir << separator << fileName;
	std::string ret = ss.str();
	return ret;
}

void Path::concate(std::string& outValue, const std::string& fileName , char separator)
{
	if(outValue != "")
	{
		outValue.push_back(separator);
	}
	outValue.append(fileName);
}