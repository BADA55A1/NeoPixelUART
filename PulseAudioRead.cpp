#include "PulseAudioRead.h"
#include <cstring>

RawAudioData::RawAudioData()
{
	this->data_len = 0;
	data = nullptr;
}

RawAudioData::RawAudioData(unsigned data_len)
{
	this->data_len = data_len;
	data = new uint8_t[data_len];
}

RawAudioData::RawAudioData(const RawAudioData &in) : RawAudioData(in.dataLength())
{
	memcpy(this->data, in.data, data_len*sizeof(uint8_t));
}

RawAudioData::~RawAudioData()
{
	delete[] data;
}


PulseAudioRead::PulseAudioRead(
	const std::shared_ptr<OneDirectionDataBuffer<RawAudioData>> out_buffer,
	const unsigned buffer_length,
	const std::string app_name,
	const std::string stream_desc
) {
	this->out_buffer = out_buffer;
	this->buffer_length = buffer_length;

	static const pa_sample_spec ss = {
		.format = PA_SAMPLE_S16LE,
		.rate = 44100,
		.channels = 1
	};
	
	pa_stream = pa_simple_new(
		NULL,                 // Use the default server.
		app_name.c_str(),     // Application name.
		PA_STREAM_RECORD,     // Stream direction
		NULL,                 // Use the default device.
		stream_desc.c_str(),  // Description of stream.
		&ss,                  // Sample format.
		NULL,                 // Use default channel map
		NULL,                 // Use default buffering attributes.
		NULL                  // Ignore error code.
	);


}

PulseAudioRead::~PulseAudioRead()
{
	if(pa_stream)	pa_simple_free(pa_stream);
}

void PulseAudioRead::execute_loop()
{
	RawAudioData tmp_buffer(buffer_length);
	while(true)
	{
		pa_simple_read(
			pa_stream,
			tmp_buffer.data,
			buffer_length, 
			NULL 
		);
		out_buffer->set(tmp_buffer);
	}
}

RawDataToDouble::RawDataToDouble(
	const std::shared_ptr<OneDirectionDataBuffer<RawAudioData>> in_buffer,
	const std::shared_ptr<OneDirectionDataBuffer<std::vector<double>>> out_buffer
) : in_buffer(in_buffer), out_buffer(out_buffer) {}

void RawDataToDouble::execute_loop()
{
	RawAudioData tmp_in_buffer = in_buffer->get();
	const unsigned out_vector_size = tmp_in_buffer.dataLength() / 2;
	std::vector<double> tmp_out_buffer(out_vector_size);

	while(true)
	{
		tmp_in_buffer = in_buffer->get();
		for(unsigned sample = 0; sample < out_vector_size; sample++)
		{
			tmp_out_buffer[sample] = (
				double(tmp_in_buffer.data[sample * 2]) + 
				double(int8_t(tmp_in_buffer.data[sample * 2 + 1]) << 8)
			) / 65536.0;
		}
		out_buffer->set(tmp_out_buffer);
	}
}