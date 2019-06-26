#include "virtual_directory.h"
#include "virtual_file.h"
#include "virtual_soft_link.h"
#include "virtual_file_system.h"
#include "../utils/string_utils.h"
#include <sstream>
#include <queue>

VirtualDirectory::VirtualDirectory() : VirtualNode(), m_dirType(VirtualDirectory::Dir_Normal)
{
}

VirtualDirectory::~VirtualDirectory()
{
	if(m_dirType != Dir_Special)
	{
		for(std::map<std::string, VirtualNode *>::iterator it = m_nodes.begin(); it != m_nodes.end(); it++)
		{
			delete it->second;
		}
	}
	m_nodes.clear();
}

VirtualDirectory* VirtualDirectory::createRoot()
{
	VirtualDirectory* dir = new VirtualDirectory();
	dir->m_dirType = Dir_Root;
	dir->m_name = "root";
	return dir;
}

VirtualDirectory* VirtualDirectory::createDrive(const std::string& name)
{
	VirtualDirectory* dir = createDirectoryInternal(name);
	dir->m_dirType = Dir_Disk;
	return dir;
}

VirtualDirectory* VirtualDirectory::createDirectory(const std::string& name)
{
	VirtualDirectory* dir = createDirectoryInternal(name);
	if(dir)
	{
		VirtualDirectory* dotDir = dir->createDirectoryInternal(".");
		dotDir->m_dirType = Dir_Special;
		//dotDir->addChild(dir);
		VirtualDirectory* dotdotDir = dir->createDirectoryInternal("..");
		dotdotDir->m_dirType = Dir_Special;
		//dotdotDir->addChild(this);
	}

	return dir;
}

VirtualSoftLink* VirtualDirectory::createSoftLink(const std::string& name, const std::string& targetLink)
{
	if(targetLink == "")
		return NULL;
	VirtualSoftLink* softLink = createInternal<VirtualSoftLink, VirtualNode::SoftLink>(name);
	softLink->setTargetLinkPath(targetLink);

	return softLink;
}


VirtualFile* VirtualDirectory::createFileFromDisk(const std::string& name, std::fstream& stream)
{
	VirtualFile* file = createInternal<VirtualFile, VirtualNode::File>(name);
	file->setContentFromStream(stream);
	return file;
}

VirtualFile* VirtualDirectory::createFileFromFileNode(const std::string& name, const VirtualFile* otherFile)
{
	VirtualFile* file = createInternal<VirtualFile, VirtualNode::File>(name);
	file->copyFrom(otherFile);
	file->setName(name);
	return file;
}

VirtualDirectory* VirtualDirectory::createDirectoryInternal(const std::string& name)
{
	return createInternal<VirtualDirectory, VirtualNode::Directory>(name);
}

VirtualNode* VirtualDirectory::createNodeInternal(const std::string& name, VirtualNode::Mode mode)
{
	std::map<std::string, VirtualNode *>::iterator it = m_nodes.find(name);
	if(it != m_nodes.end() && it->second->getMode() == mode)
		return NULL;

	VirtualNode* node;

	switch(mode)
	{
	case VirtualNode::Directory:
		{
			VirtualDirectory* vd = new VirtualDirectory();
			node = (VirtualNode*) vd;
		}
		break;
	case VirtualNode::File:
		{
			node = (VirtualNode*)new VirtualFile();
		}
		break;
	case VirtualNode::SoftLink:
		{
			node = (VirtualNode*) new VirtualSoftLink();
		}
		break;
	}

	node->setName(name);

	return node;
}

void VirtualDirectory::removeChildrenByMode(VirtualNode::Mode mode)
{
	std::map<std::string, VirtualNode *>::iterator it = m_nodes.begin(); 

	while(it != m_nodes.end())
	{
		VirtualNode* node = it->second;

		if(m_dirType == Dir_Special)
			continue;

		if(node->getMode() == mode)
		{
			it = m_nodes.erase(it);
			continue;;
		}

		it++;
	}
}


bool VirtualDirectory::renameChild(const std::string& name, const std::string& newName)
{
	std::string newLowerName = StringUtils::getLower(newName);
	std::map<std::string, VirtualNode *>::iterator newIt = m_nodes.find(newLowerName);
	if(newIt != m_nodes.end())
		return false;

	std::string lowerName = StringUtils::getLower(name);
	std::map<std::string, VirtualNode *>::iterator it = m_nodes.find(lowerName);

	if(it == m_nodes.end())
		return false;

	VirtualNode* node = it->second;
	node->setName(newName);

	m_nodes.erase(it);
	m_nodes.insert(std::make_pair(newLowerName, node));

	return true;
}

bool VirtualDirectory::contains(const std::string& name)
{
	std::string lowerName = StringUtils::getLower(name);
	std::map<std::string, VirtualNode *>::iterator it = m_nodes.find(lowerName);

	if(it != m_nodes.end())
		return true;

	return false;
}

bool VirtualDirectory::addChild(VirtualNode* node)
{
	if(m_dirType == Dir_Special)
		return false;

	const std::string& name = node->getName();
	std::string lowerName = StringUtils::getLower(name);
	if(contains(name))
	{
		return false;
	}
	m_nodes.insert(std::make_pair(lowerName, node));
	return true;
}

