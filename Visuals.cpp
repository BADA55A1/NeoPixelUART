#include "Visuals.h"

VisualsModule::VisualsModule(
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> in_buffer,
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<ColorRGB>>> out_buffer
) : in_buffer(in_buffer), out_buffer(out_buffer) {}

void VisualsModule::execute_loop()
{
	DataArray<double> fft_in = in_buffer->get();
	DataArray<ColorRGB> colors_out(LED_STRIP_LENGTH);

	double last[LED_STRIP_LENGTH] = {0};
	double current[LED_STRIP_LENGTH] = {0};
	double max[LED_STRIP_LENGTH] = {0};

	while(true)
	{
		fft_in.update(in_buffer->get() );



		for(unsigned c = 0; c < LED_STRIP_LENGTH; c++)
		{
			if(fft_in.data[c] > last[c])
			{
				current[c] = fft_in.data[c];
			}
			else
			{
				current[c] = fft_in.data[c] * 0.15 + last[c] * 0.85;
			}
			if(current[c] > max[c]) max[c] = current[c];

			colors_out.data[c] = ColorRGB(
				current[c] / max[c],
				current[c] / max[c],
				current[c] / max[c]
			);
		}

		out_buffer->set(colors_out);

		for(unsigned c = 0; c < LED_STRIP_LENGTH; c++)
		{
			max[c] *= 0.99;
			last[c] = current[c];
		}
	}
}
