#include "copy_tool.h"
#include "virtual_file_system.h"
#include "virtual_directory.h"
#include "virtual_file.h"
#include "path.h"
#include "../utils/glob.h"

CopyTool::CopyTool(VirtualFileSystem* pFS, const std::string& srcGlob, const std::string& destPath) : 
		m_pFS(pFS), 
		m_srcGlob(srcGlob), 
		m_destPath(destPath), 
		m_pendingSrcFile(NULL),
		m_destDir(NULL), 
		m_error(CopyTool::None), 
		m_sameFileName("")
{

}

CopyTool::~CopyTool()
{
	if(m_fileStream.is_open())
		m_fileStream.close();
}

void CopyTool::prepare()
{
	m_totalCopyFile = 0;
	// src
	Glob::search(m_srcGlob, m_srcPaths);
	m_srcPathIt = m_srcPaths.begin();

	// dest
	if(Path::isDrive(m_destPath))
	{
		m_destDir =  (VirtualDirectory*) m_pFS->getNode(m_destPath);
	}
	else
	{
		std::string parentDirName = Path::getDirectoryName(m_destPath);
		std::string pathName = Path::getPathName(m_destPath);

		VirtualNode* parentNode  = m_pFS->getNode(parentDirName);
		if(parentNode == NULL || parentNode->getMode() != VirtualNode::Directory)
		{
			m_error = FileNotExists;
			m_srcPathIt = m_srcPaths.end();
			return;
		}

		VirtualDirectory* parentDir = (VirtualDirectory*) parentNode;

		VirtualNode* node = parentDir->getChild(pathName);
		if(node != NULL && node->getMode() == VirtualNode::Directory)
		{
			m_destDir = (VirtualDirectory*)node;
		}
		else
		{
			m_destDir = parentDir;
		}
	}
}

void CopyTool::overrideCurrent()
{
	std::string pathName = Path::getPathName(m_destPath);

	m_destDir->removeChild(pathName);

	if(!m_isCurVirtual)
	{
		m_destDir->createFileFromDisk(pathName, m_fileStream);
	}
	else
	{
		m_destDir->createFileFromFileNode(pathName, m_pendingSrcFile);
	}

	m_error = CopyTool::None;
	m_totalCopyFile++;
	m_srcPathIt++;
}

void CopyTool::skipCurrent()
{
	m_error = CopyTool::None;

	if(!m_isCurVirtual)
	{
		if(m_fileStream.is_open())
			m_fileStream.close();
	}

	m_srcPathIt++;
}

bool CopyTool::haveNext()
{
	return m_srcPathIt != m_srcPaths.end();
}

bool CopyTool::execute()
{
	if(m_error !=  CopyTool::None)
		return false;

	if(m_srcPathIt == m_srcPaths.end())
		return true;

	const std::string& srcPath = *m_srcPathIt;

	m_isCurVirtual = Path::isVirtualPath(srcPath);
	if(!m_isCurVirtual)
	{
		std::string realPath = Path::getRealPath(srcPath);
	
		if(m_fileStream.is_open())
			m_fileStream.close();

		m_fileStream.open(realPath.c_str(), std::ios::binary | std::ios::in);
		if(!m_fileStream)
		{
			m_error = FileNotExists;
			m_fileStream.close();
			return false;
		}

		std::string pathName = Path::getPathName(m_destPath);
		if(m_destDir->contains(pathName))
		{
			m_error = CopyTool::SameFile;
			m_sameFileName = m_destPath;
			return false;
		}

		m_destDir->createFileFromDisk(pathName, m_fileStream);
		m_fileStream.close();
	}
	else
	{
		VirtualNode* srcNode = m_pFS->getNode(srcPath);
		if(srcNode == NULL)
		{
			m_error = CopyTool::FileNotExists;
			return false;
		}
		else if(srcNode->getMode() == VirtualNode::Directory)
		{
			m_error = CopyTool::SrcIsDir;
			return false;
		}

		std::string pathName = Path::getPathName(srcPath);
		if(m_destDir->contains(pathName))
		{
			m_error = CopyTool::SameFile;
			m_sameFileName = m_destPath;
			m_pendingSrcFile = (VirtualFile*) srcNode;
			return false;
		}

		VirtualFile* fileNode = (VirtualFile*)srcNode;
		m_destDir->createFileFromFileNode(pathName, fileNode);
	}

	m_totalCopyFile++;
	m_srcPathIt++;
	return true;
}

std::string CopyTool::getSameFileName()
{
	return m_sameFileName;
}