`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/16 00:22:42
// Design Name: 
// Module Name: single_spu_sim
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


module mul_spu_sim();
reg clk,Reset;
wire [31:0]address;
Mul_CPU cpu(
    .clk(clk),
    .Reset(Reset),
    .address(address)
);
always #50 clk=!clk;
initial begin
    Reset=0;clk=0;clk=1;
    #180;Reset=1;
end

endmodule
