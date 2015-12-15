//==================================================================================
// Hybrid Imaging
//==================================================================================
// This takes in two images and produces an hybrid image by filtering the two images
// with different co-efficients of gaussian filter and combine them

#include "CImg.h"
#include <cmath>
#include <iostream>
#include <string.h>
#include <complex>
#include "timer.h"

#define PI 3.14159

using namespace std;
using namespace cimg_library;

CImgList<double> GaussFilter(int imgwidth, int imgheight, CImgList<double> F, bool High)
{
  double dist;
  complex<double> H[64][64];   //Complex double array for saving Gaussian mask

  double D0,D;
  double B[4096];   //65536 is the total number pixels available in the image
  double S[4096];  

  //Calculating the gaussian mask. Magnitude and the Phase values are saved in seperate arrays.
  //Referenced from Online source. The mask is for low pass filter.

  int i = 0;
  for (int u = 0 ; u < imgwidth ; u++)
    for (int v = 0 ; v < imgheight ; v++) {
      dist = sqrt((double)(u-imgwidth/2)*(u-imgwidth/2)+(v-imgheight/2)*(v-imgheight/2));
      if (High == 1)
      {
        H[u][v] = 1 - exp(-((dist*dist)/(2*35*35)));
      }
      else 
      {
        H[u][v] = exp(-((dist*dist)/(2*10*10)));
      }
      B[i] = std::real(H[u][v]);
      S[i] = std::imag(H[u][v]);
      i++; 
    }

  //Multiplying the Magnitude of Gaussian Mask with Magnitude of FFT result

  for (int z = 0 ; z < (imgwidth*imgheight); z++)
  {
    F[0][z] = (F[0][z]*B[z]) - (F[1][z]*S[z]); 
    F[1][z] = (F[0][z]*S[z]) + (F[1][z]*B[z]); 
  }  
  return F;
} 


int main() {

  // Timer

  Timer timer("hybrid image CImg");
  timer.start();
 
  //Reading the two images

  const CImg<double> imglo = CImg<double>("marilyn.png").resize(64,64).save("original.png");
  const CImg<double> img   = CImg<double>("einstein.png").resize(64,64).save("originallo.png");

  //Applying fourier transform. Referenced it frm CImg.h. 
  //Returns list in 0 and 1 column. We assummed the values in 0 column are magnitude and 1 column are phase

  CImgList<double> F   = img.get_FFT();
  CImgList<double> Flo = imglo.get_FFT();

  //FFT Shift. Referenced from CImg.h

  cimglist_apply(F,shift)(img.width()/2,img.height()/2,0,0,2);
  cimglist_apply(Flo,shift)(imglo.width()/2,imglo.height()/2,0,0,2);

  // Apply the filtering with different co-efficients for two images

  F   = GaussFilter(img.width(), img.height(),F,1); 
  Flo = GaussFilter(imglo.width(), imglo.height(),Flo,0); 

  // Inverse FFT shift

  cimglist_apply(F,shift)(-img.width()/2,-img.height()/2,0,0,2);
  cimglist_apply(Flo,shift)(-imglo.width()/2,-imglo.height()/2,0,0,2);
  
  //Taking Inverse FFT of the Result

  CImgList<double> FT   = F.get_FFT(true);
  CImgList<double> FTlo = Flo.get_FFT(true);
  
  // Add the two images

  for(int b = 0 ; b < (img.height()*img.width()) ; b++ )
  {
    FT[0][b] = FT[0][b]+FTlo[0][b];
    FT[1][b] = FT[1][b]+FTlo[1][b];
  }

  // Calculate the magnitude with normalization

  const CImg<double> mag   = ((FT[0].get_pow(2) + FT[1].get_pow(2)).sqrt() + 1).log().normalize(0,200);
 
  // Convert to hybrid image

  CImgList<double> visu(img,mag);
  mag.save("hybrid_image.png");    // Save the image

  timer.stop();
} 
