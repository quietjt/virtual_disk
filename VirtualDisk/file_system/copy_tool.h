#pragma once

#include "../predefine.h"

#include <fstream>

class VirtualFileSystem;
class VirtualNode;
class VirtualDirectory;
class VirtualFile;

class CopyTool
{
public:
	enum Error
	{
		None,
		FileNotExists,
		SameFile,
		SrcIsDir,
	};

	CopyTool(VirtualFileSystem* pFS, const std::string& srcGlob, const std::string& destPath);	
	
	~CopyTool();

	void prepare();

	bool execute();

	bool haveNext();

	Error getError() { return m_error; }

	std::string getSameFileName();

	void overrideCurrent();

	int getTotalCopyFile() { return m_totalCopyFile; }

	void skipCurrent();
private:
	std::string m_srcGlob;
	std::string m_destPath;
	VirtualFileSystem* m_pFS;
	Error m_error;

	VirtualFile* m_pendingSrcFile;
	std::string m_sameFileName;

	int m_totalCopyFile;

	VirtualDirectory* m_destDir;
	std::vector<std::string> m_srcPaths;

	std::vector<std::string>::iterator m_srcPathIt;
	bool m_isCurVirtual;

	std::fstream m_fileStream;
};