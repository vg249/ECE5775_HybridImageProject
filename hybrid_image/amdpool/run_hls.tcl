# Project settings
open_project hybrid_image_proj -reset
set CFLAGS "-L/usr/X11R6/lib -lm -lpthread -lX11"

# Add the file for synthsis
add_files hybrid_image.cpp

# Add testbench files for co-simulation
add_files -tb hybrid_image_test.cpp
add_files -tb data/marilyn1.png 
add_files -tb data/einstein.png

# Set top module of the design
set_top dut

# Solution settings
open_solution -reset solution1

# Define technology 
set_part  {xc7k160tfbg484-1}

# Set the target clock period
create_clock -period 10.0

# Run C simulation
# Put your linker options for CImg here
#csim_design -ldflags ""
csim_design -ldflags "-lm -lpthread -L/ -L/usr/X11R6/lib -lX11"

# Set any optimization directives
config_dataflow -default_channel fifo 
# End of directives

# Set to 1: to run setup and synthesis
# Set to 2: to run setup, synthesis and RTL simulation
# Set to 3: to run setup, synthesis, RTL simulation and RTL synthesis
# Any other value will run setup only
set hls_exec 1

if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design -rtl verilog
} elseif {$hls_exec == 3} { 
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design -rtl verilog

	export_design 
} else {
	# Default is to exit after setup
	csynth_design
}
exit

