//      // verilator_coverage annotation
        `include "defines.vh"
        
        module memory_unit (
 007924     input clk,
 000290     input reset,
 003772     input ce,
 002176     input [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction,
 001084     inout [`WORD_SIZE-1:0] bus,
 000117     output [`WORD_SIZE-1:0] instruction_code
        );  
 000117     reg [`INSTRUCTION_SIZE-1:0] instruction_register;
%000000     wire [`PC_SIZE-1:0] pc;
 000120     wire [`INSTRUCTION_SIZE-1:0] instruction;
        
            assign instruction_code = instruction_register[`INSTRUCTION_SIZE-1:4];
        
            assign bus = (mem_instruction == `MEM_IVTOBUS) ? instruction_register[`WORD_SIZE-1:0] : 'Z;
        
            data_mem data_mem_0 (
                .clk(clk),
                .reset(reset),
                .instruction_value(instruction_register[`INSTRUCTION_SIZE-5:0]),
                .mem_instruction(mem_instruction),
                .bus(bus)
            );
        
            instruction_mem instruction_mem_0 (
                .mem_instruction(mem_instruction),
                .pc(pc),
                .instruction(instruction)
            );
        
            program_counter program_counter_0 (
                .clk(clk),
                .reset(reset),
                .ce(ce),
                .instruction_value(instruction_register[`INSTRUCTION_SIZE-5:0]),
                .mem_instruction(mem_instruction),
                .bus(bus),
                .pc(pc)
            );
        
 003962     always_ff @(posedge clk) begin
 000145         if (!reset) begin
 001908             if (mem_instruction == `MEM_LDINSTRC) begin
 001909                 instruction_register <= instruction;
                    end
 000145         end else begin
 000145             instruction_register <= 'b0;
                end
            end
        endmodule
        
