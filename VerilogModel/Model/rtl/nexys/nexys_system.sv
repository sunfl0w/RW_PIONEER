// Top module of the system for the NEXYS A7.
// This module includes additional output logic for the seven-segment displays of the NEXYS A7 compared to the simulation top module (system).

`include "defines.vh"

module system_nexysa7 (
    input CLK100MHZ,
    input RESETN,
    output [7:0] AN,
    output CA,
    output CB,
    output CC,
    output CD,
    output CE,
    output CF,
    output CG,
    output DP
);
    wire clk;
    wire clk_hex_display;
    wire reset;
    wire [`WORD_SIZE-1:0] bus_out_alu;
    wire [`WORD_SIZE-1:0] bus_out_mu;
    wire zero_flag;
    wire ce;
    wire halt_pc;
    wire [`WORD_SIZE-1:0] bus;
    wire [`WORD_SIZE-1:0] instruction_code;
    wire [`WORD_SIZE-1:0] instruction_value;
    wire [`ALU_MICRO_INSTRUCTION_SIZE-1:0] alu_instruction;
    wire [`MEM_MICRO_INSTRUCTION_SIZE-1:0] mem_instruction;
    wire [`WORD_SIZE-1:0] acc;
    wire [`PC_SIZE-1:0] pc;
    wire [1:0] step;

    wire [`WORD_SIZE*8-1:0] display_data;
    wire [7:0] segment_data;
    wire [7:0] segment_select;

    reg [20:0] clk_div;
    reg [`WORD_SIZE-1:0] output_regs[0:3];

    assign reset = !RESETN;
    assign bus = bus_out_alu | bus_out_mu;
    assign {DP, CG, CF, CE, CD, CC, CB, CA} = segment_data;
    assign AN = segment_select;

    assign display_data = {pc, {2'b00, step}, acc, output_regs[0], output_regs[1], output_regs[2], output_regs[3]};

    always_ff @(posedge CLK100MHZ) begin
        clk_div <= clk_div + 1;
    end

    // Output registers sort of work like the HEX display module of the original 7400-based design
    always_ff @(posedge clk) begin
        if (reset) begin
            output_regs[0] <= 0;
            output_regs[1] <= 0;
            output_regs[2] <= 0;
            output_regs[3] <= 0;
        end else begin
            if (mem_instruction == `MEM_BUSTORAM && instruction_value[3:2] == 2'b11) begin
                output_regs[instruction_value[1:0]] <= bus;
            end
        end
    end

    // ~47.7 Hz
    BUFG bufg_main_clk (
        .I(clk_div[20]),
        .O(clk)
    );

    // ~763 Hz
    BUFG bufg_hex_clk (
        .I(clk_div[16]),
        .O(clk_hex_display)
    );

    memory_unit memory_unit_0 (
        .clk(clk),
        .reset(reset),
        .ce(ce),
        .halt_pc(halt_pc),
        .mem_instruction(mem_instruction),
        .bus_in(bus),
        .bus_out(bus_out_mu),
        .instruction_value(instruction_value),
        .pc_out(pc),
        .instruction_code(instruction_code)
    );

    control control_0 (
        .clk(clk),
        .reset(reset),
        .zero_flag(zero_flag),
        .instruction_code(instruction_code),
        .step_out(step),
        .ce(ce),
        .halt_pc(halt_pc),
        .alu_instruction(alu_instruction),
        .mem_instruction(mem_instruction)
    );

    alu alu_0 (
        .clk(clk),
        .reset(reset),
        .alu_instruction(alu_instruction),
        .bus_in(bus),
        .bus_out(bus_out_alu),
        .acc_out(acc),
        .zero_flag(zero_flag)
    );

    hex_display_driver hex_display_driver_0 (
        .display_clk(clk_hex_display),
        .display_data(display_data),
        .segment_data(segment_data),
        .segment_select(segment_select)
    );
endmodule
