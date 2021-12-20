#include "Visuals.h"

VisualsModule::VisualsModule(
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> in_buffer,
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<ColorRGB>>> out_buffer
) : in_buffer(in_buffer), out_buffer(out_buffer) {}

#include <iostream>

// LOW  [0:5]
// MID  [6:25]
// HIGH [26:95]

#define VISUAL_LOW_LEN 5
#define VISUAL_MID_LEN 20
#define VISUAL_HIGH_LEN 80

void VisualsModule::calcLow(const unsigned mode, const DataArray<double> &in, DataArray<ColorRGB> &out)
{
	static double last[VISUAL_LOW_LEN] = {0};
	static double current[VISUAL_LOW_LEN] = {0};
	static double max = 0;
	static double out_tmp[VISUAL_LOW_LEN * 2 + 6];

	for(unsigned s = 0; s < VISUAL_LOW_LEN; s++)
	{
		if(in.data[s] > max) max = in.data[s];
		current[s] = in.data[s] / max;
		current[s] *= current[s];
		if(current[s] < last[s])
		{
			current[s] = current[s] * 0.35 + last[s] * 0.65 ;
		}
	}

	out_tmp[0] = (current[0] ) * 0.3 + (current[1] ) * 0.4 + (current[2] ) * 0.3;
	out_tmp[1] = (current[0] ) * 0.5 + (current[1] ) * 0.3 + (current[2] ) * 0.2;
	out_tmp[2] = current[0] ;

	for(unsigned c = 0; c < VISUAL_LOW_LEN * 2; c++)
	{
		if(c % 2)
		{
			out_tmp[c + 3] = current[(c + 1) / 2] ;
		}
		else
		{
			out_tmp[c + 3] = (current[c / 2] ) * 0.5 + (current[c / 2 + 1] ) * 0.5;
		}
	}

	out_tmp[VISUAL_LOW_LEN * 2 + 1] = (current[VISUAL_LOW_LEN - 1] ) * 0.85;
	out_tmp[VISUAL_LOW_LEN * 2 + 2] = (current[VISUAL_LOW_LEN - 1] ) * 0.65;
	out_tmp[VISUAL_LOW_LEN * 2 + 3] = (current[VISUAL_LOW_LEN - 1] ) * 0.35;
	out_tmp[VISUAL_LOW_LEN * 2 + 4] = (current[VISUAL_LOW_LEN - 1] ) * 0.2;
	out_tmp[VISUAL_LOW_LEN * 2 + 5] = (current[VISUAL_LOW_LEN - 1] ) * 0.10;

	for(unsigned c = 0; c < 2; c++)
	{
		out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c], out_tmp[c], out_tmp[c]);
	}

	for(unsigned c = 2; c < 4; c++)
	{
		if(mode == 0)
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c], out_tmp[c] * 0.5, out_tmp[c] * 0.5);
		}
		else if(mode == 1)
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c] * 0.5, out_tmp[c], out_tmp[c] * 0.5);
		}
		else
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c] * 0.5, out_tmp[c] * 0.5, out_tmp[c]);
		}
	}
	for(unsigned c = 4; c < 6; c++)
	{
		if(mode == 0)
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c], out_tmp[c] * 0.1, out_tmp[c] * 0.1);
		}
		else if(mode == 1)
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c] * 0.1, out_tmp[c], out_tmp[c] * 0.1);
		}
		else
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c] * 0.1, out_tmp[c] * 0.1, out_tmp[c]);
		}
	}

	for(unsigned c = 6; c < VISUAL_LOW_LEN * 2 + 6; c++)
	{
		if(mode == 0)
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(out_tmp[c], 0, 0);
		}
		else if(mode == 1)
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(0, out_tmp[c], 0);
		}
		else
		{
			out.data[c] = out.data[LED_STRIP_LENGTH - 1 - c] = ColorRGB(0, 0, out_tmp[c]);
		}
	}
	
	max *= 0.99;
	for(unsigned s = 0; s < VISUAL_LOW_LEN; s++)
	{
		last[s] = current[s];
	}
}

