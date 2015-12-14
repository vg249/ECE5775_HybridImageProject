#==========================================================================
# Makefile
#==========================================================================
# @brief: A makefile the compiles and synthesizes the hybrid image program
#
# @desc: 1. "make" runs csim by default
#        2. "make csim" compiles & executes the implementation
#        3. "make clean" cleans up the directory



all: csim

csim: hybrid_image.cpp 
	@echo "Compiling & simulating Hybrid image ..."
	g++  $^ -o hybrid_image.exe -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
	./hybrid_image.exe

clean:
	rm -rf hybrid_image*.exe
	rm -rf original*.png hybrid_image.png
