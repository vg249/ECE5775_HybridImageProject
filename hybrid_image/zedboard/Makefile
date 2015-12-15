#==========================================================================
# Makefile
#==========================================================================
# @brief: A makefile the compiles and runs the hybrid image program
#
# @desc: 1. "make" or "make sw" runs software execution by default
#        2. "make fpga" invokes the HW accelerator
#        3. "make clean" cleans up the directory


INC_PATH=/usr/include/vivado_hls/2015.2
CFLAGS = -I${INC_PATH} -O2 -L/usr/X11R6/lib -lm -lpthread -lX11

all: sw

sw: hybrid_image.cpp hybrid_image_test.cpp
	@echo "Compiling & executing hybrid image software program on ARM ..."
	g++ ${CFLAGS} hybrid_image.cpp hybrid_image_test.cpp -o hybrid_image-arm
	./hybrid_image-arm

fpga: host.cpp
	@echo "Compiling host program & running hybrid_image accelerator ..."
	g++ ${CFLAGS} host.cpp -o hybrid_image-fpga
	./hybrid_image-fpga

# Clean up the directory before submission
clean:
	@echo "Clean up output files"
	rm -rf hybrid_image-sw vivado_hls.log *.prj result out.dat *~
	rm -rf hybrid_image-fpga
