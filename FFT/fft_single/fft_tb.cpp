/*******************************************************************************
Vendor: Xilinx 
Associated Filename: fft_tb.cpp
Purpose: Xilinx FFT IP-XACT IP in Vivado HLS
Revision History: September 26, 2013 - initial release
                                                
*******************************************************************************
#-  (c) Copyright 2011-2014 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************


This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include "fft_top.h"
#include <stdio.h>

// For reading from stimulus input files
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

void ifftshift(complex<double> out[65536], complex<double> in[65536], int xdim, int ydim, int xshift, int yshift)
{
int ii = 0;
int jj = 0;
  for (int i = (xdim-1); i >= 0; i--) {
    ii = (i + xshift) % xdim;
    for (int j = (ydim-1); j >= 0; j--) {
      jj = (j + yshift) % ydim;
      out[ii * ydim + jj] = in[i * ydim + j];
    }
  }
}


int main()
{
    const int SIM_FRAMES = 20;
    const int SAMPLES = (1 << 3);

    int error_num = 0;
    bool ovflo_all = false;
    char res_filename[BUF_SIZE]={0};
    char dat_filename[BUF_SIZE]={0};
//    static cmpxDataIn xn_input[8];
    complex<double> xn_input[65536];
    complex<double> xnf_output[65536];
    complex<double> xnif_output[65536];

    const CImg<double> img = CImg<double>("/home/student/vg249/ECE5775_HybridImageProject/FFT/fft_ifft/marilyn1.png").resize(256,256).save("/home/student/vg249/ECE5775_HybridImageProject/FFT/fft_single/resize.png");
        
	int NFFT = 16;
        int CP_LEN = 0; // length of the cyclic prefix to be inserted for each frame
        int FWD_INV = 1;
        int sc_sch = 0;
        int line_no = 1;
        const double sc = ldexp(1.0, FFT_INPUT_WIDTH-1);

//******************************Converting the pixels to Complex data as FFT function take Complex Input*********************************

for(int m = 0; m < 65536 ;m++)
{
  double input_data_re;
  input_data_re = img[m];
  xn_input[m] = complex<double>(input_data_re, 0);
	
}

FFT(1,16,xn_input);       

fftshift(xnf_output,xn_input,256,256,(256/2),(256/2));

for(int l = 0; l < 65536; l++)
{
	   cout << "Frame:" << " index: " << l
                 << "  RE Output: " << xn_input[l].real() << endl;
           cout << "Frame:" << " index: " << l
                 << " vs. IM Output: " << xn_input[l].imag() << endl;

}

ifftshift(xnif_output,xnf_output,256,256,(-256/2),(-256/2));

FFT(-1,16,xnif_output);        

	//**************************************************** Simulate the IFFT.*************************************************

//        fft_top(0, xk_output, xki_output, &ovflo);
//
//        ovflo_all |= ovflo;
//
//***********************************************************Normalize the values in between 0 and 255*********************************


       double minValue = log10f(sqrtf((xnif_output[0].real()* xnif_output[0].real()) + (xnif_output[0].imag()*xnif_output[0].imag())) + 1) ;
       double maxValue = minValue;
       CImg<double> imgOutput(256,256,1,1,0);
//       imgOutput = (double *)malloc(65536*sizeof(double));
       for (int j = 0; j < 65536; j++)
        { 
  		double tempValue = log10f(sqrtf((xnif_output[j].real()* xnif_output[j].real()) + (xnif_output[j].imag()*xnif_output[j].imag())) + 1);
//                printf("%f\n",(sqrtf((xki_output[j].real()* xki_output[j].real()) + (xki_output[j].imag()*xki_output[j].imag())) + 1));
                if(tempValue > maxValue){
		maxValue = tempValue;}
                if(tempValue < minValue){
		minValue = tempValue;}
                imgOutput[j] = tempValue;
	}

      for(int k = 0; k<65536;k++)
	{
		imgOutput[k] = ((imgOutput[k] - minValue)*(200/(maxValue-minValue)));
//	        printf("%f\n%f\n\n",minValue,maxValue);  
//              printf("%f\n",imgOutput[k]);
	} 
	imgOutput.save("/home/student/vg249/ECE5775_HybridImageProject/FFT/fft_single/output.png");


}
