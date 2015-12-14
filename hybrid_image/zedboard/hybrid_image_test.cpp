//#include <iostream>
//#include <cstdlib>
//#include <cstring>
//#include <sstream>
//#include "hybrid_image.h"
//#include <stdio.h>
//#include "timer.h"

//#include <fstream>
//#include <string>
//#include <sstream>

//#include "CImg.h"
//#include <cmath>
//#include <iostream>
//#include <string.h>
//#include <complex>
//#include "hls_math.h"
#define PI 3.14159

#include <stdio.h>
#include <iostream>
#include <complex>
//#include "hls_math.h"

#include "hybrid_image.h"
#include "timer.h"
#include "CImg.h"


using namespace std;
using namespace cimg_library;

#define BUF_SIZE 64


int main()
{

  // HLS streams

  hls::stream<bit32_t> hybrid_image_in1;
  hls::stream<bit32_t> hybrid_image_out;

  //float array declaration for Input Images

  complex<float> xnLo_input[4096];
  complex<float> xnHi_input[4096];

  //float arrat declaration for output images

  float xn_output[4096];

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

//  bit64_t img1[4096];
//  bit64_t img2[4096];
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
bit32_t hybrid_out;
  for (int i = 0; i < 4096 ; i++ )
  {
    hybrid_out = hybrid_image_out.read();
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
