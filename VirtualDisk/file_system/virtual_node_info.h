#pragma once

#include "../predefine.h"
#include "virtual_directory.h"
#include "path.h"

class VirtualNode;
class VirtualNodeInfoImpl;
class VirtualNodeIterator;

class VirtualNodeInfo
{
public:
	typedef VirtualNodeIterator Iterator;

	VirtualNodeInfo();

	explicit VirtualNodeInfo(const std::string& absPath);

	VirtualNodeInfo(VirtualNode* node, const std::string& absPath);

	VirtualNodeInfo(const VirtualNodeInfo& other);

	VirtualNodeInfo& operator=(const VirtualNodeInfo& other);

	~VirtualNodeInfo();

	Iterator begin();

	Iterator end();

	bool isExist() { return m_isExist; }

	std::string getName();

	std::string getMode();

	std::string getCreateDay();

	std::string getCreateTime();

	std::string getFullPath();

	bool isFile();

	bool isDirectory();

	bool isSoftlink();

	bool isEmpty();

	int getSize();

	int getChildCount();

	bool isSpecialDirectory();

	std::string getTargetLinkPath();

	VirtualNodeInfo getFinalTargetLinkInfo(const std::string& curPath);

private:
	VirtualNodeInfoImpl* m_impl;
	bool m_isExist;
};

class VirtualNodeIterator
{
public:
	VirtualNodeIterator() : m_path(""), m_canIterator(false)
	{

	}

	explicit VirtualNodeIterator(const std::string& path) : m_path(path), m_canIterator(false)
	{
	}

	VirtualNodeIterator(VirtualDirectory::Iterator iter, const std::string& path) : m_path(path), m_canIterator(true), m_iterator(iter)
	{
	}

	bool operator==(const VirtualNodeIterator& other)
	{
		if(m_canIterator)
			return m_iterator == other.m_iterator;
		return true;
	}

	bool operator !=(const VirtualNodeIterator& other)
	{
		return !this->operator ==(other);
	}

	void operator++(int)
	{
		if(m_canIterator)
			m_iterator++;
	}

	VirtualNodeInfo operator*()
	{
		if(!m_canIterator)
			return VirtualNodeInfo();

		const std::string& name = m_iterator->second->getName();
		std::string curFullPath = Path::join(m_path, name, Path::InnerSeparator());

		return VirtualNodeInfo(m_iterator->second, curFullPath);
	}

private:
	VirtualDirectory::Iterator m_iterator;
	bool m_canIterator;
	std::string m_path;
};