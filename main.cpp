#include <iostream>
#include <vector>
#include <thread>

#include "PulseAudioRead.h"
#include "ThreadHelpers.h"

#include <unistd.h>

class Transmitter
{
public:
	OneDirectionDataBuffer<RawAudioData> * data_out;

	void send_in_loop()
	{
		for(;;)
		{
			RawAudioData tmp_data(1000000);
			std::cout << "Transmitter sending data...\n";
			data_out->set(tmp_data);
			usleep(50000);
		}
	}
};

class Receiver
{
public:
	OneDirectionDataBuffer<RawAudioData> * data_in;

	void get_in_loop()
	{
		for(;;)
		{
			auto tmp_data = data_in->get();
			std::cout << "Receiver got data.\n";
			usleep(100000);
		}
	}
};



int main()
{
	// std::shared_ptr<OneDirectionDataBuffer<std::vector<unsigned>>> p = std::make_shared<Derived>();

	OneDirectionDataBuffer<RawAudioData> data;
	Transmitter trans;
	Receiver reciv;
	trans.data_out = &data;
	reciv.data_in = &data;

	std::thread tr_t(&Transmitter::send_in_loop, trans);
	std::thread re_t(&Receiver::get_in_loop, reciv);
	tr_t.join();
	re_t.join();

	return 0;
}