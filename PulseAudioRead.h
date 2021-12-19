#ifndef PULSEAUDIOREAD_H
#define PULSEAUDIOREAD_H

#include <cstring>
#include <vector>
#include <memory>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "ThreadHelpers.h"

class PulseAudioModule
{
	std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> out_buffer;
	pa_simple *pa_stream = NULL;
	unsigned buffer_length = 0;

public:
	PulseAudioModule(
		const std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> out_buffer,
		const unsigned buffer_length = 1050,
		const std::string app_name = "PulseAudio Read",
		const std::string stream_desc = "pa_read thread"
	);
	~PulseAudioModule();

	void execute_loop();
};

class RawToDoubleModule
{
	std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> in_buffer;
	std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> out_buffer;
public:
	RawToDoubleModule(
		const std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> in_buffer,
		const std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> out_buffer
	);

	void execute_loop();
};

#endif // PULSEAUDIOREAD_H 