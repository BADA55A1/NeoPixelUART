#include "PulseAudioRead.h"
#include <cstring>

#include <iostream>

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
	std::cout << "copy constructor\n";
}

RawAudioData::~RawAudioData()
{
	delete[] data;
}