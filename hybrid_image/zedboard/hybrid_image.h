#include "ap_fixed.h"
//#include "hls_fft.h"
#include <complex>
#include "ap_int.h"
#include<hls_stream.h>

typedef ap_uint<32> bit32_t;
typedef ap_uint<64> bit64_t;

// configurable params

using namespace std;

//struct config1 : hls::ip_fft::params_t {
//    static const unsigned ordering_opt = hls::ip_fft::natural_order;
//
//    static const unsigned config_width = FFT_CONFIG_WIDTH;
//};

//typedef hls::ip_fft::config_t<config1> config_t;
//typedef hls::ip_fft::status_t<config1> status_t;

void dut( hls::stream<bit32_t> &strm_in, hls::stream<bit32_t> &strm_out);

void FFT(int dir, int m, complex <float> x[4096]);

void fftshift(complex<float> out[4096], complex<float> in[4096], int xdim, int ydim, int xshift, int yshift);

void GaussFilter(int imgwidth, int imgheight, complex<float> F[4096], bool High);

void normalize(complex<float> imgNormIn[4096], float imgNormOut[4096]);

void hybrid_image(int intImgSize, complex<float> imgLo_input[4096], complex<float> imgHi_input[4096],  float imgOutput[4096]);
