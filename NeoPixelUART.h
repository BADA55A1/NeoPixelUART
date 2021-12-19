#ifndef NEOPIXELUART_H
#define NEOPIXELUART_H

#include <vector>
#include <cstdint>
#include "ThreadHelpers.h"

class Serial
{
	int out;
	void setupInterface(unsigned baud_rate);
public:
	Serial(const std::string portname, const unsigned baud_rate);
	~Serial();
	void write(const uint8_t *data, const unsigned size);
};


class AdressingLEDstrip
{
	Serial out;

	std::vector<uint8_t> encode(const uint32_t color_hex);
public:
	AdressingLEDstrip(
		const std::string portname = "/dev/ttyUSB0",
		const unsigned baud_rate = 2400000
	);
	//~AdressingLEDstrip();

	void setLEDs(const std::vector<uint32_t> &led_colors_hex);
};



#endif // NEOPIXELUART_H 