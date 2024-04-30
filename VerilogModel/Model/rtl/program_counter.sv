// Module defining the program counter of the RW-Pioneer.
// Operation is defined by the incoming mem micro-instruction.

`include "defines.vh"

module program_counter (
    input clk,
    input reset,
    input ce,
    input halt,
    input [`WORD_SIZE-1:0] instruction_value,
    input [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction,
    input [`WORD_SIZE-1:0] bus_in,
    output [`PC_SIZE-1:0] pc
);
`ifdef VERILATOR
    export "DPI-C" function getPC;

    function byte getPC();
        return pc_reg;
    endfunction
`endif

    reg [`PC_SIZE-1:0] pc_reg;

    assign pc = pc_reg;

    always_ff @(posedge clk) begin
        if (!reset) begin
            if (ce && !halt) begin
                pc_reg <= pc_reg + 1;
            end

            if (mem_instruction == `MEM_SJMP) begin
                pc_reg[`PC_SIZE-5:0] <= instruction_value;
            end else if (mem_instruction == `MEM_JMP) begin
                pc_reg[`PC_SIZE-5:0] <= instruction_value;
                pc_reg[`PC_SIZE-1:4] <= bus_in;
            end
        end else begin
            pc_reg <= 'b0;
        end
    end
endmodule
