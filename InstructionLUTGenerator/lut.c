#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

enum INSTRUCTION {
    LDI,
    LDA,
    STOA,
    STOB,
    ADD,
    ADDI,
    ADDC,
    SUB,
    SUBI,
    SUBC,
    SJMP,
    JMP,
    BNEZ,
    BEZ,
    HLT,
    NOOP
};

enum ALU_INSTRUCTION {
    ALU_ADD,
    ALU_ADDC,
    ALU_SUB,
    ALU_SUBC,
    ALU_BUSTOACC,
    ALU_ACCTOBUS,
    ALU_NOOP
};

enum MEM_INSTRUCTION {
    MEM_SJMP,
    MEM_JMP,
    MEM_BUSTORAM,
    MEM_RAMTOBUS,
    MEM_IVTOBUS,
    MEM_LDINSTRC,
    MEM_NOOP
};

int main(int argc, char* argv[]) {
    uint8_t lut[64];
    for(uint32_t instruction = 0; instruction < 16; instruction++) {
        for(uint32_t zf = 0; zf < 2; zf++) {
            for(uint32_t step = 0; step < 2; step++) {
                uint8_t alu_instruction = 0;
                uint8_t mem_instruction = 0;
                bool ce = false;
                bool hlt = false;

                if(step == 0) {
                    alu_instruction = ALU_NOOP;
                    mem_instruction = MEM_LDINSTRC;
                } else {
                    switch(instruction) {
                        case LDI:
                            alu_instruction = ALU_BUSTOACC;
                            mem_instruction = MEM_IVTOBUS;
                            ce = true;
                            break;
                        case LDA:
                            alu_instruction = ALU_BUSTOACC;
                            mem_instruction = MEM_RAMTOBUS;
                            ce = true;
                            break;
                        case STOA:
                            alu_instruction = ALU_ACCTOBUS;
                            mem_instruction = MEM_BUSTORAM;
                            ce = true;
                            break;
                        case STOB:
                            alu_instruction = ALU_NOOP;
                            mem_instruction = MEM_BUSTORAM;
                            ce = true;
                            break;
                        case ADD:
                            alu_instruction = ALU_ADD;
                            mem_instruction = MEM_RAMTOBUS;
                            ce = true;
                            break;
                        case ADDI:
                            alu_instruction = ALU_ADD;
                            mem_instruction = MEM_IVTOBUS;
                            ce = true;
                            break;
                        case ADDC:
                            alu_instruction = ALU_ADDC;
                            mem_instruction = MEM_RAMTOBUS;
                            ce = true;
                            break;
                        case SUB:
                            alu_instruction = ALU_SUB;
                            mem_instruction = MEM_RAMTOBUS;
                            ce = true;
                            break;
                        case SUBI:
                            alu_instruction = ALU_SUB;
                            mem_instruction = MEM_IVTOBUS;
                            ce = true;
                            break;
                        case SUBC:
                            alu_instruction = ALU_SUBC;
                            mem_instruction = MEM_RAMTOBUS;
                            ce = true;
                            break;
                        case SJMP:
                            alu_instruction = ALU_NOOP;
                            mem_instruction = MEM_SJMP;
                            break;
                        case JMP:
                            alu_instruction = ALU_ACCTOBUS;
                            mem_instruction = MEM_JMP;
                            break;
                        case BNEZ:
                            if(!zf) {
                                alu_instruction = ALU_ACCTOBUS;
                                mem_instruction = MEM_JMP;
                            } else {
                                mem_instruction = ALU_NOOP;
                                alu_instruction = MEM_NOOP;
                                ce = true;
                            }
                            break;
                        case BEZ:
                            if(zf) {
                                alu_instruction = ALU_ACCTOBUS;
                                mem_instruction = MEM_JMP;
                            } else {
                                mem_instruction = ALU_NOOP;
                                alu_instruction = MEM_NOOP;
                                ce = true;
                            }
                            break;
                        case HLT:
                            alu_instruction = ALU_NOOP;
                            mem_instruction = MEM_NOOP;
                            hlt = true;
                            break;
                        case NOOP:
                            alu_instruction = ALU_NOOP;
                            mem_instruction = MEM_NOOP;
                            ce = true;
                            break;
                    }
                }

                uint8_t lut_entry = alu_instruction | mem_instruction << 3 | ce << 6 | hlt << 7;
                lut[instruction << 2 | zf << 1 | step] = lut_entry;
            }
        }
    }

    FILE* file = fopen("LUT.bin","wb");
    fwrite(lut, sizeof(lut), 1, file); 
    fclose(file);

    return 0;
}
