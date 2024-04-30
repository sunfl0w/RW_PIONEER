# RW-Pioneer

RW-Pioneer is a computer project inspired by Ben Eater's [8-bit breadboard computer project](https://www.youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU) over at YouTube.
I always wanted to build my own crappy computer, and the Pioneer was my first such attempt.
The design is not perfect by any means, but it was the best one I could come up with after a few semesters of studying computer science.

**Note that the RW-Pioneer is a few years old by now and I only compiled old schematics, layouts and code for this repository.**
**I recently added a binary compatible Verilog model of the system with Verilator test suite and Vivado project files for the Nexys A7 board.**

## Content

This repository contains the following schematics and software components:

- RW-Pioneer schematics, paper explaining the architecture and KiCad design files (Schematic and RW_PIONEER_WHITEPAPER.pdf)
- RW-Pioneer Verilog model with Verilator test suite and Vivado project files for the NEXYS A7 board (VerilogModel)
- HEX-Module and 1602-Module schematics and KiCad design files (ExpansionModules)
- Instruction LUT generator for the instruction decoder memory IC of the system (InstructionLUTGenerator)
- Memory-Programmer host and programmer software sources, schematics and KiCad design files (MemoryProgrammer)
- Simple software emulator for the system (RWEMU)

## Licenses

The software and schematics are licensed under the permissive BSD 3-Clause License.

## Links

[Visit my personal website to see the Pioneer in action.](https://www.rechenwerke.com/projects/rw_pioneer)

## Gallery

Image of a prototype board I assembled a few years ago.

![Image](/Images/RWP16A.jpg)

Image of the NEXYS A7 board running the RW-Pioneer Verilog model.

![Image](/Images/PioneerOnNexysA7.jpg)
