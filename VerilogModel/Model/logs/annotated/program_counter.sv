//      // verilator_coverage annotation
        `include "defines.vh"
        
        module program_counter (
 007924     input clk,
 000290     input reset,
 003772     input ce,
 000895     input [`WORD_SIZE-1:0] instruction_value,
 002176     input [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction,
 001084     inout [`WORD_SIZE-1:0] bus,
%000000     output [`PC_SIZE-1:0] pc
        );
            export "DPI-C" function get_pc;
        
%000000     function byte get_pc();
%000000         return pc_reg;
            endfunction
        
%000000     reg [`PC_SIZE-1:0] pc_reg;
        
            assign pc = pc_reg;
        
 003962     always_ff @(posedge clk) begin
 000145         if (!reset) begin
 001886             if (ce) begin
 001886                 pc_reg <= pc_reg + 1;
                    end 
        
%000000             if (mem_instruction == `MEM_SJMP) begin
%000000                 pc_reg[`PC_SIZE-5:0] <= instruction_value;
 000022             end else if (mem_instruction == `MEM_JMP) begin
 000022                 pc_reg[`PC_SIZE-5:0] <= instruction_value;
 000022                 pc_reg[`PC_SIZE-1:4] <= bus;
                    end
 000145         end else begin
 000145             pc_reg <= 'b0;
                end
            end
        endmodule
        
