#ifndef THREADHELPERS_H
#define THREADHELPERS_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>
// #include <unistd.h>


template <class T>
class DataArray
{
	unsigned data_len;
public:
	T *data;

	DataArray()
	{
		this->data_len = 0;
		data = nullptr;
	}
	DataArray(unsigned data_len)
	{
		this->data_len = data_len;
		data = new T[data_len];
	}
	DataArray(const DataArray<T> &in) : DataArray(in.size())
	{
		memcpy(this->data, in.data, data_len*sizeof(T));
	}
	// DataArray(const DataArray<T> &in)
	// {
	// 	memcpy(this->data, in.data, data_len*sizeof(T));
	// }
	~DataArray()
	{
		delete[] data;
	}

	void update(const DataArray<T> &in)
	{
		memcpy(this->data, in.data, data_len*sizeof(T));
	}

	unsigned size() const { return data_len; }
};

template <class T>
class OneDirectionDataBuffer
{
private:
	T data;

	std::mutex out_read;
	bool new_data_flag = false;
	std::mutex new_data_mutex;
	std::condition_variable new_data_cond_var;

public:
	void set(const T &data)
	{
		// Try to lock "out_read" mutex, SKIP if "out_read" mutex locked
		std::unique_lock<std::mutex> out_read_lock(out_read, std::try_to_lock);
		if (out_read_lock)
		{
			// Set new data
			this->data = data;

			// Probably not the best idea, but prevents complete thread freeze on extreme delays
			// std::lock_guard g(new_data_mutex);
			
			// Set "new data available" flag 
			new_data_flag = true;
			new_data_cond_var.notify_all();

			// Unlock "out_read" mutex
			out_read.unlock();
		}
	}

	T get()
	{
		// Wait for "new data available" flag to be set
		std::unique_lock lock(new_data_mutex);
		new_data_cond_var.wait(lock, [this]() { return new_data_flag; });

		// Lock "out_read" mutex
		out_read.lock();

		// Read new data
		T out = data;

		// Unset "new data available" flag
		new_data_flag = false;

		// Unlock "out_read" mutex
		out_read.unlock();
		
		return out;
	}

};

#endif // THREADHELPERS_H 