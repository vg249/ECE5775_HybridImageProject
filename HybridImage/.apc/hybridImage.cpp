#include "hls_video.h"
typedef hls::stream<ap_axiu<32,1,1,1> > AXI_STREAM;
typedef hls::Scalar<3, unsigned char> RGB_PIXEL;
typedef hls::Mat<1920, 1920, HLS_8UC3> RGB_IMAGE;

void image_filter(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int
cols) {
	//Create AXI streaming interfaces for the core
	RGB_IMAGE img_0(rows, cols);
	RGB_IMAGE img_1(rows, cols);
	RGB_IMAGE img_2(rows, cols);
	RGB_IMAGE img_3(rows, cols);

	RGB_IMAGE img_4(rows, cols);
	RGB_IMAGE img_5(rows, cols);
	RGB_PIXEL pix(50, 50, 50);

	// Convert AXI4 Stream data to hls::mat format
	hls::AXIvideo2Mat(INPUT_STREAM, img_0);

	// Execute the video pipelines
	hls::Sobel<1,0,3>(img_0, img_1);
	hls::SubS(img_1, pix, img_2);
	hls::Scale(img_2, img_3, 2, 0);
	hls::Erode(img_3, img_4);
	hls::Dilate(img_4, img_5);
	// Convert the hls::mat format to AXI4 Stream format
	hls::Mat2AXIvideo(img_5, OUTPUT_STREAM);
}