void VisualsModule::calcMid(const unsigned mode, const DataArray<double> &in, DataArray<ColorRGB> &out)
{
	static double last[VISUAL_MID_LEN] = {0};
	static double current[VISUAL_MID_LEN] = {0};
	static double max = 0;

	for(unsigned s = 0; s < VISUAL_MID_LEN; s++)
	{
		if(in.data[s+5] > max) max = in.data[s+5];
		current[s] = in.data[s+5] / max;
		current[s] *= current[s];
		if(current[s] < last[s])
		{
			current[s] = current[s] * 0.35 + last[s] * 0.65;
		}
	}

	if(mode == 0)
	{
		out.data[10] += ColorRGB(current[0] / 4, 0, 0);
		out.data[10 + VISUAL_MID_LEN - 1] += ColorRGB(current[10 + VISUAL_MID_LEN - 1] / 4, 0, 0);
	}
	else if(mode == 1)
	{
		out.data[10] += ColorRGB(0, current[0] / 4, 0);
		out.data[10 + VISUAL_MID_LEN - 1] += ColorRGB(0, current[10 + VISUAL_MID_LEN - 1] / 4, 0);
	}
	else
	{
		out.data[10] += ColorRGB(0, 0, current[0] / 4);
		out.data[10 + VISUAL_MID_LEN - 1] += ColorRGB(0, 0, current[10 + VISUAL_MID_LEN - 1] / 4);
	}

	for(unsigned c = 1; c < (VISUAL_MID_LEN - 1); c++)
	{
		if(mode == 0)
		{
			out.data[c+10] += ColorRGB(current[c], 0, 0);
		}
		else if(mode == 1)
		{
			out.data[c+10] += ColorRGB(0, current[c], 0);
		}
		else
		{
			out.data[c+10] += ColorRGB(0, 0, current[c]);
		}
	}
	
	max *= 0.99;
	for(unsigned s = 0; s < VISUAL_MID_LEN; s++)
	{
		last[s] = current[s];
	}
}

void VisualsModule::calcHigh(const unsigned mode, const DataArray<double> &in, DataArray<ColorRGB> &out)
{
	// static double last[VISUAL_HIGH_LEN / 4] = {0};
	// static double current[VISUAL_HIGH_LEN / 4] = {0};
	// static double max = 0;

	// for(unsigned s = 0; s < VISUAL_HIGH_LEN / 4; s++)
	// {
	// 	double sum =
	// 	if(in.data[s+25] > max) max = in.data[s+25];
	// 	current[s] = in.data[s+25] / max;
	// 	current[s] *= current[s];
	// 	if(current[s] < last[s])
	// 	{
	// 		current[s] = current[s] * 0.4 + last[s] * 0.6 ;
	// 	}
	// }
	// for(unsigned c = 1; c < (VISUAL_HIGH_LEN / 2); c++)
	// {
	// 	if(mode == 0)
	// 	{
	// 		out.data[c+3] += ColorRGB(current[c*2] + current[c*2 +1], 0, 0);
	// 	}
	// 	else if(mode == 1)
	// 	{
	// 		out.data[c+3] += ColorRGB(0, current[c*2] + current[c*2 +1], 0);
	// 	}
	// 	else
	// 	{
	// 		out.data[c+3] += ColorRGB(0, 0, current[c*2] + current[c*2 +1]);
	// 	}
	// }

	// max *= 0.99;
	// for(unsigned s = 0; s < VISUAL_HIGH_LEN; s++)
	// {
	// 	last[s] = current[s];
	// }
}

void VisualsModule::execute_loop()
{
	DataArray<double> fft_in = in_buffer->get();
	DataArray<ColorRGB> colors_out(LED_STRIP_LENGTH);

	uint32_t clock = 0;
	unsigned mode = 0;
	bool mode_ch_pause = false;
	unsigned mode_l[] = {0,0,1,1,2,2};
	unsigned mode_m[] = {1,2,0,2,0,1};
	unsigned mode_h[] = {2,1,2,0,1,0};

	double current = 0, last = 0;

	for(unsigned l = 0; l < LED_STRIP_LENGTH; l++) colors_out.data[l] = ColorRGB();
	while(true)
	{
		fft_in.update(in_buffer->get() );

		for(unsigned l = 0; l < LED_STRIP_LENGTH; l++) colors_out.data[l] = ColorRGB();
		calcLow(mode_l[mode], fft_in, colors_out);
		calcMid(mode_m[mode], fft_in, colors_out);
		calcHigh(mode_h[mode], fft_in, colors_out);

		out_buffer->set(colors_out);

		clock++;
		if(clock > 100000) 
		{
			if(fft_in.data[0] > last)
			{
				current = fft_in.data[0];
				if(clock > 100100)
				{
					mode++;
					if(mode == 6) mode = 0;
					clock = 0;
				}
			}
			else
			{
				current = fft_in.data[0] * 0.3 + last * 0.7;
			}
		}
		else if(fft_in.data[0] == 0)
		{
			if(mode_ch_pause)
			{
				mode++;
				if(mode == 6) mode = 0;
				mode_ch_pause = false;
			}
		}
		else
		{
			mode_ch_pause = true;
		}
	}
}
