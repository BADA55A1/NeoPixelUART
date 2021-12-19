#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "PulseAudioRead.h"
#include "DFT.h"
#include "ThreadHelpers.h"

#include <unistd.h>


int main()
{
	// Setup buffers
	std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> raw_audio = std::make_shared<OneDirectionDataBuffer<DataArray<uint8_t>>>();

	std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> real_audio = std::make_shared<OneDirectionDataBuffer<DataArray<double>>>();

	std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> fft = std::make_shared<OneDirectionDataBuffer<DataArray<double>>>();

	// Setup modules
	PulseAudioModule module_pa(raw_audio, 1050);
	RawToDoubleModule module_to_double(raw_audio, real_audio);
	DFT module_dft(real_audio, fft);

	// Start modules
	std::thread module_pa_t(&PulseAudioModule::execute_loop, module_pa);
	std::thread module_to_double_t(&RawToDoubleModule::execute_loop, module_to_double);
	std::thread module_dft_t(&DFT::execute_loop, module_dft);

	module_pa_t.join();
	module_to_double_t.join();
	module_dft_t.join();

	return 0;
}