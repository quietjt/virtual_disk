#include "virtual_file_system.h"
#include "path.h"
#include <fstream>

void VirtualFileSystem::initWithDriveName(const std::string& rootDir)
{
	VirtualFileSystem::init();
	getInstPtr()->createDefaultDrive(rootDir);
}

void VirtualFileSystem::createDefaultDrive(const std::string& name)
{
	m_rootDir = VirtualDirectory::createRoot();
	m_defaultDriveName = name;
	m_rootDir->createDrive(m_defaultDriveName);
}

VirtualNode* VirtualFileSystem::getNode(const std::string& path)
{
	std::vector<std::string> pathlets;
	Path::split(path, pathlets);

	VirtualNode* curNode = (VirtualNode*) m_rootDir;

	for(std::vector<std::string>::iterator it = pathlets.begin(); it != pathlets.end(); it++)
	{
		if(curNode->getMode() != VirtualNode::Directory)
			return NULL;

		VirtualDirectory* dir = (VirtualDirectory*) curNode;
		curNode = dir->getChild(*it);
	
		if(curNode == NULL)
			break;
	}

	return curNode;
}

bool VirtualFileSystem::createDirectory(const std::string& path)
{
	std::vector<std::string> pathlets;
	Path::split(path, pathlets);

	VirtualDirectory* curDir = m_rootDir;
	for(std::vector<std::string>::iterator it = pathlets.begin(); it != pathlets.end(); it++)
	{
		if(*it == "")
			return false;

		VirtualNode* subNode = (VirtualNode*) curDir->getChild(*it);
		if(subNode != NULL)
		{
			if( subNode->getMode() == VirtualNode::Directory)
			{
				curDir = (VirtualDirectory*) subNode;
				continue;
			}
			else
			{
				return false;
			}
		}
		
		VirtualDirectory* subDir = curDir->createDirectory(*it);
		if(subDir == NULL)
			return false;

		curDir = subDir;
	}

	return true;
}

bool VirtualFileSystem::createSoftLink(const std::string& softLinkPath, const std::string& targetPath)
{
	std::string parentDirName = Path::getDirectoryName(softLinkPath);
	std::string pathName = Path::getPathName(softLinkPath);

	VirtualNode* parentNode = getNode(parentDirName);
	if(parentNode == NULL || parentNode->getMode() != VirtualNode::Directory)
		return false;

	VirtualDirectory* dir = (VirtualDirectory*)parentNode;

	if(dir->contains(pathName))
		return false;

	VirtualSoftLink* softLink = dir->createSoftLink(pathName, targetPath);

	return softLink == NULL ? false : true;
}

bool VirtualFileSystem::move(const std::string& srcDirName, const std::string& srcPathName , 
		  const std::string& destDirName, const std::string& destPathName)
{
	VirtualDirectory* srcDir = (VirtualDirectory*) getNode(srcDirName);
	if(srcDir == NULL || srcDir->getMode() != VirtualNode::Directory)
		return false;

	VirtualDirectory* destDir = (VirtualDirectory*) getNode(destDirName);
	if(destDir == NULL || destDir->getMode() != VirtualNode::Directory)
		return false;

	return srcDir->moveNode(srcPathName, destDir, destPathName);
}

bool VirtualFileSystem::rename(const std::string& path, const std::string& newName)
{
	std::string parentDirName = Path::getDirectoryName(path);
	std::string pathName = Path::getPathName(path);

	VirtualNode* parentNode = getNode(parentDirName);

	if(parentNode == NULL || parentNode->getMode() != VirtualNode::Directory)
		return false;
	
	VirtualDirectory* dir = (VirtualDirectory*)parentNode;
    return dir->renameChild(pathName, newName);
}


bool VirtualFileSystem::remove(const std::string& path)
{
	std::string parentDirName = Path::getDirectoryName(path);
	std::string pathName = Path::getPathName(path);

	VirtualNode* parentNode = getNode(parentDirName);

	if(parentNode == NULL || parentNode->getMode() != VirtualNode::Directory)
		return false;

	VirtualDirectory* dir = (VirtualDirectory*)parentNode;
	return dir->removeChild(pathName);
}

bool VirtualFileSystem::removeAllFilesInDir(const std::string& path)
{
	VirtualNode* node = getNode(path);

	if(node == NULL || node->getMode() != VirtualNode::Directory)
		return false;

	VirtualDirectory* dir = (VirtualDirectory*)node;

	dir->removeChildrenByMode(VirtualNode::File);
	dir->removeChildrenByMode(VirtualNode::SoftLink);

	return true;
}

void VirtualFileSystem::save(IArchive& ar)
{
	ar.serialize(m_defaultDriveName, "default_drive");
	m_rootDir->save(ar);
}

bool VirtualFileSystem::load(IArchive& ar)
{
	delete m_rootDir;
	m_defaultDriveName = "";

	ar.serialize(m_defaultDriveName, "default_drive");

	std::string modeStr;
	ar.serialize(modeStr, "mode");

	VirtualNode::Mode mode = VirtualNode::strToMode(modeStr);
	//if(mode != VirtualNode::Directory)
	//{
	//	std::cout << "文件不正确，根必须是目录类型" << std::endl;
	//	return false;
	//}

	m_rootDir = VirtualDirectory::createRoot();
	m_rootDir->load(ar);

	return true;
}