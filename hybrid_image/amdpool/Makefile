#==========================================================================
# Makefile
#==========================================================================
# @brief: A makefile the compiles and synthesizes the hybrid image program
#
# @desc: 1. "make" runs csim by default
#        2. "make csim" compiles & executes the implementation
#        3. "make clean" cleans up the directory


# Extract Vivado HLS include path
VHLS_PATH := $(dir $(shell which vivado_hls))/..
VHLS_INC ?= ${VHLS_PATH}/include

CFLAGS = -g -I${VHLS_INC} 

all: csim

csim: hybrid_image.cpp hybrid_image_test.cpp
	@echo "Compiling & simulating Hybrid image on amdpool ..."
	g++ ${CFLAGS} $^ -o hybrid_image
	./hybrid_image

bitstream: 
	@echo "Synthesizing Hynrid imaging and creating bitstream on amdpool ..."
	vivado_hls -f run_hls.tcl
	./run_bitstream.sh

clean:
	rm -rf hybrid_image *.dat *.prj *.log
	rm -rf zedboard_project* xillydemo.bit
