// ALU module of the RW-Pioneer.
// Module operation is defined by the incoming alu micro instruction.

`include "defines.vh"

module alu (
    input clk,
    input reset,
    input [`ALU_MICRO_INSTRUCTION_SIZE-1:0] alu_instruction,
    input [`WORD_SIZE-1:0] bus_in,
    output [`WORD_SIZE-1:0] bus_out,
`ifdef NEXYSA7
    output [`WORD_SIZE-1:0] acc_out,
`endif
    output zero_flag
);
    reg [`WORD_SIZE-1:0] acc_register;
    reg carry_flag_register;
    reg zero_flag_register;

    wire [`WORD_SIZE-1:0] adder_res;
    wire carry_in;
    wire carry_out;
    wire sub_flag;
    wire [`WORD_SIZE-1:0] b_in;

`ifdef VERILATOR
    export "DPI-C" function getACC;

    function byte getACC();
        return {4'b0, acc_register};
    endfunction
`endif

    assign carry_in = ((alu_instruction == `ALU_ADDC && carry_flag_register) ||
                      alu_instruction == `ALU_SUB ||
                      (alu_instruction == `ALU_SUBC && carry_flag_register))
                      ? 1'b1
                      : 1'b0;
    assign sub_flag = (alu_instruction == `ALU_SUB || alu_instruction == `ALU_SUBC) ? 1'b1 : 1'b0;
    assign zero_flag = zero_flag_register;
    assign bus_out = (alu_instruction == `ALU_ACCTOBUS) ? acc_register : '0;

    assign b_in = bus_in ^ {(`WORD_SIZE) {sub_flag}};
    assign {carry_out, adder_res} = acc_register + b_in + {3'b000, carry_in};

`ifdef NEXYSA7
    assign acc_out = acc_register;
`endif

    always_ff @(posedge clk) begin
        if (!reset) begin
            case (alu_instruction)
                `ALU_ADD, `ALU_ADDC, `ALU_SUB, `ALU_SUBC: begin
                    acc_register <= adder_res;
                    carry_flag_register <= carry_out;
                    zero_flag_register <= ~(|(adder_res));
                end
                `ALU_BUSTOACC: acc_register <= bus_in;
                default: ;
            endcase
        end else begin
            acc_register <= 'b0;
        end
    end
endmodule
