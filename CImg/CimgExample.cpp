#include "CImg.h"
#include <cmath>
#include <string.h>
#include <complex>

#define PI 3.14159

using namespace std;
using namespace cimg_library;

CImg<double> Multiply(CImg<double> Image){
        printf("Multiply (-1)^(x+y) to center the transform for filtering\n");
        CImg<double> MultiplyImage(Image.width(),Image.height(),1,1,0);
        for ( int  i = 0; i < Image.width(); i++){
                for ( int j = 0; j < Image.height(); j++)
                        if ( (i + j)%2 != 0)
                                MultiplyImage(i,j) = -Image(i,j);
                        else
                                MultiplyImage(i,j) = Image(i,j);

        }
        CImgDisplay MultiplyDisplay(MultiplyImage, "After (-1)^(x+y)");
        system("PAUSE");
        return MultiplyImage;

}

void Gaussian(CImg<double> Image) {
        printf("Construct Gaussian highpass filter\n");
        complex<double> H[100][100];
        double D0, D;
        printf("Please input radii:");
        scanf("%lf", &D0);
        for ( int u = 0; u < Image.width() ; u++){
                for ( int v = 0; v < Image.height() ; v++){
                        D = sqrt(pow((double)u - ((double)Image.width()/2),2) + pow((double)v - ((double)Image.height()/2),2));
                        H[u][v] =(complex<double>)(1,0.0)- exp(complex<double>(0.0,-(double)((double)pow(D, 2)/ (double)(2* pow(D0,2)))));
                }
        }

}


int main() {
  
  const CImg<double> img = CImg<double>("marilyn1.png").resize(256,256);
//  CImg<double> img = Multiply(Originalimg);

  CImgList<double> F = img.get_FFT();
  cimglist_apply(F,shift)(img.width()/2,img.height()/2,0,0,2);
  
//CImgList<float> F = FT.get_FFT(true);

  complex<double> H[256][256];
  double D0,D;

  for ( int u = 0; u < img.width() ; u++){
    for ( int v = 0; v < img.height() ; v++){
      D = sqrt(pow((double)u - ((double)img.width()/2),2) + pow((double)v - ((double)img.height()/2),2));
      H[u][v] =(complex<double>)(1,0.0)- exp(complex<double>(0.0,-(double)((double)pow(D, 2)/ (double)(2* pow(D0,2)))));
       }
   }
//complex<double> S[][];
  // for ( int u = 0; u < img.width() ; u++){
//	for ( int v = 0; v < img.height() ; v++)
//	   printf("%f", F[u][v]);
//*= H[u][v];
//	}
  
  CImgList<double> FT = F.get_FFT(true);
//  printf("%f",H[0][0]);
  const CImg<double> mag = ((FT[0].get_pow(2) + FT[1].get_pow(2)).sqrt() + 1).log().normalize(0,255);
 //const CImg<unsigned char> mag = ((F[0].get_pow(2) + F[1].get_pow(2)).sqrt() + 1);
//const CImg<unsigned char> mag = ((FT[0].get_pow(2) + FT[1].get_pow(2)).sqrt() + 1);
  CImgList<double> visu(img,mag);
  mag.save("fftimage.png");
  
}


