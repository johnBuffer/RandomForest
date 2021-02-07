#pragma once

#include <vector>


template<typename T>
class Index
{
private:
	std::vector<T>* m_container;
	uint64_t m_index;

public:
	Index()
		: m_container(nullptr)
		, m_index(0)
	{}

	Index(std::vector<T>& v, uint64_t i)
		: m_container(&v)
		, m_index(i)
	{}

	T& operator*()
	{
		return (*m_container)[m_index];
	}

	const T& operator*() const
	{
		return (*m_container)[m_index];
	}

	T* operator->()
	{
		return &(*m_container)[m_index];
	}

	const T* operator->() const
	{
		return &(*m_container)[m_index];
	}
};
