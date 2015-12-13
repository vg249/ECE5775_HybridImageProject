
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

void FFT(int dir, long m, complex <double> x[]);

void fftshift(complex<double> out[65536], complex<double> in[65536], int xdim, int ydim, int xshift, int yshift);

void GaussFilter(int imgwidth, int imgheight, complex<double> F[65536], bool High);

void normalize(complex<double> imgNormIn[65536], double imgNormOut[65536]);

void hybrid_image(int intImgSize, complex<double> imgLo_input[65536], complex<double> imgHi_input[65536],  double imgOutput[65536]);

