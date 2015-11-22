#include "ImageProcessing.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
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
                printf("0x%x\n",image[i]);
            
	}
        printf("0x%x \n", result); 
	cout<<"Reading the image "<<"of zhiru "<<" was sucessfull...\n";
        free(image);
}
