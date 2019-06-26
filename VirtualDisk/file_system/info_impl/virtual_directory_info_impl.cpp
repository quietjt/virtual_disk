#include "virtual_directory_info_impl.h"

VirtualNodeInfo::Iterator VirtualDirectoryInfoImpl::begin()
{
	return VirtualNodeInfo::Iterator(m_dir->begin());
}

VirtualNodeInfo::Iterator VirtualDirectoryInfoImpl::end()
{
	return VirtualNodeInfo::Iterator(m_dir->end());
}

bool VirtualDirectoryInfoImpl::isEmpty()
{
	return m_dir->isEmpty();
}

bool VirtualDirectoryInfoImpl::isSpecialDirectory()
{
	return m_dir->getDirType() == VirtualDirectory::Dir_Special;
}
