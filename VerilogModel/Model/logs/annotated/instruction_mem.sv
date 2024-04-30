//      // verilator_coverage annotation
        `include "defines.vh"
        
        module instruction_mem (
 002176     input [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction,
%000000     input [`PC_SIZE-1:0] pc,
 000120     output [`INSTRUCTION_SIZE-1:0] instruction
        );
        `ifdef VERILATOR  // Code for Verilator tests
            import "DPI-C" function byte instructionMemoryRead(input byte address);
        
            assign instruction = (mem_instruction == `MEM_LDINSTRC) ? instructionMemoryRead(pc) : 'Z;
        `else  // Code for FPGA
            reg [`INSTRUCTION_SIZE-1:0] memory[0:`INSTRUCTION_MEMORY_SIZE-1];
        
            assign instruction = (mem_instruction == `MEM_LDINSTRC) ? memory[pc] : 'Z;
        
            integer i;
            initial begin
                for (i = 0; i < `INSTRUCTION_MEMORY_SIZE; i = i + 1) begin
                    memory[i] = 0;
                end
            end
        `endif
        endmodule
        
