############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2014 Xilinx Inc. All rights reserved.
############################################################
open_project HybridImage
set_top clsHybridImage
add_files HybridImage/.apc/hybridImage.cpp
add_files HybridImage/.apc/.src/hybridImage.h
add_files -tb ../hybridImage_test.cpp
add_files -tb HybridImage/zhang-jpg.pgm
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
source "./HybridImage/solution1/directives.tcl"
csim_design -clean -setup
csynth_design
cosim_design
export_design -format ip_catalog
