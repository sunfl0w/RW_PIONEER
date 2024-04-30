// Top module for running tests in using Verilator.

`include "defines.vh"

module system (
    input clk,
    input reset
);

`ifdef VERILATOR
    export "DPI-C" function getInstructionCode;
    export "DPI-C" function getALUInstruction;
    export "DPI-C" function getMemInstruction;
    export "DPI-C" function getBUS;

    function byte getInstructionCode();
        return {4'b0, instruction_code};
    endfunction

    function byte getALUInstruction();
        return {5'b0, alu_instruction};
    endfunction

    function byte getMemInstruction();
        return {5'b0, mem_instruction};
    endfunction

    function byte getBUS();
        return {4'b0, bus};
    endfunction
`endif

    wire [`WORD_SIZE-1:0] bus_out_alu;
    wire [`WORD_SIZE-1:0] bus_out_mu;
    wire zero_flag;
    wire ce;
    wire halt_pc;
    wire [`WORD_SIZE-1:0] bus;
    wire [`WORD_SIZE-1:0] instruction_code;
    wire [`ALU_MICRO_INSTRUCTION_SIZE-1:0] alu_instruction;
    wire [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction;

    assign bus = bus_out_alu | bus_out_mu;

    memory_unit memory_unit_0 (
        .clk(clk),
        .reset(reset),
        .ce(ce),
        .halt_pc(halt_pc),
        .mem_instruction(mem_instruction),
        .bus_in(bus),
        .bus_out(bus_out_mu),
        .instruction_code(instruction_code)
    );

    control control_0 (
        .clk(clk),
        .reset(reset),
        .zero_flag(zero_flag),
        .instruction_code(instruction_code),
        .ce(ce),
        .halt_pc(halt_pc),
        .alu_instruction(alu_instruction),
        .mem_instruction(mem_instruction)
    );

    alu alu_0 (
        .clk(clk),
        .reset(reset),
        .alu_instruction(alu_instruction),
        .bus_in(bus),
        .bus_out(bus_out_alu),
        .zero_flag(zero_flag)
    );
endmodule
