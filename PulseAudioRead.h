#ifndef PULSEAUDIOREAD_H
#define PULSEAUDIOREAD_H

#include <cstring>
#include <vector>
#include <memory>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "ThreadHelpers.h"

class RawAudioData
{
	unsigned data_len;
public:
	uint8_t *data;

	RawAudioData();
	RawAudioData(unsigned data_len);
	RawAudioData(const RawAudioData &in);
	~RawAudioData();

	unsigned dataLength() const { return data_len; }
};

class PulseAudioRead
{
	std::shared_ptr<OneDirectionDataBuffer<RawAudioData>> out_buffer;
	pa_simple *pa_stream = NULL;
	unsigned buffer_length = 0;

public:
	PulseAudioRead(
		const std::shared_ptr<OneDirectionDataBuffer<RawAudioData>> out_buffer,
		const unsigned buffer_length = 1050,
		const std::string app_name = "PulseAudio Read",
		const std::string stream_desc = "pa_read thread"
	);
	~PulseAudioRead();

	void execute_loop();
};

class RawDataToDouble
{
	std::shared_ptr<OneDirectionDataBuffer<RawAudioData>> in_buffer;
	std::shared_ptr<OneDirectionDataBuffer<std::vector<double>>> out_buffer;
public:
	RawDataToDouble(
		const std::shared_ptr<OneDirectionDataBuffer<RawAudioData>> in_buffer,
		const std::shared_ptr<OneDirectionDataBuffer<std::vector<double>>> out_buffer
	);

	void execute_loop();
};

#endif // PULSEAUDIOREAD_H 