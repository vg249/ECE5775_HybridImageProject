#include <hls_stream.h>

#include <ap_axi_sdata.h>

typedef ap_axiu<32,1,1,1> ap_32_bit_channel;

typedef hls::stream<ap_32_bit_channel> AXI_STREAM;

void image_filter(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int
cols);
