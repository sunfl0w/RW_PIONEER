// RAM module for the RW-Pioneer.

`include "defines.vh"

module data_mem (
    input clk,
    input reset,
    input [`WORD_SIZE-1:0] instruction_value,
    input [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction,
    input [`WORD_SIZE-1:0] bus_in,
    output reg [`WORD_SIZE-1:0] bus_out
);

`ifdef VERILATOR
    import "DPI-C" function byte dataMemoryRead(input byte address);
    import "DPI-C" function void dataMemoryWrite(
        input byte address,
        input byte data
    );
`else
    reg [`WORD_SIZE-1:0] memory[0:`MEM_ADDRESSES-1];
`endif

    always_ff @(posedge clk) begin
        if (!reset) begin
            if (mem_instruction == `MEM_BUSTORAM) begin
`ifdef VERILATOR
                dataMemoryWrite({4'b0, instruction_value}, {4'b0, bus_in});
`else
                memory[instruction_value] <= bus_in;
`endif
            end else if (mem_instruction == `MEM_RAMTOBUS) begin
`ifdef VERILATOR
                bus_out <= `WORD_SIZE'(dataMemoryRead({4'b0, instruction_value}));
`else
                bus_out <= `WORD_SIZE'(memory[instruction_value]);
`endif
            end
        end
    end
endmodule
