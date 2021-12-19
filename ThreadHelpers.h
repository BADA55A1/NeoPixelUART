#ifndef THREADHELPERS_H
#define THREADHELPERS_H

#include <iostream>
#include <mutex>
#include <condition_variable>
// #include <unistd.h>


template <class T>
class OneDirectionData
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

};

#endif // THREADHELPERS_H 