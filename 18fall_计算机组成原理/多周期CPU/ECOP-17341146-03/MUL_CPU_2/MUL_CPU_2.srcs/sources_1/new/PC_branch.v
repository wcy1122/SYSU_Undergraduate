`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/09 23:11:15
// Design Name: 
// Module Name: PC_branch
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


module PC_branch(
    input [31:0]PC4,
    input [31:0]immediate,
    output reg[31:0] PC_Jump
);
always@(*)begin
    PC_Jump=PC4+immediate*4;
end
endmodule
