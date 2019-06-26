#include "string_utils.h"

#include <algorithm>

std::string StringUtils::getLower(const std::string& value)
{
	std::string lowerValue = value;
	std::transform(value.begin(), value.end(), lowerValue.begin(), ::tolower);
	return lowerValue;
}