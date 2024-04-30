// Important defines for the whole model

// SIMULATION / FPGA switch
// `define NEXYSA7 // Only define if synthesizing for the NEXYS A7

// GLOBAL
`define WORD_SIZE 4
`define PC_SIZE 8
`define MEM_ADDRESSES 16
`define INSTRUCTION_SIZE 8
`define INSTRUCTION_MEMORY_SIZE 256
`define ALU_MICRO_INSTRUCTION_SIZE 3
`define MEM_MICRO_INSTRUCTION_SIZE 3

// ALU MICRO INSTRUCTIONS
`define ALU_ADD      3'h0
`define ALU_ADDC     3'h1
`define ALU_SUB      3'h2
`define ALU_SUBC     3'h3
`define ALU_BUSTOACC 3'h4
`define ALU_ACCTOBUS 3'h5
`define ALU_NOOP     3'h6

// MEM MICRO INSTRUCTIONS
`define MEM_SJMP     3'h0
`define MEM_JMP      3'h1
`define MEM_BUSTORAM 3'h2
`define MEM_RAMTOBUS 3'h3
`define MEM_IVTOBUS  3'h4
`define MEM_LDINSTRC 3'h5
`define MEM_NOOP     3'h6

// INSTRUCTIONS
`define LDI  4'h0
`define LDA  4'h1
`define STOA 4'h2
`define STOB 4'h3
`define ADD  4'h4
`define ADDI 4'h5
`define ADDC 4'h6
`define SUB  4'h7
`define SUBI 4'h8
`define SUBC 4'h9
`define SJMP 4'hA
`define JMP  4'hB
`define BNEZ 4'hC
`define BEZ  4'hD
`define HALT 4'hE
`define NOOP 4'hF
