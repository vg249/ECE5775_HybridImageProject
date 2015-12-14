//==========================================================================
//CimgExample.cpp
//==========================================================================
// @brief: A Hybrid imaging of 2 images with different fitering

//#include "CImg.h"
#include "hybrid_image.h"
//#include <cmath>
//#include <iostream>
//#include <string.h>
#include <complex>
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
    hls::stream<bit32_t> &strm_in,
    hls::stream<bit32_t> &strm_out
)
{
  // Declare the input and output variables
  complex<float> out[4096];
  complex<float> complex_In1[4096];
  complex<float> complex_In2[4096];
  float input_data_re = 0;

  //-------------------------------------------------------
  // Input processing
  //-------------------------------------------------------
  // Read the two input 32-bit words
  bit32_t input1_lo;
  bit32_t input2_hi;
  bit32_t output_r;
  bit32_t output_i;
 
  for(int i = 0; i < 4096 ;i++) 
  {
    input1_lo = strm_in.read();
    input2_hi = strm_in.read();
    input_data_re = input1_lo;
    complex_In1[i] = complex<float>(input_data_re, 0);
    input_data_re = input2_hi;
    complex_In2[i] = complex<float>(input_data_re, 0);

  }

//  for(int m = 0; m < 4096 ;m++)
//  {
////    input_data_re = in1[m];
//    complex_In1[m] = complex<float>(80, 0);
////    input_data_re = in2[m];
//    complex_In2[m] = complex<float>(80, 0);
//  }
 
  // ------------------------------------------------------
  // Call Hybrid Imaging
  // ------------------------------------------------------
  hybrid_image(12, complex_In1, complex_In2, out );

  // ------------------------------------------------------
  // Output processing
  // ------------------------------------------------------
  // Write out the computed digit value
  
  for(int i = 0; i < 4096 ;i++)  
  { 
//    printf("%f\n",out[i]);
//    output = out[i];
    output_r = out[i].real();
    output_i = out[i].imag();
    strm_out.write(output_r);
    strm_out.write(output_i );
  }
}

//----------------------------------------------------------
// FFT/IFFT shift function
//----------------------------------------------------------
//fft shift function

void fftshift(complex<float> out[4096], complex<float> in[4096], int xdim, int ydim, int xshift, int yshift)
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

void FFT(int dir, int m, complex <float> x[4096])
{
   int i, i1, i2,j, k, l, l1, l2, n;
   complex <float>  t1, u, c, temp;

   // Calculate the number of points

   n = 1;
   for(i = 0; i < m; i++)
      n <<= 1;
   i2 = n >> 1;
   j = 0;
   for (i = 0; i < n-1 ; i++)
   {
      if (i < j) {
//         swap(x[i], x[j]);
        temp = x[i];
        x[i] = x[j];
        x[j] = temp;
      }   
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

void GaussFilter(int imgwidth, int imgheight, complex<float> F[4096], bool High)
{
  float dist;
  complex<float> H[64][64];   //Complex float array for saving Gaussian mask

//  float D0,D;
//  float B[4096];   //4096 is the total number pixels available in the image
//  float S[4096];

 //Calculating the gaussian mask. Magnitude and the Phase values are saved in seperate arrays.
 //Referenced from Online source. The mask is for low pass filter.

  int i = 0;
  for (int u=0; u<imgwidth; u++)
    for (int v=0; v<imgheight; v++) {
      dist = sqrt((float)(v-imgwidth/2)*(v-imgwidth/2)+(u-imgheight/2)*(u-imgheight/2));
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

//void normalize(complex<float> imgNormIn[4096], float imgNormOut[4096])
//{
//
//   float minValue = logf(sqrtf((imgNormIn[0].real()* imgNormIn[0].real()) + (imgNormIn[0].imag()*imgNormIn[0].imag())) + 1) ;
//   float maxValue = minValue;
//   float tempValue = 0;
//
//   for (int j = 0; j < 4096; j++)
//    {
//        tempValue = logf(sqrtf((imgNormIn[j].real()* imgNormIn[j].real()) + (imgNormIn[j].imag()*imgNormIn[j].imag())) + 1);
//            if(tempValue > maxValue){
//        maxValue = tempValue;}
//            if(tempValue < minValue){
//        minValue = tempValue;}
//            imgNormOut[j] = tempValue;
//    }
//
//   for(int k = 0; k<4096;k++)
//    {
//        imgNormOut[k] = ((imgNormOut[k] - minValue)*(255/(maxValue-minValue)));
//    }
//
//}

//----------------------------------------------------------
// hybrid imaging
//----------------------------------------------------------
// @param[in] : input - 2 images
// @return : the hybrid image

void hybrid_image(int intImgSize, complex<float> imgLo_input[4096], complex<float> imgHi_input[4096],  complex<float> hybrid_output[4096])
{

   complex<float> img_FFTS_output[4096];
   complex<float> img_IFFTS_output[4096];

//   complex<float> hybrid_output[4096];

   FFT(1, intImgSize, imgLo_input);

   fftshift(img_FFTS_output,imgLo_input,64,64,(64/2),(64/2));

   GaussFilter(64,64,img_FFTS_output,0);

   fftshift(img_IFFTS_output,img_FFTS_output,64,64,(64/2),(64/2));

   FFT(-1,intImgSize,img_IFFTS_output);

   for(int k = 0; k<4096;k++)
    {
        hybrid_output[k] = img_IFFTS_output[k];
    }

   FFT(1, intImgSize, imgHi_input);
   fftshift(img_FFTS_output,imgHi_input,64,64,(64/2),(64/2));
   GaussFilter(64,64,img_FFTS_output,1);
   fftshift(img_IFFTS_output,img_FFTS_output,64,64,(64/2),(64/2));
   FFT(-1,intImgSize,img_IFFTS_output);
   for(int k = 0; k<4096;k++)
    {
        hybrid_output[k] = hybrid_output[k] + img_IFFTS_output[k];
    }

//   normalize(hybrid_output, imgOutput);

}

