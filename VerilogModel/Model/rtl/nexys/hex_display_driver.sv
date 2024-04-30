// Module controlling the seven-segment displays of the NEXYS A7.
// Output is mostly realized using LUTs.

`include "defines.vh"

module hex_display_driver (
    input display_clk,
    input [`WORD_SIZE*8-1:0] display_data,
    output logic [7:0] segment_data,
    output logic [7:0] segment_select
);
    reg [2:0] segment_counter;
    logic [`WORD_SIZE-1:0] bcd;

    always_comb begin
        case (segment_counter)
            3'b000: segment_select = 8'b11111110;
            3'b001: segment_select = 8'b11111101;
            3'b010: segment_select = 8'b11111011;
            3'b011: segment_select = 8'b11110111;
            3'b100: segment_select = 8'b11101111;
            3'b101: segment_select = 8'b11011111;
            3'b110: segment_select = 8'b10111111;
            3'b111: segment_select = 8'b01111111;
        endcase

        case (segment_counter)
            3'b000: bcd = display_data[`WORD_SIZE*1-1:`WORD_SIZE*0];
            3'b001: bcd = display_data[`WORD_SIZE*2-1:`WORD_SIZE*1];
            3'b010: bcd = display_data[`WORD_SIZE*3-1:`WORD_SIZE*2];
            3'b011: bcd = display_data[`WORD_SIZE*4-1:`WORD_SIZE*3];
            3'b100: bcd = display_data[`WORD_SIZE*5-1:`WORD_SIZE*4];
            3'b101: bcd = display_data[`WORD_SIZE*6-1:`WORD_SIZE*5];
            3'b110: bcd = display_data[`WORD_SIZE*7-1:`WORD_SIZE*6];
            3'b111: bcd = display_data[`WORD_SIZE*8-1:`WORD_SIZE*7];
        endcase

        case (bcd)  // ----------------PGFEDCBA
            4'b0000: segment_data = 8'b11000000;  // 0
            4'b0001: segment_data = 8'b11111001;  // 1
            4'b0010: segment_data = 8'b10100100;  // 2
            4'b0011: segment_data = 8'b10110000;  // 3
            4'b0100: segment_data = 8'b10011001;  // 4
            4'b0101: segment_data = 8'b10010010;  // 5
            4'b0110: segment_data = 8'b10000010;  // 6
            4'b0111: segment_data = 8'b11111000;  // 7
            4'b1000: segment_data = 8'b10000000;  // 8
            4'b1001: segment_data = 8'b10011000;  // 9
            4'b1010: segment_data = 8'b10001000;  // A
            4'b1011: segment_data = 8'b10000011;  // B
            4'b1100: segment_data = 8'b11000110;  // C
            4'b1101: segment_data = 8'b10100001;  // D 
            4'b1110: segment_data = 8'b10000110;  // E
            4'b1111: segment_data = 8'b10001110;  // F
        endcase
    end

    always_ff @(posedge display_clk) begin
        segment_counter <= segment_counter + 1;
    end
endmodule
