//      // verilator_coverage annotation
        `include "defines.vh"
        
        module system (
 007924     input clk,
 000290     input reset
        );
            export "DPI-C" function get_instruction_code;
            export "DPI-C" function get_alu_instruction;
            export "DPI-C" function get_mem_instruction;
            export "DPI-C" function getBUS;
        
 002537     function byte get_instruction_code();
 002537         return {4'b0, instruction_code};
            endfunction
        
%000000     function byte get_alu_instruction();
%000000         return {5'b0, alu_instruction};
            endfunction
        
%000000     function byte get_mem_instruction();
%000000         return {5'b0, mem_instruction};
            endfunction
        
%000000     function byte getBUS();
%000000         return {4'b0, bus};
            endfunction
        
        
        
 000085     wire zero_flag;
 003772     wire ce;
 001084     wire [`WORD_SIZE-1:0] bus;
 000117     wire [`WORD_SIZE-1:0] instruction_code;
 000583     wire [`ALU_MICRO_INSTRUCTION_SIZE-1:0] alu_instruction;
 002176     wire [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction;
        
            memory_unit memory_unit_0 (
                .clk(clk),
                .reset(reset),
                .ce(ce),
                .mem_instruction(mem_instruction),
                .bus(bus),
                .instruction_code(instruction_code)
            );
        
            control control_0 (
                .clk(clk),
                .reset(reset),
                .zero_flag(zero_flag),
                .instruction_code(instruction_code),
                .ce(ce),
                .alu_instruction(alu_instruction),
                .mem_instruction(mem_instruction)
            );
        
            alu alu_0 (
                .clk(clk),
                .reset(reset),
                .alu_instruction(alu_instruction),
                .bus(bus),
                .zero_flag(zero_flag)
            );
        endmodule
        
