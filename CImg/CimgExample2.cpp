#include "CImg.h"
#include <cmath>
#include <string.h>
#include <complex>

#define PI 3.14159

using namespace std;
using namespace cimg_library;

complex<double> F[100][100];
complex<double> f[100][100];
complex<double> H[100][100];

// (b) Compute the two-dimensional discrete Fourier transform.
void FourierTransform(CImg<double> Image){
	printf("Apply Fourier Transform\n");
	for( int u = 0;  u <= Image.width() - 1; u++){
		for ( int v = 0; v <= Image.height() - 1; v++){
			complex<double> temp(0.0, 0.0);
			for ( int x = 0; x <= Image.width() - 1; x++){
				for ( int y = 0; y<= Image.height() - 1; y++){
					temp += Image(x,y) * exp(complex<double>( 0.0, (-2 * PI * ((double)u*x/Image.width() + (double)v*y/Image.height()))));
				}
			}
			F[u][v] = temp / sqrt(Image.width() * Image.height());
		}	
	}

	for ( int i = 0; i < Image.width(); i++){
		for ( int j = 0; j < Image.height(); j++)
			Image(i,j) = real(F[i][j]);
	}
	CImgDisplay MultiplyDisplay(Image, "Apply Fourier Transform");
        Image.save("Fourier Transform");
	system("PAUSE");
}

//(c) Compute the inverse two-dimensional discrete Fourier transform.
CImg<double> InverseFourierTransform(CImg<double> Image){
	CImg<double> outputImage(Image.width(),Image.height(),1,1,0);
	printf("Apply Inverse Fourier Transform\n");
	int u,v;
	for(  u = 0;  u <= Image.width() - 1; u++){
		for (  v = 0; v <= Image.height() - 1; v++){
			complex<double> temp(0.0, 0.0);
			for ( int x = 0; x <= Image.width() - 1; x++){
				for ( int y = 0; y<= Image.height() - 1; y++){
					temp += F[x][y] * exp(complex<double>( 0.0, (2 * PI * ((double)u*x/Image.width() + (double)v*y/Image.height()))));
				}
			}
			f[u][v] = temp / sqrt(Image.width() * Image.height());
			
		}	
	}
   for ( int x = 0; x <= Image.width() - 1; x++){
			for ( int y = 0; y<= Image.height() - 1; y++){
					outputImage(x,y)=real(f[x][y]);
				}
			}
	return outputImage;
	system("PAUSE");
}




//(a) Multiply the input image by (-1)x+y to center the transform for filtering.
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

//(e) Compute the Fourier spectrum.
void FourierSpectrum(CImg<double> Image){
	printf("The Fourier spectrum\n");
	CImg<double> FourierSpectrumImage(Image.width(),Image.height(),1,1,0);
	for ( int i = 0; i < Image.width() ; i++){
		for ( int j = 0; j < Image.height() ; j++){
			FourierSpectrumImage(i,j) = log(1 + sqrt(pow(real(F[i][j]), 2) + pow(imag(F[i][j]), 2)));
		}
	}
	CImgDisplay FourierSpectrumDisplay(FourierSpectrumImage, "FourierSpectrum");
	system("PAUSE");
}

//partb:(a)Write a C++ function for the transfer function of the Gaussian highpass filter in Eq. (4.4-4). 
void Gaussian(CImg<double> Image){
	printf("Construct Gaussian highpass filter\n");
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

void Multiply_HF(CImg<double> Image){
	printf("Multiply H(u,v) with F(u,v)\n");
	for ( int u = 0; u < Image.width() ; u++){
		for ( int v = 0; v < Image.height() ; v++)
			F[u][v] *= H[u][v];
	}
}


int main(){
	char Pic[20];
	CImg<double> Image("marilyn1.png"),result;
	CImg<double> Result;
	complex<double> Transform[50][50];
	printf("Assignment III highpass Filtering\n");
//(a) Multiply the input image by (-1)x+y to center the transform for filtering. 
	Result = Multiply(Image);
	CImgDisplay main_disp(Result,"Multiply Image");
//(b) Compute the two-dimensional discrete Fourier transform. 
	FourierTransform(Result);
//(c) Compute the inverse two-dimensional discrete Fourier transform. 
        result=InverseFourierTransform(Image);
//(d) Multiply the result by (-1)x+y and take the real part. 
        Multiply(result);
//(e) Compute the Fourier spectrum. 
        FourierSpectrum(Image);
//partb:(a) Write a C++ function for the transfer function of the Gaussian highpass filter in Eq. (4.4-4). 
	Gaussian(Result);
	Multiply_HF(Result);
	InverseFourierTransform(Result);
	for ( int i = 0; i < Image.width(); i++){
		for ( int j = 0; j < Image.height(); j++)
			Result(i,j) = real(f[i][j]);
	}
	Result = Multiply(Result);

        Result.save("FFTExample2.jpg");
	FourierSpectrum(Result);
        return 0;     
}

