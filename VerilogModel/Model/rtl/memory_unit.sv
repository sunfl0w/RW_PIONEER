// The memory unit contains the instruction ROM, RAM and the program counter modules.
// This modules operations are guided by the mem micro instructions.

`include "defines.vh"

module memory_unit (
    input clk,
    input reset,
    input ce,
    input halt_pc,
    input [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction,
    input [`WORD_SIZE-1:0] bus_in,
    output logic [`WORD_SIZE-1:0] bus_out,
`ifdef NEXYSA7
    output [`WORD_SIZE-1:0] instruction_value,
    output [`PC_SIZE-1:0] pc_out,
`endif
    output [`WORD_SIZE-1:0] instruction_code
);  
    wire [`PC_SIZE-1:0] pc;
    wire [`INSTRUCTION_SIZE-1:0] instruction;
    wire [`WORD_SIZE-1:0] bus_out_dm;

    assign instruction_code = instruction[`INSTRUCTION_SIZE-1:4];

`ifdef NEXYSA7
    assign instruction_value = instruction[`WORD_SIZE-1:0];
    assign pc_out = pc;
`endif

    // Select bus out source according to mem instruction
    always_comb begin
        case(mem_instruction)
            `MEM_IVTOBUS: bus_out = instruction[`WORD_SIZE-1:0];
            `MEM_RAMTOBUS : bus_out = bus_out_dm;
            default: bus_out = '0;
        endcase
    end

    data_mem data_mem_0 (
        .clk(clk),
        .reset(reset),
        .instruction_value(instruction[`INSTRUCTION_SIZE-5:0]),
        .mem_instruction(mem_instruction),
        .bus_in(bus_in),
        .bus_out(bus_out_dm)
    );

    instruction_mem instruction_mem_0 (
        .clk(clk),
        .mem_instruction(mem_instruction),
        .pc(pc),
        .instruction(instruction)
    );

    program_counter program_counter_0 (
        .clk(clk),
        .reset(reset),
        .ce(ce),
        .halt(halt_pc),
        .instruction_value(instruction[`INSTRUCTION_SIZE-5:0]),
        .mem_instruction(mem_instruction),
        .bus_in(bus_in),
        .pc(pc)
    );
endmodule
