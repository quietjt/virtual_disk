#pragma once

#include "../predefine.h"
#include "../singleton.h"
#include "virtual_directory.h"
#include "../serialize/i_archive.h"

/**
 * VirtualFileSystem操作的路径都为绝对路径
 */

class VirtualFileSystem : public Singleton<VirtualFileSystem>
{
public:
	static void initWithDriveName(const std::string& rootDir);

	const std::string& getDefaultDriveName()
	{
		return m_defaultDriveName;
	}

	bool createDirectory(const std::string& path);

	bool createSoftLink(const std::string& softLinkPath, const std::string& targetPath);

	bool move(const std::string& srcDirName, const std::string& srcPathName , 
		const std::string& destDirName, const std::string& destPathName);

	bool rename(const std::string& path, const std::string& newName);

	bool remove(const std::string& path);

	bool removeAllFilesInDir(const std::string& path);

	// pathName如果是软链接，不进行进一步解析
	VirtualNode* getNode(const std::string& path);

	void save(IArchive& ar);

	bool load(IArchive& ar);

private:
	void createDefaultDrive(const std::string& rootDir);

	VirtualDirectory* getDirectoryNode(const std::string& path);

	// pathName如果是软链接，进行进一步解析
	VirtualNode* getNodeInner(const std::string& path);

private:
	VirtualDirectory* m_rootDir;
	std::string m_defaultDriveName;
};