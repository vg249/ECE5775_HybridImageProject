#include "CImg.h"
#include <cmath>
#include <iostream>
#include <string.h>
#include <complex>

#define PI 3.14159

using namespace std;
using namespace cimg_library;


int main() {
 
  //Reading the image 
  const CImg<double> img = CImg<double>("marilyn1.png").resize(256,256).save("original.png");

  //Applying fourier transform. Referenced it frm CImg.h. 
  //Returns list in 0 and 1 column. We assummed the values in 0 column are magnitude and 1 column are phase
  CImgList<double> F = img.get_FFT();

  //FFT Shift. Referenced from CImg.h
  cimglist_apply(F,shift)(img.width()/2,img.height()/2,0,0,2);
  
  complex<double> H[256][256];   //Complex double array for saving Gaussian mask

  double D0,D;
  double B[65536];   //65536 is the total number pixels available in the image
  double S[65536];  

 //Calculating the gaussian mask. Magnitude and the Phase values are saved in seperate arrays.
 //Referenced from Online source. The mask is for low pass filter.
  int i = 0;
  for ( int u = 0; u < img.width() ; u++){
    for ( int v = 0; v < img.height() ; v++){
      D0 = 15;
      D = sqrt(pow((double)u - ((double)img.width()/2),2) + pow((double)v - ((double)img.height()/2),2));
      H[u][v] =  exp(complex<double>(0.0,-(double)((double)pow(D, 2)/ (double)(2* pow(D0,2)))));
      B[i] = std::abs(H[u][v]);
      S[i] = std::arg(H[u][v]); 
      i++;

       }
   }

 printf("%d",i); 
//Multiplying the Magnitude of Gaussian Mask with Magnitude of FFT result
 for (int z=0; z< i; z++)
 {
  F[0][z] = (F[0][z]*B[i]); 
 }  
  
//Taking Inverse FFT of the Result
  CImgList<double> FT = F.get_FFT(true);

  const CImg<double> mag = ((FT[0].get_pow(2) + FT[1].get_pow(2)).sqrt() + 1).log().normalize(0,255);

  CImgList<double> visu(img,mag);
  mag.save("fftimage.png");
  
}


