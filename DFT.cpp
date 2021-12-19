#include "DFT.h"
#include <fftw3.h>
#include <math.h>

#include <iostream>

DFT::DFT(
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> in_buffer,
	const std::shared_ptr<OneDirectionDataBuffer<DataArray<double>>> out_buffer
) : in_buffer(in_buffer), out_buffer(out_buffer) {}

void DFT::execute_loop()
{
	DataArray<double> signal_in = in_buffer->get();
	const unsigned fft_size = signal_in.size() / 2;

	DataArray<double> fft_out(fft_size);

	fftw_complex *fft_out_c;
	fft_out_c = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fft_size);
   fftw_plan plan = fftw_plan_dft_r2c_1d(signal_in.size(), signal_in.data, fft_out_c, FFTW_ESTIMATE | FFTW_PRESERVE_INPUT);

	
	while(true)
	{
		signal_in.update(in_buffer->get() );

		fftw_execute(plan);
		for(unsigned n = 0; n < fft_size; n++)
		{
			fft_out.data[n] = sqrt(fft_out_c[n][0] * fft_out_c[n][0] + fft_out_c[n][1] * fft_out_c[n][1]);
		}

		std::cout << fft_out.data[fft_size - 1] << "\n";
		out_buffer->set(fft_out);
	}
}
