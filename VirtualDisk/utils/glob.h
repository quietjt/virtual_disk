#pragma once
#include "../predefine.h"

class Glob
{
public:
	static void search(const std::string& pattern, std::vector<std::string>& outValue);

private:
	static bool isMatch(const std::string& pattern, const std::string& value);
};