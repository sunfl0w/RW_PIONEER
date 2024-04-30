#include <stdbool.h>
#include <stdio.h>

enum instructions {
    LDI = 0,
    LDA,
    STOA,
    STOB,
    ADD,
    ADDI,
    ADDC,
    SUB,
    SUBI,
    SUBC,
    JMP,
    LJMP,
    BNEZ,
    BEZ,
    HALT,
    NOOP
};

int main(int argc, char* argv[]) {
    unsigned char program[256];
    for (unsigned int i = 0; i < 256; i++) {
        program[i] = 0b11110000;
    }

    //Fn = 3 2 1 0
    //Fn+1 = 7 6 5 4
    //Temp = 9 8
    //Count = 11 10
    //Out = 12 13 14 15

    // Fibonacci

    // Init memory
    program[0] = LDI << 4 | 0; //LDI 0
    program[1] = STOA << 4 | 0; //STOA 0
    program[2] = STOA << 4 | 1; //STOA 1
    program[3] = STOA << 4 | 2; //STOA 2
    program[4] = STOA << 4 | 3; //STOA 3
    program[5] = STOA << 4 | 5; //STOA 5
    program[6] = STOA << 4 | 6; //STOA 6
    program[7] = STOA << 4 | 7; //STOA 7
    program[8] = STOA << 4 | 8; //STOA 8
    program[9] = STOA << 4 | 9; //STOA 9
    program[10] = STOA << 4 | 10; //STOA 10
    program[11] = STOA << 4 | 11; //STOA 11
    program[12] = STOA << 4 | 12; //STOA 12
    program[13] = STOA << 4 | 13; //STOA 13
    program[14] = STOA << 4 | 14; //STOA 14
    program[15] = STOA << 4 | 15; //STOA 15
    program[16] = LDI << 4 | 1; //LDI 1
    program[17] = STOA << 4 | 4; //STOA 4

    // Increase counter
    program[18] = LDA << 4 | 10; //LDA 10
    program[19] = ADDI << 4 | 1; //ADDI 1
    program[20] = STOA << 4 | 10; //STOA 10
    program[21] = LDI << 4 | 0; //LDI 0
    program[22] = ADDC << 4 | 11; //ADDC 11
    program[23] = STOA << 4 | 11; //STOA 11

    // Compute next fib number
    program[24] = LDA << 4 | 0; //LDA 0
    program[25] = ADD << 4 | 4; //ADD 4
    program[26] = STOA << 4 | 0; //STOA 0
    program[27] = LDA << 4 | 1; //LDA 1
    program[28] = ADDC << 4 | 5; //ADDC 5
    program[29] = STOA << 4 | 1; //STOA 1
    program[30] = LDA << 4 | 2; //LDA 2
    program[31] = ADDC << 4 | 6; //ADDC 6
    program[32] = STOA << 4 | 2; //STOA 2
    program[33] = LDA << 4 | 3; //LDA 3
    program[34] = ADDC << 4 | 7; //ADDC 7
    program[35] = STOA << 4 | 3; //STOA 3

    // Swap numbers
    program[36] = LDA << 4 | 0; //LDA 0
    program[37] = STOA << 4 | 8; //STOA 8
    program[38] = LDA << 4 | 4; //LDA 4
    program[39] = STOA << 4 | 0; //STOA 0
    program[40] = LDA << 4 | 8; //LDA 8
    program[41] = STOA << 4 | 4; //STOA 4

    program[42] = LDA << 4 | 1; //LDA 1
    program[43] = STOA << 4 | 8; //STOA 8
    program[44] = LDA << 4 | 5; //LDA 5
    program[45] = STOA << 4 | 1; //STOA 1
    program[46] = LDA << 4 | 8; //LDA 8
    program[47] = STOA << 4 | 5; //STOA 5

    program[48] = LDA << 4 | 2; //LDA 2
    program[49] = STOA << 4 | 8; //STOA 8
    program[50] = LDA << 4 | 6; //LDA 6
    program[51] = STOA << 4 | 2; //STOA 2
    program[52] = LDA << 4 | 8; //LDA 8
    program[53] = STOA << 4 | 6; //STOA 6

    program[54] = LDA << 4 | 3; //LDA 3
    program[55] = STOA << 4 | 8; //STOA 8
    program[56] = LDA << 4 | 7; //LDA 7
    program[57] = STOA << 4 | 3; //STOA 3
    program[58] = LDA << 4 | 8; //LDA 8
    program[59] = STOA << 4 | 7; //STOA 7

    // Output current last fib number to hex display
    program[60] = LDA << 4 | 4; //LDA 4
    program[61] = STOA << 4 | 15; //STOA 15
    program[62] = LDA << 4 | 5; //LDA 5
    program[63] = STOA << 4 | 14; //STOA 14
    program[64] = LDA << 4 | 6; //LDA 6
    program[65] = STOA << 4 | 13; //STOA 13
    program[66] = LDA << 4 | 7; //LDA 7
    program[67] = STOA << 4 | 12; //STOA 12

    // Check counter and jump back if necessary
    program[68] = LDI << 4 | 7; //LDI 7
    program[69] = SUB << 4 | 10; //SUB 10
    program[70] = LDI << 4 | 1; //LDI 1
    program[71] = BNEZ << 4 | 2; //BNEZ 2
    program[72] = LDI << 4 | 1; //LDI 1
    program[73] = SUBC << 4 | 11; //SUBC 11
    program[74] = LDI << 4 | 1; //LDI 1
    program[75] = BNEZ << 4 | 2; //BNEZ 2

    // End of program
    program[76] = HALT << 4 | 15; //HLT 15

    // Merry christmas
    // Command = 14
    // Data = 15

    // Function set
    /*program[0] = LDI << 4 | 3; //LDI 3
    program[1] = STOA << 4 | 14; //STOA 14
    program[2] = LDI << 4 | 3; //LDI 3
    program[3] = STOA << 4 | 14; //STOA 14
    program[4] = LDI << 4 | 3; //LDI 3
    program[5] = STOA << 4 | 14; //STOA 14
    program[6] = LDI << 4 | 2; //LDI 2
    program[7] = STOA << 4 | 14; //STOA 14
    program[8] = LDI << 4 | 2; //LDI 2
    program[9] = STOA << 4 | 14; //STOA 14
    program[10] = LDI << 4 | 8; //LDI 8
    program[11] = STOA << 4 | 14; //STOA 14

    // Clear
    program[12] = LDI << 4 | 0; //LDI 0
    program[13] = STOA << 4 | 14; //STOA 14
    program[14] = LDI << 4 | 1; //LDI 1
    program[15] = STOA << 4 | 14; //STOA 14

    // Display and cursor on
    program[16] = LDI << 4 | 0; //LDI 0
    program[17] = STOA << 4 | 14; //STOA 14
    program[18] = LDI << 4 | 15; //LDI 15
    program[19] = STOA << 4 | 14; //STOA 14

    // Set entry mode
    program[20] = LDI << 4 | 0; //LDI 0
    program[21] = STOA << 4 | 14; //STOA 14
    program[22] = LDI << 4 | 6; //LDI 6
    program[23] = STOA << 4 | 14; //STOA 14

    // Send character data
    // F
    program[24] = LDI << 4 | 4; //LDI 4
    program[25] = STOA << 4 | 15; //STOA 15
    program[26] = LDI << 4 | 6; //LDI 6
    program[27] = STOA << 4 | 15; //STOA 15

    // r
    program[28] = LDI << 4 | 7; //LDI 7
    program[29] = STOA << 4 | 15; //STOA 15
    program[30] = LDI << 4 | 2; //LDI 2
    program[31] = STOA << 4 | 15; //STOA 15

    // o
    program[32] = LDI << 4 | 6; //LDI 6
    program[33] = STOA << 4 | 15; //STOA 15
    program[34] = LDI << 4 | 15; //LDI 15
    program[35] = STOA << 4 | 15; //STOA 15

    // h
    program[36] = LDI << 4 | 6; //LDI 6
    program[37] = STOA << 4 | 15; //STOA 15
    program[38] = LDI << 4 | 8; //LDI 8
    program[39] = STOA << 4 | 15; //STOA 15

    // e
    program[40] = LDI << 4 | 6; //LDI 6
    program[41] = STOA << 4 | 15; //STOA 15
    program[42] = LDI << 4 | 5; //LDI 5
    program[43] = STOA << 4 | 15; //STOA 15

    // Change row
    program[44] = LDI << 4 | 12; //LDI 12
    program[45] = STOA << 4 | 14; //STOA 14
    program[46] = LDI << 4 | 0; //LDI 0
    program[47] = STOA << 4 | 14; //STOA 14

    // Send character data
    // W
    program[48] = LDI << 4 | 5; //LDI 5
    program[49] = STOA << 4 | 15; //STOA 15
    program[50] = LDI << 4 | 7; //LDI 7
    program[51] = STOA << 4 | 15; //STOA 15

    // e
    program[52] = LDI << 4 | 6; //LDI 6
    program[53] = STOA << 4 | 15; //STOA 15
    program[54] = LDI << 4 | 5; //LDI 5
    program[55] = STOA << 4 | 15; //STOA 15

    // i
    program[56] = LDI << 4 | 6; //LDI 6
    program[57] = STOA << 4 | 15; //STOA 15
    program[58] = LDI << 4 | 9; //LDI 9
    program[59] = STOA << 4 | 15; //STOA 15

    // h
    program[60] = LDI << 4 | 6; //LDI 6
    program[61] = STOA << 4 | 15; //STOA 15
    program[62] = LDI << 4 | 8; //LDI 8
    program[63] = STOA << 4 | 15; //STOA 15

    // n
    program[64] = LDI << 4 | 6; //LDI 6
    program[65] = STOA << 4 | 15; //STOA 15
    program[66] = LDI << 4 | 14; //LDI 14
    program[67] = STOA << 4 | 15; //STOA 15

    // a
    program[68] = LDI << 4 | 6; //LDI 6
    program[69] = STOA << 4 | 15; //STOA 15
    program[70] = LDI << 4 | 1; //LDI 1
    program[71] = STOA << 4 | 15; //STOA 15

    // c
    program[72] = LDI << 4 | 6; //LDI 6
    program[73] = STOA << 4 | 15; //STOA 15
    program[74] = LDI << 4 | 3; //LDI 3
    program[75] = STOA << 4 | 15; //STOA 15

    // h
    program[76] = LDI << 4 | 6; //LDI 6
    program[77] = STOA << 4 | 15; //STOA 15
    program[78] = LDI << 4 | 8; //LDI 8
    program[79] = STOA << 4 | 15; //STOA 15

    // t
    program[80] = LDI << 4 | 7; //LDI 7
    program[81] = STOA << 4 | 15; //STOA 15
    program[82] = LDI << 4 | 4; //LDI 4
    program[83] = STOA << 4 | 15; //STOA 15

    // e
    program[84] = LDI << 4 | 6; //LDI 6
    program[85] = STOA << 4 | 15; //STOA 15
    program[86] = LDI << 4 | 5; //LDI 5
    program[87] = STOA << 4 | 15; //STOA 15

    // n
    program[88] = LDI << 4 | 6; //LDI 6
    program[89] = STOA << 4 | 15; //STOA 15
    program[90] = LDI << 4 | 14; //LDI 14
    program[91] = STOA << 4 | 15; //STOA 15

    // !
    program[92] = LDI << 4 | 2; //LDI 2
    program[93] = STOA << 4 | 15; //STOA 15
    program[94] = LDI << 4 | 1; //LDI 1
    program[95] = STOA << 4 | 15; //STOA 15

    // End of program
    program[96] = HALT << 4 | 15; //HLT 15
    */

    unsigned char ram[16] = {0};
    unsigned char acc = 0;
    unsigned char pc = 0;
    bool cf = false;
    bool zf = false;
    bool halt = false;

    while (!halt) {
        unsigned char operation = program[pc] >> 4;
        unsigned char operand = program[pc] & 0b1111;
        unsigned char sum = 0;

        switch(operation) {
            case LDA:
                acc = ram[operand];
                pc++;
                break;
            case LDI:
                acc = operand;
                pc++;
                break;
            case STOA:
                ram[operand] = acc;
                pc++;
                break;
            case STOB:
                pc++;
                break;
            case ADD:
                sum = acc + ram[operand];
                cf = (sum & 0b00010000) >> 4 == 1;
                zf = sum == 0;
                acc = sum & 0b00001111;
                pc++;
                break;
            case ADDI:
                sum = acc + operand;
                cf = (sum & 0b00010000) >> 4 == 1;
                zf = sum == 0;
                acc = sum & 0b00001111;
                pc++;
                break;
            case ADDC:
                sum = acc + ram[operand];
                if(cf) {
                    sum += 1;
                }
                cf = (sum & 0b00010000) >> 4 == 1;
                zf = sum == 0;
                acc = sum & 0b00001111;
                pc++;
                break;
            case SUB:
                sum = acc - ram[operand];
                cf = (sum & 0b00010000) >> 4 == 1;
                zf = sum == 0;
                acc = sum & 0b00001111;
                pc++;
                break;
            case SUBI:
                sum = acc - operand;
                cf = (sum & 0b00010000) >> 4 == 1;
                zf = sum == 0;
                acc = sum & 0b00001111;
                pc++;
                break;
            case SUBC:
                sum = acc - ram[operand];
                if(cf) {
                    sum -= 1;
                }
                cf = (sum & 0b00010000) >> 4 == 1;
                zf = sum == 0;
                acc = sum & 0b00001111;
                pc++;
                break;
            case JMP:
                pc &= 0b11110000;
                pc += operand;
                break;
            case LJMP:
                pc = acc << 4;
                pc += operand;
                break;
            case BNEZ:
                if(!zf) {
                    pc = acc << 4;
                    pc += operand;
                } else {
                    pc++;
                }
                break;
            case BEZ:
                if(zf) {
                    pc = acc << 4;
                    pc += operand;
                } else {
                    pc++;
                }
                break;
            case HALT:
                halt = true;
                break;
            default:
                pc++;
                break;
            
        }
    }

    printf("HEX-Module output\n");
    int output = (int)ram[12] << 12 | (int)ram[13] << 8 | (int)ram[14] << 4 | (int)ram[15];
    printf("%X\n", output);

    //FILE* file = fopen("PROGRAM.bin", "wb");
    //fwrite(program, sizeof(program), 1, file);
    //fclose(file);
}
