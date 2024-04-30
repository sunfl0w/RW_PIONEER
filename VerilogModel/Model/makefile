MODULE := system
BUILD_DIR := ./build
RTL_DIR := ./rtl
VERILOG_INDLUCE_DIRS := -I./rtl -I./rtl/nexys
VERILATOR_FLAGS := -Wall --coverage --trace -O0 --Mdir $(BUILD_DIR)
CFLAGS := -Wall -O0
LDFLAGS := -lgtest

TB_MAIN_SRC := $(shell find $(TB_SRC_DIRS) -name '$(MODULE)_tb.cpp')

.PHONY:test
test: $(BUILD_DIR)/V$(MODULE)
	@echo -e
	@echo -e "RUNNING TEST\n"
	./$(BUILD_DIR)/V$(MODULE)

$(BUILD_DIR)/V$(MODULE): verilate
	@echo -e
	@echo -e "BUILDING\n"
	make -C $(BUILD_DIR) -f V$(MODULE).mk V$(MODULE) -j 8

.PHONY:verilate
verilate: $(RTL_DIR)/$(MODULE).sv $(TB_MAIN_SRC)
	@echo -e
	@echo -e "VERILATING\n"
	verilator $(VERILATOR_FLAGS) -cc $(RTL_DIR)/$(MODULE).sv $(VERILOG_INDLUCE_DIRS) --exe $(TB_MAIN_SRC) -CFLAGS $(CFLAGS) -LDFLAGS $(LDFLAGS) -j 8