bool VirtualDirectory::removeChild(const std::string& name)
{
	std::string lowerName = StringUtils::getLower(name);
	std::map<std::string, VirtualNode *>::iterator it = m_nodes.find(lowerName);
	if(it == m_nodes.end())
		return false;

	if(it->second->getMode() == VirtualNode::Directory)
	{
		VirtualDirectory* dir = (VirtualDirectory*)it->second;
		if(dir->m_dirType == Dir_Special)
			return false;
	}

	delete it->second;
	it->second = NULL;

	m_nodes.erase(it);
	return true;
}

bool VirtualDirectory::moveNode(const std::string& childName, VirtualDirectory* destDir, const std::string& newName)
{
	std::string lowerName = StringUtils::getLower(childName);
	std::map<std::string, VirtualNode *>::iterator it = m_nodes.find(lowerName);
	if(it == m_nodes.end())
		return false;

	if(destDir->contains(newName))
		return false;

	VirtualNode* node = it->second;
	m_nodes.erase(it);

	node->setName(newName);
	destDir->addChild(node);

	return true;
}

VirtualNode* VirtualDirectory::getChild(const std::string& name)
{
	std::string lowerName = StringUtils::getLower(name);
	std::map<std::string, VirtualNode *>::iterator it = m_nodes.find(lowerName);

	if(it == m_nodes.end())
		return NULL;

	return it->second;
}



int VirtualDirectory::getChildCount()
{
	return m_nodes.size();
}

VirtualDirectory::Iterator VirtualDirectory::begin()
{
	return m_nodes.begin();
}

VirtualDirectory::Iterator VirtualDirectory::end()
{
	return m_nodes.end();
}

bool VirtualDirectory::isEmpty()
{
	if(m_dirType == Dir_Disk || m_dirType == Dir_Root)
	{
		return getChildCount() == 0;
	}
	else
	{
		return getChildCount() == 2;
	}
}

void VirtualDirectory::save(IArchive& ar)
{
	ar.serialize((ISerializable*)this);

	std::queue<VirtualDirectory*> dirs;
	dirs.push(this);

	while(!dirs.empty())
	{
		VirtualDirectory* curDir = dirs.front();

		for(VirtualDirectory::Iterator it = curDir->begin(); it != curDir->end(); it++)
		{
			VirtualNode* node = it->second;

			ar.serialize((ISerializable*)  node);

			if(node->getMode() == VirtualNode::Directory)
			{
				VirtualDirectory* dir = (VirtualDirectory*)node;
				if(dir->getDirType() != Dir_Special)
				{
					dirs.push((VirtualDirectory*)dir);
				}
			}
		}

		dirs.pop();
	}
}

void VirtualDirectory::load(IArchive& ar)
{
	int childCount = 0;
	ar.serialize(childCount, "child_count");
	ar.serialize((ISerializable*) this);

	std::queue<std::pair<VirtualDirectory*, int>> dirs;
	dirs.push(std::make_pair(this, childCount));

	while(!dirs.empty())
	{
		std::pair<VirtualDirectory*, int> p = dirs.front();
		VirtualDirectory* curDir = p.first;
		childCount = p.second;
		
		for(int i = 0; i < childCount; i++)
		{
			std::string modeStr;
			ar.serialize(modeStr, "mode");

			VirtualNode::Mode mode = VirtualNode::strToMode(modeStr);
			VirtualNode* node = createNodeInternal("", mode);

			// 反序列化
			if(node->getMode() == VirtualNode::Directory)
			{
				int subChildCount = 0;
				ar.serialize(subChildCount, "child_count");
				ar.serialize((ISerializable*)node);

				VirtualDirectory* subDir = (VirtualDirectory*)node;
				dirs.push(std::make_pair(subDir, subChildCount));
				
				//if(node->getName() == ".")
				//{
				//	subDir->addChild(subDir);
				//}
				//else if(node->getName() == "..")
				//{
				//	subDir->addChild(curDir);
				//}
				//else
				//{
				//	dirs.push(std::make_pair(subDir, subChildCount));
				//}
			}
			else
			{
				ar.serialize((ISerializable*)node);
			}

			// 添加
			curDir->addChild(node);
		}
		dirs.pop();
	}
}

void VirtualDirectory::serialize(IArchive& ar, const std::string& name)
{
	std::string modeStr = modeToStr(getMode());
	ar.serialize(modeStr, "mode");

	int childCount = 0;
	if(m_dirType != Dir_Special)
	{
		childCount = getChildCount();
	}

	ar.serialize(childCount, "child_count");
	ar.serialize(m_name, "name");
	ar.serialize(m_createTimeStamp, "create_timestamp");

	int dirType = (int)m_dirType;
	ar.serialize(dirType, "dir_type");
}

void VirtualDirectory::deserialize(IArchive& ar, const std::string& name /*= ""*/)
{
	VirtualNode::deserialize(ar);
	int dirType = 0;
	ar.serialize(dirType, "dir_type");
	m_dirType = (DirType)dirType;
}
