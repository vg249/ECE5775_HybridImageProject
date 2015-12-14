#include "hybrid_image.h"
#include "timer.h"

#include <string>
#include <sstream>

#include "CImg.h"
#include <cmath>
#include <iostream>
#include <string.h>
#include <complex>
#include "hls_math.h"
#define PI 3.14159


using namespace std;
using namespace cimg_library;

#define BUF_SIZE 32

//----------------------------------------------------------
// Normalization
//----------------------------------------------------------
//normalize the output values between 0 and 255

void normalize(complex<float> imgNormIn[4096], float imgNormOut[4096])
{

   float minValue = logf(sqrtf((imgNormIn[0].real()* imgNormIn[0].real()) + (imgNormIn[0].imag()*imgNormIn[0].imag())) + 1) ;
   float maxValue = minValue;
   float tempValue = 0;

   for (int j = 0; j < 4096; j++)
    {
        tempValue = logf(sqrtf((imgNormIn[j].real()* imgNormIn[j].real()) + (imgNormIn[j].imag()*imgNormIn[j].imag())) + 1);
            if(tempValue > maxValue){
        maxValue = tempValue;}
            if(tempValue < minValue){
        minValue = tempValue;}
            imgNormOut[j] = tempValue;
    }

   for(int k = 0; k<4096;k++)
    {
        imgNormOut[k] = ((imgNormOut[k] - minValue)*(255/(maxValue-minValue)));
    }

}


int main()
{

  // HLS streams

  hls::stream<bit32_t> hybrid_image_in1;
  hls::stream<bit32_t> hybrid_image_out;

  //float array declaration for Input Images

  complex<float> xnLo_input[4096];
  complex<float> xnHi_input[4096];

  //float arrat declaration for output images

  complex<float> xn_output[4096];

  //Image Output
    
  CImg<float> imgOutput(64,64,1,1,0);

  //Reading the images into float array using CImg Library

  const CImg<float> imgLo = CImg<float>("marilyn1.png").resize(64,64).save("resize.png");
  const CImg<float> imgHi = CImg<float>("einstein.png").resize(64,64).save("einsteinresize.png");

  //Converting the image pixel values to complex numbers

  float input_data_re = 0;


//  timer.start();

  //--------------------------------------------------------------------
  // Send data input images
  //--------------------------------------------------------------------

//  bit32_t img1[4096];
//  bit32_t img2[4096];
  bit32_t input1_lo;
  bit32_t input2_hi;
  for (int i = 0; i < 4096 ; i++ )
  {
    bit32_t input1_lo = imgLo[i];
    bit32_t input2_hi = imgHi[i]; 

    hybrid_image_in1.write(input1_lo);
    hybrid_image_in1.write(input2_hi);

  }

  //--------------------------------------------------------------------
  // Execute the hybrid image
  //--------------------------------------------------------------------

  dut (hybrid_image_in1,hybrid_image_out);

  //--------------------------------------------------------------------
  // Receive the hybrid image matrix
  //--------------------------------------------------------------------
  bit32_t hybrid_out_r;
  bit32_t hybrid_out_i;
  float xn_output_norm[4096];

  for (int i = 0; i < 4096 ; i++ )
  {
    hybrid_out_r = hybrid_image_out.read();
    hybrid_out_i = hybrid_image_out.read();
    xn_output[i] = complex<float>(hybrid_out_r,hybrid_out_i);
  }

//  timer.stop();
  normalize(xn_output, xn_output_norm);
  //saving the output values as hybrid image

  for(int k = 0; k <4096 ;k++)
  {
    imgOutput[k] = xn_output_norm[k];
//    printf("%f\n",xn_output[k]);
  }

  imgOutput.save("output.png");
}
