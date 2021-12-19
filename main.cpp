#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "PulseAudioRead.h"
#include "ThreadHelpers.h"

#include <unistd.h>


int main()
{
	std::shared_ptr<OneDirectionDataBuffer<RawAudioData>> raw_audio = std::make_shared<OneDirectionDataBuffer<RawAudioData>>();

	std::shared_ptr<OneDirectionDataBuffer<std::vector<double>>> real_audio = std::make_shared<OneDirectionDataBuffer<std::vector<double>>>();

	PulseAudioRead trans(raw_audio, 1050);
	RawDataToDouble raw_to_double(raw_audio, real_audio);

	std::thread tr_t(&PulseAudioRead::execute_loop, trans);
	std::thread re_t(&RawDataToDouble::execute_loop, raw_to_double);
	tr_t.join();
	re_t.join();

	return 0;
}