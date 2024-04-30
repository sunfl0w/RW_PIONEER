// Module controlling the sub modules of the RW-Pioneer.
// Notice that each instruction requires three clock cycles compared to the two of the base design (the design using integrated circuits, etc.).
// This is because the base design used asynchronous memory operations, whereas the HDL design requires synchronous memory operations.
// Pipeline: (0) Fetch/Decode, (1) ReadMem/ReadIV, (2) Execute/Writeback

`include "defines.vh"

localparam ALU_MICRO_INSTRUCTION_SIZE = 3;
localparam MEM_MICRO_INSTRUCTION_SIZE = 3;

module control (
    input clk,
    input reset,
    input zero_flag,
    input [3:0] instruction_code,
`ifdef NEXYSA7
    output [1:0] step_out,
`endif
    output logic ce,
    output logic halt_pc,
    output logic [ALU_MICRO_INSTRUCTION_SIZE-1:0] alu_instruction,
    output logic [MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction
);
`ifdef VERILATOR
    export "DPI-C" function getStep;

    function byte getStep();
        return {6'b0, step};
    endfunction
`endif

    reg[1:0] step;

`ifdef NEXYSA7
    assign step_out = step;
`endif

    always_comb begin
        // Decode instruction and step to ALU instruction
        case ({step, instruction_code})
            {2'h2,`LDI},{2'h2,`LDA}: alu_instruction = `ALU_BUSTOACC;
            {2'h2,`STOA}: alu_instruction = `ALU_ACCTOBUS;
            {2'h2,`ADD},{2'h2,`ADDI}: alu_instruction = `ALU_ADD;
            {2'h2,`ADDC}: alu_instruction = `ALU_ADDC;
            {2'h2,`SUB},{2'h2,`SUBI}: alu_instruction = `ALU_SUB;
            {2'h2,`SUBC}: alu_instruction = `ALU_SUBC;
            {2'h2,`JMP}: alu_instruction = `ALU_ACCTOBUS;
            {2'h2,`BNEZ}: begin
                if (!zero_flag) begin
                    alu_instruction = `ALU_ACCTOBUS;
                end else begin
                    alu_instruction = `ALU_NOOP;
                end
            end
            {2'h2,`BEZ}: begin
                if (zero_flag) begin
                    alu_instruction = `ALU_ACCTOBUS;
                end else begin
                    alu_instruction = `ALU_NOOP;
                end
            end
            default: alu_instruction = `ALU_NOOP;
        endcase

        // Decode instruction and step to MEM instruction
        case ({step, instruction_code}) inside
            {2'h0,4'b????}: mem_instruction = `MEM_LDINSTRC;
            //{2'h1,`LDI},{2'h1,`ADDI},{2'h1,`SUBI}: mem_instruction = `MEM_IVTOBUS;
            {2'h2,`LDI},{2'h2,`ADDI},{2'h2,`SUBI}: mem_instruction = `MEM_IVTOBUS;
             // Load data from memory to tmp register
            {2'h1,`LDA},{2'h1,`ADD},{2'h1,`ADDC},{2'h1,`SUB},{2'h1,`SUBC}: mem_instruction = `MEM_RAMTOBUS;
            // Put data from register on bus
            {2'h2,`LDA},{2'h2,`ADD},{2'h2,`ADDC},{2'h2,`SUB},{2'h2,`SUBC}: mem_instruction = `MEM_RAMTOBUS;
            {2'h2,`STOA},{2'h2,`STOB}: mem_instruction = `MEM_BUSTORAM;
            {2'h2,`SJMP}: mem_instruction = `MEM_SJMP;
            {2'h2,`JMP}: mem_instruction = `MEM_JMP;
            {2'h2,`BNEZ}: begin
                if (!zero_flag) begin
                    mem_instruction = `MEM_JMP;
                end else begin
                    mem_instruction = `MEM_NOOP;
                end
            end
            {2'h2,`BEZ}: begin
                if (zero_flag) begin
                    mem_instruction = `MEM_JMP;
                end else begin
                    mem_instruction = `MEM_NOOP;
                end
            end
            default: mem_instruction = `MEM_NOOP;
        endcase

        // Decode instruction and step to ce (pc count enable) signal
        if (step == 2'b00 || step == 2'b01) begin
            ce = 1'b0;
            halt_pc = 1'b0;
        end else begin
            if (instruction_code == `BNEZ && !zero_flag ||
                instruction_code == `BEZ && zero_flag || 
                instruction_code == `SJMP || 
                instruction_code == `JMP) 
            begin
                ce = 1'b0;
                halt_pc = 1'b0;
            end else if (instruction_code == `HALT) begin
                ce = 1'b0;
                halt_pc = 1'b1;
            end else begin
                ce = 1'b1;
                halt_pc = 1'b0;
            end
        end
    end

    always_ff @(posedge clk) begin
        if (reset) begin
            step <= 'b0;
        end else if (!halt_pc) begin
            if (step == 2'b10) begin
                step <= 'b0;
            end else begin
                step <= step + 1;
            end
        end
    end
endmodule
