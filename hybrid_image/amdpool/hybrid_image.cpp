//==========================================================================
//CimgExample.cpp
//==========================================================================
// @brief: A Hybrid imaging of 2 images with different fitering

//#include "CImg.h"
#include "fft.h"
//#include <cmath>
//#include <iostream>
//#include <string.h>
//#include <complex>
//#include "ap_int.h"
//
//#define PI 3.14159

using namespace std;
//using namespace cimg_library;
typedef ap_uint<32> bit32_t;

//----------------------------------------------------------
// Top function
//----------------------------------------------------------

void dut(
    hls::stream<bit32_t> &strm_in1,
    hls::stream<bit32_t> &strm_in2,
    hls::stream<bit32_t> &strm_out
)
{
  // Declare the input and output variables
  bit64_t in1[65536];
  bit64_t in2[65536];
  double out[65536];
  complex<double> complex_In1[65536];
  complex<double> complex_In2[65536];
  double input_data_re = 0;

  //-------------------------------------------------------
  // Input processing
  //-------------------------------------------------------
  // Read the two input 32-bit words
  bit32_t input1_lo;
  bit32_t input2_lo;
  bit32_t input1_hi;
  bit32_t input2_hi;

  for(int i = 0; i < 65536 ;i++) 
  {
    input1_lo = strm_in1.read();
    input1_hi = strm_in1.read();
    input2_lo = strm_in2.read();
    input2_hi = strm_in2.read();
    in1[i](31, 0) = input1_lo;
    in1[i](63,32) = input1_hi;
    in2[i](31, 0) = input2_lo;
    in2[i](63,32) = input2_hi;
  }

  for(int m = 0; m < 65536 ;m++)
  {
    input_data_re = in1[m];
    complex_In1[m] = complex<double>(input_data_re, 0);
    input_data_re = in2[m];
    complex_In2[m] = complex<double>(input_data_re, 0);
  }
 
  // ------------------------------------------------------
  // Call Hybrid Imaging
  // ------------------------------------------------------
  hybrid_image(16, complex_In1, complex_In2, out );

  // ------------------------------------------------------
  // Output processing
  // ------------------------------------------------------
  // Write out the computed digit value
  for(int i = 0; i < 65536 ;i++)  
  {
    strm_out.write( out[i] );
  }
}

//----------------------------------------------------------
// FFT/IFFT shift function
//----------------------------------------------------------
//fft shift function

void fftshift(complex<double> out[65536], complex<double> in[65536], int xdim, int ydim, int xshift, int yshift)
{
int ii = 0;
int jj = 0;
  for (int i =0; i < xdim; i++) {
    ii = (i + xshift) % xdim;
    for (int j = 0; j < ydim; j++) {
      jj = (j + yshift) % ydim;
      out[ii * ydim + jj] = in[i * ydim + j];
    }
  }
}

//----------------------------------------------------------
// FFT/IFFT function
//----------------------------------------------------------
//FFT function

void FFT(int dir, long m, complex <double> x[65536])
{
   long i, i1, i2,j, k, l, l1, l2, n;
   complex <double> tx, t1, u, c;

   // Calculate the number of points

   n = 1;
   for(i = 0; i < m; i++)
      n <<= 1;
   i2 = n >> 1;
   j = 0;
   for (i = 0; i < n-1 ; i++)
   {
      if (i < j)
         swap(x[i], x[j]);
      k = i2;
      while (k <= j)
          {
         j -= k;
         k >>= 1;
      }
      j += k;
   }
   c.real(-1.0);
   c.imag(0.0);
   l2 = 1;
   for (l = 0; l < m; l++)
   {
      l1 = l2;
      l2 <<= 1;
      u.real(1.0);
      u.imag(0.0);

      for (j = 0; j < l1; j++)
          {
         for (i = j; i < n; i += l2)
                 {
            i1 = i + l1;
            t1 = u * x[i1];
            x[i1] = x[i] - t1;
            x[i] += t1;
         }
         u = u * c;
      }
      c.imag(sqrt((1.0 - c.real()) / 2.0));
      if (dir == 1)
         c.imag(-c.imag());
      c.real(sqrt((1.0 + c.real()) / 2.0));
   }
   if (dir == 1)
   {
      for (i = 0; i < n; i++)
         x[i] /= n;
   }
return;
}



