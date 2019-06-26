#pragma once

#include "../predefine.h"
#include "virtual_directory.h"

class VirtualNode;
class VirtualNodeInfoImpl;
class VirtualNodeIterator;

class VirtualNodeInfo
{
public:
	typedef VirtualNodeIterator Iterator;

	explicit VirtualNodeInfo(const std::string& path);

	explicit VirtualNodeInfo(VirtualNode* node = NULL);

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
	VirtualNodeIterator() : m_canIterator(false)
	{
	}

	VirtualNodeIterator(VirtualDirectory::Iterator iter) : m_canIterator(true), m_iterator(iter)
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

		return VirtualNodeInfo(m_iterator->second);
	}

private:
	VirtualDirectory::Iterator m_iterator;
	bool m_canIterator;
};