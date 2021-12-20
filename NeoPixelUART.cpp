#include "NeoPixelUART.h"

#include <cstring>
#include <stdexcept>
#include <iterator>
#include <sys/ioctl.h>
#include <asm/termbits.h>
#include <unistd.h>
#include <fcntl.h>
// #include <math.h>


static const uint8_t led_bit_triplets_lookup_table[8] = {0x5b, 0x1b, 0x53, 0x13, 0x5a, 0x1a, 0x52, 0x12};

void Serial::setupInterface(unsigned baud_rate)
{
	struct termios2 tio;
	std::memset (&tio, 0, sizeof tio);

 	if(ioctl(out, TCGETS2, &tio) ) throw std::runtime_error("Failed to read port settings");

	// Here's some UNIX UART black magic
	tio.c_cflag = (tio.c_cflag & ~CSIZE) | CS7;
	tio.c_cflag &= ~CSTOPB;
	tio.c_cflag &= ~PARENB;

	tio.c_cflag &= ~CBAUD;
	tio.c_cflag |= CBAUDEX;	
	tio.c_ospeed = baud_rate;

	tio.c_oflag &= ~OPOST;
	tio.c_oflag &= ~ONLCR;
	
	if(ioctl(out, TCSETS2, &tio) ) throw std::runtime_error("Failed to write port settings");
}

Serial::Serial(const std::string portname, const unsigned baud_rate)
{
	out = open(portname.c_str(), O_WRONLY | O_NOCTTY | O_NDELAY);
	if (out < 0) throw std::runtime_error(std::string("Failed to open port: ") + portname);

	setupInterface(baud_rate);
}

Serial::~Serial()
{
	close(out);
	// if( close(out) ) throw std::runtime_error("Failed to close tty file");
}

void Serial::write(const uint8_t *data, const unsigned size)
{
	::write(out, data, size);
}

NeoPixelUART::NeoPixelUART(const std::string portname, const unsigned baud_rate)
: out(portname, baud_rate)
{

}

std::vector<uint8_t> NeoPixelUART::encode(const uint32_t color_hex)
{
	std::vector<uint8_t> out;
	for(int8_t b = 7; b >= 0; b--)
	{
		out.push_back(
			led_bit_triplets_lookup_table[ (color_hex >> (b * 3) ) & 0x7]
		);
	}
	return out;
}

void NeoPixelUART::setLEDsHEX(const std::vector<uint32_t> &led_colors_hex)
{
	std::vector<uint8_t> led_data;
	static std::vector<uint8_t> encoded_color;
	for(auto led_color: led_colors_hex)
	{
		encoded_color = encode(led_color);
		led_data.insert(std::end(led_data), std::begin(encoded_color), std::end(encoded_color) );
	}
	out.write(led_data.data(), led_data.size() );
}

void NeoPixelUART::setLEDs(const std::vector<ColorRGB> &led_colors)
{
	std::vector<uint32_t> hex_colors;
	for(auto led_color: led_colors)
	{
		hex_colors.push_back(led_color.toHEX() );
	}
	setLEDsHEX(hex_colors);
}

void NeoPixelUART::setLEDs(const DataArray<ColorRGB> &led_colors)
{
	std::vector<uint32_t> hex_colors;
	for(unsigned c = 0; c < led_colors.size(); c++)
	{
		hex_colors.push_back(led_colors.data[c].toHEX() );
	}
	setLEDsHEX(hex_colors);
}

uint32_t ColorRGB::toHEX() const
{
	return (uint32_t(r * 255.0) << 16) + (uint32_t(g * 255.0) << 8) + uint32_t(b * 255.0);
}

ColorRGB ColorRGB::operator+=(const ColorRGB &rhs)
{
   r += rhs.r;
   g += rhs.g;
   b += rhs.b;
   return *this;
}

// uint32_t NeoPixelUART::hsv2hex(const double h, const double s, const double v)
// {
// 	return
// }


NeoPixelModule::NeoPixelModule(
	const std::shared_ptr<NeoPixelUART> led_strip,
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<ColorRGB>>> in_buffer
) : led_strip(led_strip), in_buffer(in_buffer) {}

void NeoPixelModule::execute_loop()
{
	DataArray<ColorRGB> colors_in = in_buffer->get();
	while(true)
	{
		colors_in.update(in_buffer->get() );
		led_strip->setLEDs(colors_in);
	}
}