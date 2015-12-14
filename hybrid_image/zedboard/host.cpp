#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<math.h>
#include<assert.h>

#include<iostream>
#include<fstream>

#include "CImg.h"
#include "timer.h"

//--------------------------------------
// main function
//--------------------------------------
int main(int argc, char** argv)
{
  // Open channels to the FPGA board.
  // These channels appear as files to the Linux OS
  int fdr = open("/dev/xillybus_read_32", O_RDONLY);
  int fdw = open("/dev/xillybus_write_32", O_WRONLY);

  // Check that the channels are correctly opened
  if ((fdr < 0) || (fdw < 0)) {
    fprintf (stderr, "Failed to open Xillybus device channels\n");
    exit(-1);
  }

  //float array declaration for Input Images

  complex<float> xnLo_input[4096];
  complex<float> xnHi_input[4096];

  //float arrat declaration for output images

  float xn_output[4096];

  //Image Output

  CImg<float> imgOutput(64,64,1,1,0);

  //Reading the images into float array using CImg Library

  const CImg<float> imgLo = CImg<float>("data/marilyn1.png").resize(64,64).save("resize.png");
  const CImg<float> imgHi = CImg<float>("data/einstein.png").resize(64,64).save("einsteinresize.png");

  //Converting the image pixel values to complex numbers

  float input_data_re = 0;

  // Timer
  Timer timer("digitrec FPGA");

  //--------------------------------------------------------------------
  // Send data input images
  //--------------------------------------------------------------------

  bit32_t input1_lo;
  bit32_t input2_hi;
  for (int i = 0; i < 4096 ; i++ )
  {
    bit32_t input1_lo = imgLo[i];
    bit32_t input2_hi = imgHi[i];
 
    // Write words to the device
    nbytes = write (fdw, (void *)&input1_lo, sizeof(input1_lo));
    assert (nbytes == sizeof(input1_lo));
    nbytes = write (fdw, (void *)&input2_lo, sizeof(input2_lo));
    assert (nbytes == sizeof(input2_lo));

  }

//  dut (hybrid_image_in1,hybrid_image_out);

  //--------------------------------------------------------------------
  // Receive the hybrid image matrix
  //--------------------------------------------------------------------

  bit32_t hybrid_out;

  for (int i = 0; i < 4096 ; i++ )
  {
//    hybrid_out = hybrid_image_out.read();
    nbytes = read (fdr, (void *)&hybrid_out, sizeof(hybrid_out));
    assert (nbytes == sizeof(hybrid_out));
    xn_output[i] = hybrid_out;
  }

  //saving the output values as hybrid image

  for(int k = 0; k <4096 ;k++)
  {
    imgOutput[k] = xn_output[k];
  }

  imgOutput.save("output.png");
}

