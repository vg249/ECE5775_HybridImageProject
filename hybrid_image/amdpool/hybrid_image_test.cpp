#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include "fft.h"
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
  hls::stream<bit32_t> hybrid_image_in2;
  hls::stream<bit32_t> hybrid_image_out;

  //double array declaration for Input Images

  complex<double> xnLo_input[65536];
  complex<double> xnHi_input[65536];

  //double arrat declaration for output images

  double xn_output[65536];

  //Image Output
    
  CImg<double> imgOutput(256,256,1,1,0);

  //Reading the images into double array using CImg Library

  const CImg<double> imgLo = CImg<double>("marilyn1.png").resize(256,256).save("resize.png");
  const CImg<double> imgHi = CImg<double>("einstein.png").resize(256,256).save("einsteinresize.png");

  //Converting the image pixel values to complex numbers

  double input_data_re = 0;

  for(int m = 0; m < 65536 ;m++)
  {
    input_data_re = imgLo[m];
    xnLo_input[m] = complex<double>(input_data_re, 0);
    input_data_re = imgHi[m];
    xnHi_input[m] = complex<double>(input_data_re, 0);	
  }

//  timer.start();

  //--------------------------------------------------------------------
  // Send data input images
  //--------------------------------------------------------------------

  for (int i = 0; i < 65536 ; i++ )
  {
     
    hybrid_image_in1.write(imgLo[i]);
    hybrid_image_in2.write(imgHi[i]);

  }

  //--------------------------------------------------------------------
  // Execute the hybrid image
  //--------------------------------------------------------------------

  dut (hybrid_image_in1,hybrid_image_in2,hybrid_image_out);

  //--------------------------------------------------------------------
  // Receive the hybrid image matrix
  //--------------------------------------------------------------------

  for (int i = 0; i < 65536 ; i++ )
  {
    xn_output[i] = hybrid_image_out.read();
  }

//  timer.stop();

  //saving the output values as hybrid image

  for(int k = 0; k <65536 ;k++)
  {
    imgOutput[k] = xn_output[k];
  }

  imgOutput.save("output.png");
}