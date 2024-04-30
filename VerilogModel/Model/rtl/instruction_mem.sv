// Module representing the instruction ROM.

`include "defines.vh"

module instruction_mem (
    input clk,
    input [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction,
    input [`PC_SIZE-1:0] pc,
    output reg [`INSTRUCTION_SIZE-1:0] instruction
);
`ifdef VERILATOR
    import "DPI-C" function byte instructionMemoryRead(input byte address);

    always_ff @(posedge clk) begin
        if (mem_instruction == `MEM_LDINSTRC) begin
            instruction <= instructionMemoryRead(pc);
        end else begin
            instruction <= instruction;
        end
    end

`else
    reg [`INSTRUCTION_SIZE-1:0] memory[0:`INSTRUCTION_MEMORY_SIZE-1];

    always_ff @(posedge clk) begin
        if (mem_instruction == `MEM_LDINSTRC) begin
            instruction <= memory[pc];
        end else begin
            instruction <= instruction;
        end
    end

`ifdef NEXYSA7
    initial begin
        $readmemh("FIB.mem", memory);
    end
`endif
`endif
endmodule
