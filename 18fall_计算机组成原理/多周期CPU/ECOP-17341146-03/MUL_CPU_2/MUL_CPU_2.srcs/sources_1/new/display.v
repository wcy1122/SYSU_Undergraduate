`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/16 23:55:01
// Design Name: 
// Module Name: display
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module display(
    input clk,
    input [3:0]A,
    input [3:0]B,
    input [3:0]C,
    input [3:0]D,
    output [7:0]dispcode,
    output reg [3:0]dispseg
);
integer i;initial i=0;
reg [3:0] tmp;
always @(posedge clk) begin
    i=i+1;
    if(i>3)i=0;
    case (i)
        0: begin
            tmp=A; dispseg=4'b0111;
        end
        1: begin
            tmp=B; dispseg=4'b1011;
        end
        2: begin
            tmp=C; dispseg=4'b1101;
        end
        3: begin
            tmp=D; dispseg=4'b1110;
        end
    endcase 
end
SegLED seg(tmp,dispcode);
endmodule
