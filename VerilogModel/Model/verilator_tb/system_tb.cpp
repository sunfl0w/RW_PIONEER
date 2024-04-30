#include "Vsystem.h"
#include "Vsystem__Dpi.h"
#include "Vsystem___024unit.h"
#include "system_dut.hpp"

#include <array>
#include <fstream>
#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <svdpi.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define MAX_SIM_TIME 5000

enum instructions { LDI = 0, LDA, STOA, STOB, ADD, ADDI, ADDC, SUB, SUBI, SUBC, JMP, LJMP, BNEZ, BEZ, HALT, NOOP };

SystemDUT<Vsystem> dut = SystemDUT<Vsystem>();

// DPI FUNCTIONS

char dataMemoryRead(char address) {
    return dut.getDataMemory()[(uint8_t)address];
}

void dataMemoryWrite(char address, char data) {
    dut.getDataMemory()[(uint8_t)address] = data;
}

char instructionMemoryRead(char address) {
    return dut.getInstructionMemory()[(uint8_t)address];
}

// MISC FUNCTIONS

void readProgramFromFile(uint8_t* instruction_buffer, std::string path) {
    std::ifstream file = std::ifstream(path, std::ios::binary);
    std::streampos beg = file.tellg();
    file.seekg(0, std::ios::end);
    std::streampos end = file.tellg();
    if (end - beg < 256) {
        file.close();
        return;
    }
    file.seekg(0, std::ios::beg);
    file.read((char*)instruction_buffer, 256);
    file.close();
}

// TEST FUNCTIONS

TEST(PIONEER_PROGRAM_TEST, FIB_TEST) {
    bool halt = false;
    vluint64_t sim_time = 0;

    readProgramFromFile(dut.getInstructionMemory(), "verilator_tb/data/fib.bin");
    dut.reset();

    while (sim_time < MAX_SIM_TIME && !halt) {
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system");
        assert(scope);
        svSetScope(scope);

        if (dut.getModule()->getInstructionCode() == 14) {
            halt = true;
        }

        sim_time++;
    }

    ASSERT_LT(sim_time, MAX_SIM_TIME);
    ASSERT_EQ(dut.getInstructionMemory()[12], 0xB);
    ASSERT_EQ(dut.getInstructionMemory()[13], 0x5);
    ASSERT_EQ(dut.getInstructionMemory()[14], 0x2);
    ASSERT_EQ(dut.getInstructionMemory()[15], 0x0);
}

