
#include "ap_fixed.h"
#include "hls_fft.h"
#include <complex>
#include "ap_int.h"
#include<hls_stream.h>

typedef ap_uint<32> bit32_t;

// configurable params
const char FFT_INPUT_WIDTH                     = 16;
const char FFT_OUTPUT_WIDTH                    = FFT_INPUT_WIDTH;
const char FFT_CONFIG_WIDTH                    = 16;
const char FFT_NFFT_MAX                        = 3; 
const int  FFT_LENGTH                          = 8; 

typedef ap_fixed<FFT_INPUT_WIDTH,1> data_in_t;
typedef ap_fixed<FFT_OUTPUT_WIDTH,FFT_OUTPUT_WIDTH-FFT_INPUT_WIDTH+1> data_out_t;
typedef std::complex<data_in_t> cmpxDataIn;
typedef std::complex<data_out_t> cmpxDataOut;

using namespace std;

struct config1 : hls::ip_fft::params_t {
    static const unsigned ordering_opt = hls::ip_fft::natural_order;

    static const unsigned config_width = FFT_CONFIG_WIDTH;
};

typedef hls::ip_fft::config_t<config1> config_t;
typedef hls::ip_fft::status_t<config1> status_t;

void dut(
    hls::stream<bit32_t> &strm_in1,
    hls::stream<bit32_t> &strm_in2,
    hls::stream<bit32_t> &strm_out
);

void FFT(int dir, long m, complex <double> x[]);

void fftshift(complex<double> out[65536], complex<double> in[65536], int xdim, int ydim, int xshift, int yshift);

void GaussFilter(int imgwidth, int imgheight, complex<double> F[65536], bool High);

void normalize(complex<double> imgNormIn[65536], double imgNormOut[65536]);

void hybrid_image(int intImgSize, complex<double> imgLo_input[65536], complex<double> imgHi_input[65536],  double imgOutput[65536]);

