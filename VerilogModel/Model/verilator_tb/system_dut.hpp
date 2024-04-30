#pragma once

#include "dut.hpp"

#include <array>

template <typename MODULE>
class SystemDUT : public DUT<MODULE> {
    std::array<uint8_t, 256> instruction_memory = std::array<uint8_t, 256>();
    std::array<uint8_t, 16> data_memory = std::array<uint8_t, 16>();

    public:
    void resetMemory() {
        instruction_memory.fill(0);
        data_memory.fill(0);
    }

    uint8_t* getInstructionMemory() {
        return instruction_memory.data();
    }

    uint8_t* getDataMemory() {
        return instruction_memory.data();
    }
};