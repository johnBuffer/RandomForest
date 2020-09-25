#pragma once
#include <vector>
#include <iostream>


template<typename T>
struct Ptr;

template<typename T>
struct IndexVector
{
	IndexVector() :
		data_size(0)
	{}
	// Data ADD / REMOVE
	uint64_t push_back(const T& obj);
	void erase(uint64_t id);
	// Data access by ID
	T& operator[](uint64_t i);
	const T& operator[](uint64_t i) const;
	// Returns a standalone object allowing access to the underlying data
	Ptr<T> getPtr(uint64_t id);
	// Returns the data at a specific place in the data vector (not an ID)
	T& getDataAt(uint64_t i);
	// Check if the data behind the pointer is the same
	bool isValid(uint64_t i, uint64_t validity) const;
	// Returns the ID of the ith element of the data array
	uint64_t getID(uint64_t i) const;
	// Iterators
	typename std::vector<T>::iterator begin();
	typename std::vector<T>::iterator end();
	// Number of objects in the array
	uint64_t size() const;

private:
	std::vector<T> data;
	std::vector<uint64_t> op_ids;
	std::vector<uint64_t> ids;
	std::vector<uint64_t> rids;
	uint64_t data_size;
	uint64_t op_count;

	uint64_t createNewSlot(const T& obj);
	uint64_t reuseSlot(const T& obj);
	const T& getAt(uint64_t i) const;
};

template<typename T>
inline uint64_t IndexVector<T>::push_back(const T& obj)
{
	if (data_size == data.size()) {
		return createNewSlot(obj);
	}

	return reuseSlot(obj);
}

template<typename T>
inline void IndexVector<T>::erase(uint64_t id)
{
	--data_size;
	uint64_t current_data_id = ids[id];
	std::swap(data[data_size], data[current_data_id]);
	rids[data_size] = id;

	const uint64_t last_obj_id = rids[data_size];
	rids[current_data_id] = last_obj_id;
	ids[last_obj_id] = current_data_id;
}

template<typename T>
inline T& IndexVector<T>::operator[](uint64_t i)
{
	return const_cast<T&>(getAt(i));
}

template<typename T>
inline const T& IndexVector<T>::operator[](uint64_t i) const
{
	return getAt(i);
}

template<typename T>
inline Ptr<T> IndexVector<T>::getPtr(uint64_t id)
{
	return Ptr<T>(id, *this, op_ids[id]);
}

template<typename T>
inline T& IndexVector<T>::getDataAt(uint64_t i)
{
	return data[i];
}

template<typename T>
inline uint64_t IndexVector<T>::getID(uint64_t i) const
{
	return rids[i];
}

template<typename T>
inline uint64_t IndexVector<T>::size() const
{
	return data_size;
}

template<typename T>
inline typename std::vector<T>::iterator IndexVector<T>::begin()
{
	return data.begin();
}

template<typename T>
inline typename std::vector<T>::iterator IndexVector<T>::end()
{
	return data.begin() + data_size;
}

template<typename T>
inline uint64_t IndexVector<T>::createNewSlot(const T& obj)
{
	data.push_back(obj);
	ids.push_back(data_size);
	rids.push_back(data_size);
	op_ids.push_back(op_count++);
	return data_size++;
}

template<typename T>
inline uint64_t IndexVector<T>::reuseSlot(const T& obj)
{
	const uint64_t reuse_id = rids[data_size];
	data[reuse_id] = obj;
	op_ids[reuse_id] = op_count++;
	++data_size;
	return reuse_id;
}

template<typename T>
inline const T& IndexVector<T>::getAt(uint64_t i) const
{
	return data[ids[i]];
}

template<typename T>
inline bool IndexVector<T>::isValid(uint64_t i, uint64_t validity) const
{
	return validity == op_ids[i];
}


template<typename T>
struct Ptr
{
	Ptr(uint64_t id_, IndexVector<T>& a, uint64_t vid)
		: id(id_)
		, array(a)
		, validity_id(vid)
	{}

	T* operator->()
	{
		return &array[id];
	}

	T& operator*()
	{
		return array[id];
	}

	operator bool() const
	{
		return array.isValid(id, validity_id);
	}

private:
	uint64_t id;
	IndexVector<T>& array;
	uint64_t validity_id;
};
