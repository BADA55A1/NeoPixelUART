#include "ThreadHelpers.h"

// Templates in C++ ....
/*
template <class T>
void OneDirectionData<T>::set(const T &data)
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

template <class T>
T OneDirectionData<T>::get()
{
	T out;
	// Wait for "new data available" flag to be set
	std::unique_lock lock(new_data_mutex);
	new_data_cond_var.wait(lock, [this]() { return new_data_flag; });
	// Lock "out_read" mutex
	out_read.lock();
	// Read new data
	out = data;
	// Unset "new data available" flag
	new_data_flag = false;
	// Unlock "out_read" mutex
	out_read.unlock();

	return out;
}
*/