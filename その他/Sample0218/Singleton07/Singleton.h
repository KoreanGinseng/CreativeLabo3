#pragma once

#include	<iostream>
#include	<cassert>

/**
 * Singleton用テンプレート
 */
template< typename T >
class Singleton
{
private:
	Singleton(const Singleton& obj) = delete;
	Singleton(Singleton&& obj) = delete;

	static T* instance;
protected:
	Singleton()
	{
		assert(!instance);
		int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
		instance = (T*)((int)this + offset);
	}

	virtual ~Singleton()
	{
		assert(instance);
		instance = nullptr;
	}

public:
	static T& GetInstance(void)
	{
		assert(instance);
		return *instance;
	}
};
template< typename T > T* Singleton<T>::instance = nullptr;