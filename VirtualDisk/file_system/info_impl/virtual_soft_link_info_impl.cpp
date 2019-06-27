#include "virtual_soft_link_info_impl.h"
#include "../path.h"
#include <algorithm>

VirtualNodeInfo VirtualSoftLinkInfoImpl::getFinalTargetLinkInfo()
{
	std::vector<std::string> tracks;

	std::string curDirPath = Path::getDirectoryName(this->getFullPath());

	std::string targetLinkPath = getTargetLinkPath();
	std::string absPath = Path::getAbsPath(curDirPath, targetLinkPath);

	VirtualNodeInfo info(absPath);

	std::vector<std::string>::iterator it = tracks.end();

	while(info.isSoftlink())
	{
		std::string linkFullPath = info.getFullPath();
		it = std::find(tracks.begin(), tracks.end(), linkFullPath);

		if(it == tracks.end())
		{
			targetLinkPath = info.getTargetLinkPath();
			absPath = Path::getAbsPath(curDirPath, targetLinkPath);
			info = VirtualNodeInfo(absPath);

			tracks.push_back(linkFullPath);
		}
		else
		{
			info = VirtualNodeInfo();
			break;
		}
	}

	return info;
}
