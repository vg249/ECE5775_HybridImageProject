#include "ImageProcessing.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <complex>
#define FALSE 0;
#define TRUE 1;

using namespace std;


int main()
{
	FILE * fpIn;
	char * string;
	int doneReading = FALSE;
	char c;
        
        int numberOfColumns, numberOfRows, numberOfBands, highVal, totalPixels, header, highHisto;
	unsigned char * image, * histogram, * outimageHistogram;
        
	fpIn = fopen("zhang-jpg.pgm", "rb");
	if(fpIn == NULL){
		cerr<<"Such a file does not exist...";
		exit;
	}
	string = (char *) calloc(256,1);
	while(!doneReading && (c = (char) fgetc(fpIn)) != '\0')
{
                cout << c << endl;
		switch(c){
			case 'P':
				c = (char) fgetc(fpIn);
                                cout << c << endl;
				switch(c){
					case '2':
						header = 2;
						numberOfBands = 1;
						//pgm plain
					break;
					case '5':
						header = 5;
						numberOfBands = 1;
						//pgm Normal
					break;
					case '3':
						header = 3;
						numberOfBands = 3;
						//ppm plain
					break;
					case '6':
						header = 6;
						numberOfBands = 3;
						//ppm Normal
					break;
				}
				c = (char) fgetc(fpIn);
				if(c != 0x0A){
					ungetc(c, fpIn);
				}
				else{
					ungetc(c, fpIn);
					fgets(string, 256, fpIn);
				}
			break;
			case '#':
			fgets(string, 256, fpIn);
			cout<<"File you entered is "<<string<<"\n";
			break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				ungetc(c, fpIn);
				fscanf(fpIn,"%d %d %d", &(numberOfColumns), &(numberOfRows), &(highVal));
				doneReading = TRUE;
				fgetc(fpIn);
                                cout << "I Came Inside" << std::endl;
			break;
		}
}
	totalPixels = numberOfRows*numberOfColumns*numberOfBands;
        cout << numberOfBands << endl;
//        for(int i=0;i<numberOfRows;i++)  
//	{	
//           for(int j=0;j<numberOfRows;j++)
//            {
//		c = fgetc(fpIn);
//
//                if(j==0)
//                printf("{0x%x,",(unsigned char)c);
//                else if(j==numberOfColumns-1 && i!=numberOfRows-1 )
//                printf("0x%x},\n",(unsigned char)c);
//                else if(j==numberOfColumns-1 && i==numberOfRows-1 )
//                printf("0x%x}\n",(unsigned char)c);
//                else
//                printf("0x%x,",(unsigned char)c);
//            }
//            printf("\n");
//
//            
//	}
//        
	image = (unsigned char *) malloc (totalPixels);
	size_t result;
        result = fread(image,1,totalPixels,fpIn);
        for(int i=0;i<totalPixels;i++)  
	{
			
                printf("%d\n",image[i]);
            
	}
        printf("0x%x \n", result); 
	cout<<"Reading the image "<<"of zhiru "<<" was sucessfull...\n";
        free(image);
}

int FFT(int dir,int m,double *x,double *y)
{
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i=0;i<m;i++)
      nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1;
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1)
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<nn;i++) {
         x[i] /= (double)nn;
         y[i] /= (double)nn;
      }
   }

   return(TRUE);
}

/*-------------------------------------------------------------------------
   Calculate the closest but lower power of two of a number
   twopm = 2**m <= n
   Return TRUE if 2**m == n
*/
int Powerof2(int n,int *m,int *twopm)
{
   if (n <= 1) {
      *m = 0;
      *twopm = 1;
      return(FALSE);
   }

   *m = 1;
   *twopm = 2;
   do {
      (*m)++;
      (*twopm) *= 2;
   } while (2*(*twopm) <= n);

   if (*twopm != n)
      return(FALSE);
   else
      return(TRUE);
}