//----------------------------------------------------------
// Gaussian filtering
//----------------------------------------------------------
// @param[in] : input - width and height of an image, high/low 
// @return : the filtered matrix

void GaussFilter(int imgwidth, int imgheight, complex<double> F[65536], bool High)
{
  double dist;
  complex<double> H[256][256];   //Complex double array for saving Gaussian mask

  double D0,D;
  double B[65536];   //65536 is the total number pixels available in the image
  double S[65536];

 //Calculating the gaussian mask. Magnitude and the Phase values are saved in seperate arrays.
 //Referenced from Online source. The mask is for low pass filter.

  int i = 0;
  for (int u=0; u<imgwidth; u++)
    for (int v=0; v<imgheight; v++) {
      dist = sqrt((double)(v-imgwidth/2)*(v-imgwidth/2)+(u-imgheight/2)*(u-imgheight/2));
      if (High == 1){
      H[u][v] = 1 - exp(-((dist*dist)/(2*35*35)));}
      else{
      H[u][v] = exp(-((dist*dist)/(2*15*15)));}

      F[i] = F[i] * H[u][v];
      i++;
    }
}
//----------------------------------------------------------
// Normalization
//----------------------------------------------------------
//normalize the output values between 0 and 255

void normalize(complex<double> imgNormIn[65536], double imgNormOut[65536])
{

   double minValue = logf(sqrtf((imgNormIn[0].real()* imgNormIn[0].real()) + (imgNormIn[0].imag()*imgNormIn[0].imag())) + 1) ;
   double maxValue = minValue;
   double tempValue = 0;

   for (int j = 0; j < 65536; j++)
    {
        tempValue = logf(sqrtf((imgNormIn[j].real()* imgNormIn[j].real()) + (imgNormIn[j].imag()*imgNormIn[j].imag())) + 1);
            if(tempValue > maxValue){
        maxValue = tempValue;}
            if(tempValue < minValue){
        minValue = tempValue;}
            imgNormOut[j] = tempValue;
    }

   for(int k = 0; k<65536;k++)
    {
        imgNormOut[k] = ((imgNormOut[k] - minValue)*(255/(maxValue-minValue)));
    }

}

//----------------------------------------------------------
// hybrid imaging
//----------------------------------------------------------
// @param[in] : input - 2 images
// @return : the hybrid image

void hybrid_image(int intImgSize, complex<double> imgLo_input[65536], complex<double> imgHi_input[65536],  double imgOutput[65536])
{

   complex<double> imgLo_FFTS_output[65536];
   complex<double> imgLo_IFFTS_output[65536];
   complex<double> imgHi_FFTS_output[65536];
   complex<double> imgHi_IFFTS_output[65536];
   complex<double> hybrid_output[65536];

   // FFT

   FFT(1, intImgSize, imgLo_input);
   FFT(1, intImgSize, imgHi_input);

   // FFT shift

   fftshift(imgLo_FFTS_output,imgLo_input,256,256,(256/2),(256/2));
   fftshift(imgHi_FFTS_output,imgHi_input,256,256,(256/2),(256/2));

   // Gaussian filtering with different co-efficients for two images

   GaussFilter(256,256,imgLo_FFTS_output,0);
   GaussFilter(256,256,imgHi_FFTS_output,1);

   // FFT inverse shift

   fftshift(imgLo_IFFTS_output,imgLo_FFTS_output,256,256,(256/2),(256/2));
   fftshift(imgHi_IFFTS_output,imgHi_FFTS_output,256,256,(256/2),(256/2));

   // Inverse FFT

   FFT(-1,intImgSize,imgLo_IFFTS_output);
   FFT(-1,intImgSize,imgHi_IFFTS_output);

   // Normalizing the hybrid image

   for(int k = 0; k<65536;k++)
    {
        hybrid_output[k] = imgLo_IFFTS_output[k] + imgHi_IFFTS_output[k];
    }

   normalize(hybrid_output, imgOutput);

}

