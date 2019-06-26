#pragma once
#include "../predefine.h"

class Path
{
public:
	static char InnerSeparator()
	{
		return '/';
	}

	static void split(const std::string& path, std::vector<std::string>& outValue);

	static bool isVirtualPath(const std::string& value);
	
	static std::string getNormalizedPath(const std::string& value);
	
	static void transformSeparator(std::string& value, char separator);

	static bool isDrive(const std::string& value);

	static bool isAbsPath(const std::string& value);

	static bool exists(const std::string& absPath);

	/** @brief 标准化且转换成绝对路径
	 */
	static std::string getAbsPath(const std::string&curPath, const std::string& path);

	static std::string getDirectoryName(const std::string& value);

	static std::string getPathName(const std::string& value);

	static std::string getRealPath(const std::string& value);

	static std::string join(const std::string& dir, const std::string& fileName, char separator);
};