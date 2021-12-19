#ifndef PULSEAUDIOREAD_H
#define PULSEAUDIOREAD_H


#include <pulse/simple.h>
#include <pulse/error.h>

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

// class PulseAudioRead
// {
// 	std::shared_ptr<RawAudioData>
// }


#endif // PULSEAUDIOREAD_H 