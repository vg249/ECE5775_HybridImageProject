#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include "hybrid_image.h"
#include <stdio.h>
#include "timer.h"

#include <fstream>
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

#define BUF_SIZE 64


int main()
{

  // HLS streams

  hls::stream<bit32_t> hybrid_image_in1;
  hls::stream<bit32_t> hybrid_image_out;

  //double array declaration for Input Images

  complex<double> xnLo_input[4096];
  complex<double> xnHi_input[4096];

  //double arrat declaration for output images

  double xn_output[4096];

  //Image Output
    
  CImg<double> imgOutput(64,64,1,1,0);

  //Reading the images into double array using CImg Library

  const CImg<double> imgLo = CImg<double>("marilyn1.png").resize(64,64).save("resize.png");
  const CImg<double> imgHi = CImg<double>("einstein.png").resize(64,64).save("einsteinresize.png");

  //Converting the image pixel values to complex numbers

  double input_data_re = 0;


//  timer.start();

  //--------------------------------------------------------------------
  // Send data input images
  //--------------------------------------------------------------------

  bit64_t img1[4096];
  bit64_t img2[4096];

  for (int i = 0; i < 4096 ; i++ )
  {
    img1[i] = imgLo[i];
    img2[i] = imgHi[i];
    bit32_t input1_lo = img1[i].range(31,0);
    bit32_t input1_hi = img1[i].range(63,32); 
    bit32_t input2_lo = img2[i].range(31,0);
    bit32_t input2_hi = img2[i].range(63,32);

    hybrid_image_in1.write(input1_lo);
    hybrid_image_in1.write(input1_hi);
    hybrid_image_in1.write(input2_lo);
    hybrid_image_in1.write(input2_hi);

  }

  //--------------------------------------------------------------------
  // Execute the hybrid image
  //--------------------------------------------------------------------

  dut (hybrid_image_in1,hybrid_image_out);

  //--------------------------------------------------------------------
  // Receive the hybrid image matrix
  //--------------------------------------------------------------------
bit64_t hybrid_out;
  for (int i = 0; i < 4096 ; i++ )
  {
    hybrid_out.range(31,0) = hybrid_image_out.read();
    hybrid_out.range(63,32) = hybrid_image_out.read();
    xn_output[i] = hybrid_out;
  }

//  timer.stop();

  //saving the output values as hybrid image

  for(int k = 0; k <4096 ;k++)
  {
    imgOutput[k] = xn_output[k];
//    printf("%f\n",xn_output[k]);
  }

  imgOutput.save("output.png");
}
