#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "NeoPixelUART.h"
#include "PulseAudioRead.h"
#include "DFT.h"
#include "ThreadHelpers.h"
#include "Visuals.h"

#include <unistd.h>


int main()
{
	std::shared_ptr<NeoPixelUART> led_strip = std::make_shared<NeoPixelUART>("/dev/ttyUSB0", 2400000);

	std::vector<ColorRGB> black_col(40);
	led_strip->setLEDs(black_col);


	// Setup buffers
	std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> raw_audio = std::make_shared<OneDirectionDataBuffer<DataArray<uint8_t>>>();

	std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> real_audio = std::make_shared<OneDirectionDataBuffer<DataArray<double>>>();

	std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> fft = std::make_shared<OneDirectionDataBuffer<DataArray<double>>>();

	std::shared_ptr<OneDirectionDataBuffer<DataArray<ColorRGB>>> colors = std::make_shared<OneDirectionDataBuffer<DataArray<ColorRGB>>>();

	// Setup modules
	PulseAudioModule module_pa(raw_audio, 2048);
	RawToDoubleModule module_to_double(raw_audio, real_audio);
	DFTModule module_dft(real_audio, fft);
	VisualsModule module_visuals(fft, colors);
	NeoPixelModule module_leds(led_strip, colors);

	// Start modules
	std::thread module_pa_t(&PulseAudioModule::execute_loop, module_pa);
	std::thread module_to_double_t(&RawToDoubleModule::execute_loop, module_to_double);
	std::thread module_dft_t(&DFTModule::execute_loop, module_dft);
	std::thread module_visuals_t(&VisualsModule::execute_loop, module_visuals);
	std::thread module_leds_t(&NeoPixelModule::execute_loop, module_leds);

	module_pa_t.join();
	module_to_double_t.join();
	module_dft_t.join();
	module_visuals_t.join();
	module_leds_t.join();

	return 0;
}