`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/15 18:58:19
// Design Name: 
// Module Name: Ins_Reg
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


module Ins_Reg(
    input clk,
    input [31:0]datain,
    output reg [5:0]OP,
    output reg [4:0]rs,
    output reg [4:0]rt,
    output reg [4:0]rd,
    output reg [4:0]sa,
    output reg [15:0]immediate
);
always @(posedge clk)begin
    OP= datain [31:26];
    rs= datain [25:21];
    rt= datain [20:16];
    rd= datain [15:11];
    sa= datain [10:6];
    immediate= datain [15:0];
end
endmodule