TEST(PIONEER_INSTRUCTION_TEST, LDI_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.reset();

        dut.tick();
        dut.tick();
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t acc = dut.getModule()->getACC();

        ASSERT_EQ(acc, i);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, LDA_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getDataMemory()[i] = i;
        dut.getInstructionMemory()[0] = LDA << 4 | i;
        dut.reset();

        dut.tick();
        dut.tick();
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t acc = dut.getModule()->getACC();

        ASSERT_EQ(acc, i);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, STOA_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = STOA << 4 | i;
        dut.reset();

        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t mem_val = dut.getDataMemory()[i];

        ASSERT_EQ(mem_val, i);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, ADD_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = STOA << 4 | 0x0;
        dut.getInstructionMemory()[2] = LDI << 4 | i;
        dut.getInstructionMemory()[3] = ADD << 4 | 0x0;
        dut.reset();

        for (uint8_t t = 0; t < 12; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t acc = dut.getModule()->getACC();

        ASSERT_EQ(acc, (i + i) & 0xF);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, ADDI_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = STOA << 4 | 0x0;
        dut.getInstructionMemory()[2] = ADDI << 4 | i;
        dut.reset();

        for (uint8_t t = 0; t < 9; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t acc = dut.getModule()->getACC();

        ASSERT_EQ(acc, (i + i) & 0xF);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, ADDC_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = STOA << 4 | 0x0;
        dut.getInstructionMemory()[2] = STOA << 4 | 0x1;
        dut.getInstructionMemory()[3] = STOA << 4 | 0x2;
        dut.getInstructionMemory()[4] = STOA << 4 | 0x3;
        dut.getInstructionMemory()[5] = LDA << 4 | 0x0;
        dut.getInstructionMemory()[6] = ADD << 4 | 0x2;
        dut.getInstructionMemory()[7] = STOA << 4 | 0x4;
        dut.getInstructionMemory()[8] = LDA << 4 | 0x1;
        dut.getInstructionMemory()[9] = ADDC << 4 | 0x3;
        dut.getInstructionMemory()[10] = STOA << 4 | 0x5;
        dut.reset();

        for (uint8_t t = 0; t < 33; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t expected = (i << 4 | i) + (i << 4 | i);

        uint8_t mem_val = dut.getDataMemory()[0x5] << 4 | dut.getDataMemory()[0x4];

        ASSERT_EQ(mem_val, expected);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, SUB_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = STOA << 4 | 0x0;
        dut.getInstructionMemory()[2] = LDI << 4 | i;
        dut.getInstructionMemory()[3] = SUB << 4 | 0x0;
        dut.reset();

        for (uint8_t t = 0; t < 12; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t acc = dut.getModule()->getACC();

        ASSERT_EQ(acc, 0x0);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, SUBI_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = STOA << 4 | 0x0;
        dut.getInstructionMemory()[2] = SUBI << 4 | i;
        dut.reset();

        for (uint8_t t = 0; t < 9; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t acc = dut.getModule()->getACC();

        ASSERT_EQ(acc, 0x0);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, SUBC_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = STOA << 4 | 0x0;
        dut.getInstructionMemory()[2] = STOA << 4 | 0x1;
        dut.getInstructionMemory()[3] = STOA << 4 | 0x2;
        dut.getInstructionMemory()[4] = STOA << 4 | 0x3;
        dut.getInstructionMemory()[5] = LDA << 4 | 0x0;
        dut.getInstructionMemory()[6] = SUB << 4 | 0x2;
        dut.getInstructionMemory()[7] = STOA << 4 | 0x4;
        dut.getInstructionMemory()[8] = LDA << 4 | 0x1;
        dut.getInstructionMemory()[9] = SUBC << 4 | 0x3;
        dut.getInstructionMemory()[10] = STOA << 4 | 0x5;
        dut.reset();

        for (uint8_t t = 0; t < 33; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.alu_0");
        assert(scope);
        svSetScope(scope);

        uint8_t expected = 0x0;

        uint8_t mem_val = dut.getDataMemory()[0x5] << 4 | dut.getDataMemory()[0x4];

        ASSERT_EQ(mem_val, expected);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, JMP_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = JMP << 4 | i;
        dut.reset();

        dut.tick();
        dut.tick();
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ(dut.getModule()->getPC(), i);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, LJMP_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | i;
        dut.getInstructionMemory()[1] = LJMP << 4 | i;
        dut.reset();

        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ((uint8_t)dut.getModule()->getPC(), (i << 4) | i);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, BNEZ_JMP_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | 0x0;
        dut.getInstructionMemory()[1] = ADDI << 4 | 0x1;
        dut.getInstructionMemory()[2] = LDI << 4 | i;
        dut.getInstructionMemory()[3] = BNEZ << 4 | i;
        dut.reset();

        for (uint8_t t = 0; t < 12; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ((uint8_t)dut.getModule()->getPC(), (i << 4) | i);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, BNEZ_NJMP_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | 0x0;
        dut.getInstructionMemory()[1] = ADDI << 4 | 0x0;
        dut.getInstructionMemory()[2] = LDI << 4 | i;
        dut.getInstructionMemory()[3] = BNEZ << 4 | i;
        dut.reset();

        for (uint8_t t = 0; t < 12; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ((uint8_t)dut.getModule()->getPC(), 0x04);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, BEZ_JMP_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | 0x0;
        dut.getInstructionMemory()[1] = ADDI << 4 | 0x0;
        dut.getInstructionMemory()[2] = LDI << 4 | i;
        dut.getInstructionMemory()[3] = BEZ << 4 | i;
        dut.reset();

        for (uint8_t t = 0; t < 12; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ((uint8_t)dut.getModule()->getPC(), (i << 4) | i);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, BEZ_NJMP_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = LDI << 4 | 0x0;
        dut.getInstructionMemory()[1] = ADDI << 4 | 0x1;
        dut.getInstructionMemory()[2] = LDI << 4 | i;
        dut.getInstructionMemory()[3] = BEZ << 4 | i;
        dut.reset();

        for (uint8_t t = 0; t < 12; t++) {
            dut.tick();
        }

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ((uint8_t)dut.getModule()->getPC(), 0x04);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, HALT_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = HALT << 4 | 0xF;
        dut.reset();

        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ((uint8_t)dut.getModule()->getPC(), 0x0);
    }
}

TEST(PIONEER_INSTRUCTION_TEST, NOOP_TEST) {
    for (uint8_t i = 0; i <= 0xF; i++) {
        dut.resetMemory();
        dut.getInstructionMemory()[0] = NOOP << 4 | 0xF;
        dut.getInstructionMemory()[1] = NOOP << 4 | 0xF;
        dut.reset();

        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();
        dut.tick();

        svScope scope = svGetScopeFromName("TOP.system.memory_unit_0.program_counter_0");
        assert(scope);
        svSetScope(scope);

        ASSERT_EQ((uint8_t)dut.getModule()->getPC(), 0x2);
    }
}

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);
    Verilated::mkdir("logs");

    Verilated::traceEverOn(true);
    VerilatedVcdC* trace = new VerilatedVcdC;
    dut.getModule()->trace(trace, 1);
    trace->open("waveform.vcd");
    dut.setTrace(trace);

    testing::InitGoogleTest(&argc, argv);
    auto res = RUN_ALL_TESTS();

#if VM_COVERAGE
    Verilated::mkdir("logs");
    VerilatedCov::write("logs/coverage.dat");
#endif

    trace->close();
    dut.done();
    delete trace;

    return res;
}
