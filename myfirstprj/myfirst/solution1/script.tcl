############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2014 Xilinx Inc. All rights reserved.
############################################################
open_project myfirst
add_files ../opencv.cpp
add_files -tb ../opencv_test.cpp
open_solution "solution1"
set_part {xc7k70tfbg484-1}
create_clock -period 10 -name default
source "./myfirst/solution1/directives.tcl"
csim_design -setup
csynth_design
cosim_design
export_design -format ip_catalog
