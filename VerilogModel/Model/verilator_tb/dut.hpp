#pragma once

#include <verilated_vcd_c.h>

template <class MODULE>
class DUT {
    protected:
    unsigned long ticks;
    MODULE* module;
    VerilatedVcdC *trace;

    public:
    DUT() {
        module = new MODULE;
        ticks = 0;
    }

    virtual ~DUT() {
        delete module;
        module = nullptr;
    }

    virtual void reset() {
        module->reset = 1;
        this->tick();
        module->reset = 0;
    }

    virtual void tick() {
        module->clk = 0;
        module->eval();

        module->clk = 1;
        ticks++;
        module->eval();
        trace->dump(ticks);
        module->clk = 0;
        ticks++;
        module->eval();
        trace->dump(ticks);

        onTick();
    }

    virtual void onTick() {

    }

    virtual bool done() {
        return Verilated::gotFinish();
    }

    MODULE* getModule() {
        return module;
    }

    void setTrace(VerilatedVcdC *trace) {
        this->trace = trace;
    }
};