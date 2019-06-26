#pragma once
#include "../predefine.h"

#include "virtual_node.h"
#include <fstream>

class VirtualFile;
class VirtualSoftLink;

class VirtualDirectory : public VirtualNode
{	
public:
	enum DirType 
	{
		Dir_Normal = 0, 
		Dir_Root,
		Dir_Disk,
		Dir_Special,
	};

	virtual ~VirtualDirectory();

	typedef std::map<std::string, VirtualNode *>::iterator Iterator;

	virtual Mode getMode() const
	{
		return VirtualNode::Directory;
	}

	inline DirType getDirType()
	{
		return m_dirType;
	}
	
	static VirtualDirectory* createRoot();

	VirtualDirectory* createDrive(const std::string& name);

	VirtualDirectory* createDirectory(const std::string& name);

	VirtualSoftLink* createSoftLink(const std::string& name, const std::string& targetLink);

	//TODO 先直接用stream，同时创建的时间不用硬盘上的。
	VirtualFile* createFileFromDisk(const std::string& name, std::fstream& stream);

	VirtualFile* createFileFromFileNode(const std::string& name, const VirtualFile* otherFile);

	bool renameChild(const std::string& name, const std::string& newName);

	bool contains(const std::string& name);

	VirtualNode* getChild(const std::string& name);

	bool moveNode(const std::string& childName, VirtualDirectory* destDir, const std::string& newName);

	void removeChildrenByMode(VirtualNode::Mode mode);

	bool removeChild(const std::string& name);

	int getChildCount();

	Iterator begin();

	Iterator end();

	bool isEmpty();

	void save(IArchive& ar);

	void load(IArchive& ar);

	virtual void serialize(IArchive& ar, const std::string& name = "");

	virtual void deserialize(IArchive& ar, const std::string& name = "");

protected:
	VirtualDirectory();
	VirtualDirectory(const VirtualDirectory& other) {}
	VirtualDirectory& operator=(const VirtualDirectory& other) {}

	bool addChild(VirtualNode* node);

private:
	template<typename T, VirtualNode::Mode mode>
	T* createInternal(const std::string& name)
	{
		if(contains(name))
			return NULL;

		T* node = (T*) createNodeInternal(name, mode);
		this->addChild(node);
		return node;
	}

	VirtualDirectory* createDirectoryInternal(const std::string& name);

	VirtualNode* createNodeInternal(const std::string& name, VirtualNode::Mode mode);


private:
	std::map<std::string, VirtualNode*> m_nodes;
	DirType m_dirType;
};