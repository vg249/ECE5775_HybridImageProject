#include "hls_video.h"
#include "hls_opencv.h"

typedef hls::stream<ap_axiu<32,1,1,1> > AXI_STREAM;

int main (int argc, char** argv) {

	// Load data in OpenCV image format

	IplImage* src = cvLoadImage("/home/student/vg249/sample.bmp");
	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

	AXI_STREAM src_axi, dst_axi;

	// Convert OpenCV format to AXI4 Stream format
	IplImage2AXIvideo(src, src_axi);

	char * OUTPUT_IMAGE = "";

	// Call the function to be synthesized

	// Convert the AXI4 Stream data to OpenCV format
	AXIvideo2IplImage(dst_axi, dst);

	// Standard OpenCV image functions
	cvSaveImage(OUTPUT_IMAGE, dst);

	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	char tempbuf[2000];
	return 1;


}
