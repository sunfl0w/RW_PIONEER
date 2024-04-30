The 'Model' directory contains the Verilog model of the RW-Pioneer.
'RW_PIONEER_Vivado' contains a Vivado project file for the NEXYS A7 board to run the model on FPGA hardware.
'Programs' contains a memory file with a program to run when implementing the design on the NEXYS A7.

Run make test in the 'Model' directory to run a full the of the RW-Pioneer model using Verilator.
Note that google test must be installed to run the tests.

NOTICE: When running the test comment out the line that says '`define NEXYSA7' in 'Model/rtl/defines.vh'. When synthesizing the model for the NEXYSA7 uncomment the line.