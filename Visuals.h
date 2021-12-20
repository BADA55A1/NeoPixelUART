#ifndef VISUALS_H
#define VISUALS_H

#include <vector>
#include "ThreadHelpers.h"
#include "NeoPixelUART.h"

#define LED_STRIP_LENGTH 40

class VisualsModule
{
	std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> in_buffer;
	std::shared_ptr<OneDirectionDataBuffer<DataArray<ColorRGB>>> out_buffer;
public:
	VisualsModule(
		const std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> in_buffer,
		const std::shared_ptr<OneDirectionDataBuffer<DataArray<ColorRGB>>> out_buffer
	);

	void execute_loop();
};

#endif // VISUALS_H 