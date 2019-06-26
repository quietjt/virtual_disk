#pragma once
#include <cstddef>
#include <iostream>

template<typename T>
class Singleton
{
public:
	static void init()
	{
		m_inst = new T();
		m_inst->onInit();
	}

	static void release()
	{
		if(m_inst)
		{
			delete m_inst;
			m_inst = NULL;
		}
	}

	static T* getInstPtr()
	{
		if(m_inst == NULL)
		{
			std::cerr << "You should call init() first" << std::endl;
		}

		return m_inst;
	}

protected:
	virtual void onInit() {};

	Singleton() {}
	Singleton(const Singleton& other) {}
	Singleton& operator=(const Singleton& other) {}

protected:
	static T* m_inst;
};

template<typename T>
typename T* Singleton<T>::m_inst = NULL;  