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


module single_spu_sim();
reg clk,Reset;
wire [31:0] address;
wire [31:0] result;
wire [31:0] data1;
wire [31:0] data2;
wire [31:0] data3;
wire [31:0] data4;
wire [5:0] OP;
wire [4:0] rs;
wire [4:0] rt;
wire [4:0] rd;
wire [4:0] RegWrite;
wire [31:0] writeData;
wire [2:0] ALUOp;
wire [1:0] PCSrc;

Single_CPU cpu(
    .clk(clk),
    .Reset(Reset),
    .address(address),
    .result(result),
    .data1(data1),
    .data2(data2),
    .data3(data3),
    .data4(data4),
    .OP(OP),
    .rs(rs),
    .rt(rt),
    .rd(rd),
    .RegWrite(RegWrite),
    .writeData(writeData),
    .ALUOp(ALUOp),
    .PCSrc(PCSrc)
);
always #50 clk=!clk;
initial begin
    Reset=0;clk=0;clk=1;
    #210;Reset=1;
end

endmodule
