#include "virtual_soft_link_info_impl.h"
#include "../path.h"

VirtualNodeInfo VirtualSoftLinkInfoImpl::getFinalTargetLinkInfo(const std::string& curPath)
{
	std::string targetLinkPath = getTargetLinkPath();
	std::string absPath = Path::getAbsPath(curPath, targetLinkPath);
	VirtualNodeInfo info(absPath);

	while(info.isSoftlink())
	{
		targetLinkPath = info.getTargetLinkPath();
		absPath = Path::getAbsPath(curPath, targetLinkPath);
		info = VirtualNodeInfo(absPath);
	}

	return info;
}

