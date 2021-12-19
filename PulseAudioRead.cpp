#include "PulseAudioRead.h"
#include <cstring>


PulseAudioModule::PulseAudioModule(
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> out_buffer,
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

PulseAudioModule::~PulseAudioModule()
{
	if(pa_stream)	pa_simple_free(pa_stream);
}

void PulseAudioModule::execute_loop()
{
	DataArray<uint8_t> tmp_buffer(buffer_length);
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

RawToDoubleModule::RawToDoubleModule(
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<uint8_t>>> in_buffer,
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> out_buffer
) : in_buffer(in_buffer), out_buffer(out_buffer) {}

void RawToDoubleModule::execute_loop()
{
	DataArray<uint8_t> tmp_in_buffer = in_buffer->get();
	const unsigned out_vector_size = tmp_in_buffer.size() / 2;
	DataArray<double> tmp_out_buffer(out_vector_size);

	while(true)
	{
		tmp_in_buffer.update(in_buffer->get() );
		for(unsigned sample = 0; sample < out_vector_size; sample++)
		{
			tmp_out_buffer.data[sample] = (
				double(tmp_in_buffer.data[sample * 2]) + 
				double(int8_t(tmp_in_buffer.data[sample * 2 + 1]) << 8)
			) / 65536.0;
		}
		out_buffer->set(tmp_out_buffer);
	}
}