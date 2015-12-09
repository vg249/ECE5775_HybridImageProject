/*******************************************************************************
Vendor: Xilinx 
Associated Filename: fft_top.cpp
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


#include "fft_top.h"
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


//FFT function 

void FFT(int dir, long m, complex <double> x[65536])
{
   long i, i1, i2,j, k, l, l1, l2, n;
   complex <double> tx, t1, u, c;
   /*Calculate the number of points */
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

//Gauss filter function

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


void fft_top(int intImgSize, complex<double> imgLo_input[65536], complex<double> imgHi_input[65536],  double imgOutput[65536])
{

   complex<double> imgLo_FFTS_output[65536];
   complex<double> imgLo_IFFTS_output[65536];
   complex<double> imgHi_FFTS_output[65536];
   complex<double> imgHi_IFFTS_output[65536];
   
   complex<double> hybrid_output[65536];

   FFT(1, intImgSize, imgLo_input);
   FFT(1, intImgSize, imgHi_input);
   
   fftshift(imgLo_FFTS_output,imgLo_input,256,256,(256/2),(256/2));
   fftshift(imgHi_FFTS_output,imgHi_input,256,256,(256/2),(256/2));

   GaussFilter(256,256,imgLo_FFTS_output,0);   
   GaussFilter(256,256,imgHi_FFTS_output,1);   
   
   fftshift(imgLo_IFFTS_output,imgLo_FFTS_output,256,256,(256/2),(256/2));
   fftshift(imgHi_IFFTS_output,imgHi_FFTS_output,256,256,(256/2),(256/2));

   FFT(-1,intImgSize,imgLo_IFFTS_output);        
   FFT(-1,intImgSize,imgHi_IFFTS_output);        

   for(int k = 0; k<65536;k++)
    {
    	hybrid_output[k] = imgLo_IFFTS_output[k] + imgHi_IFFTS_output[k];
    } 
   
   normalize(hybrid_output, imgOutput);
   
}
