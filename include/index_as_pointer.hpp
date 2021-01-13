#pragma once
#include <vector>


template<typename T>
struct Index
{
	uint64_t index;
	std::vector<T>* container;

	Index()
		: index(0)
		, container(nullptr)
	{

	}

	Index(uint64_t i, std::vector<T>& cont)
		: index(i)
		, container(&cont)
	{}

	explicit operator bool() const
	{
		return container != nullptr;
	}

	const T& getObject() const
	{
		return (*container)[index];
	}
	
	T& operator*()
	{
		return const_cast<T&>(getObject());
	}

	T* operator->()
	{
		return &const_cast<T&>(getObject());
	}

	const T& operator*() const
	{
		return getObject();
	}

	const T* operator->() const
	{
		return &getObject();
	}
};

