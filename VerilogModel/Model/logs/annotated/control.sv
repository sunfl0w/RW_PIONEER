//      // verilator_coverage annotation
        `include "defines.vh"
        
        localparam ALU_MICRO_INSTRUCTION_SIZE = 3;
        localparam MEM_MICRO_INSTRUCTION_SIZE = 3;
        
        module control (
 007924     input clk,
 000290     input reset,
 000085     input zero_flag,
 000117     input [3:0] instruction_code,
 003772     output ce,
 000583     output [ALU_MICRO_INSTRUCTION_SIZE-1:0] alu_instruction,
 002176     output [MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction
        );
            export "DPI-C" function get_step;
        
%000000     function byte get_step();
%000000         return {7'b0, step};
            endfunction
        
 003817     reg  step;
        
            assign alu_instruction = step ? ((instruction_code == `LDI || instruction_code == `LDA) ? `ALU_BUSTOACC :
                                             (instruction_code == `STOA || (instruction_code == `BNEZ && !zero_flag) || (instruction_code == `BEZ && zero_flag)) ? `ALU_ACCTOBUS :
                                             (instruction_code == `ADD || instruction_code == `ADDI) ? `ALU_ADD :
                                             (instruction_code == `ADDC) ? `ALU_ADDC :
                                             (instruction_code == `SUB || instruction_code == `SUBI) ? `ALU_SUB :
                                             (instruction_code == `SUBC) ? `ALU_SUBC :
                                             `ALU_NOOP) :
                                     `ALU_NOOP;
        
            assign mem_instruction = step ? ((instruction_code == `LDI || instruction_code == `ADDI || instruction_code == `SUBI) ? `MEM_IVTOBUS :
                                             (instruction_code == `LDA || instruction_code == `ADD || instruction_code == `ADDC ||
                                                instruction_code == `SUB || instruction_code == `SUBC) ? `MEM_RAMTOBUS :
                                             (instruction_code == `STOA || instruction_code == `STOB) ? `MEM_BUSTORAM :
                                             (instruction_code == `SJMP) ? `MEM_SJMP :
                                             (instruction_code == `JMP) ? `MEM_JMP :
                                             (instruction_code == `BNEZ && !zero_flag) ? `MEM_JMP :
                                             (instruction_code == `BEZ && zero_flag) ? `MEM_JMP :
                                             `MEM_NOOP) :
                                     `MEM_LDINSTRC;
        
            assign ce = (step == 1'b0 || (step == 1'b1 && ((instruction_code == `BNEZ && !zero_flag) || 
                                                           (instruction_code == `BEZ && zero_flag) || 
                                                           instruction_code == `SJMP ||
                                                           instruction_code == `JMP ||
                                                           instruction_code == `HALT))) ? 1'b0 : 1'b1;
        
 003962     always_ff @(posedge clk) begin
 000145         if (reset) begin
 000145             step <= 'b0;
 003817         end else begin
 003817             step <= step + 1;
                end
            end
        endmodule
        
