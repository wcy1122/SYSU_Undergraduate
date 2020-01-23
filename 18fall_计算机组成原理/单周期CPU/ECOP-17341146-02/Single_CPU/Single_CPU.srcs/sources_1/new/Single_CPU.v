`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/15 20:25:49
// Design Name: 
// Module Name: Single_CPU
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


module Single_CPU(
    input clk,Reset,
    output [31:0] address,
    output [31:0] result,
    output [31:0] data1,
    output [31:0] data2,
    output [31:0] data3,
    output [31:0] data4,
    output [5:0] OP,
    output [4:0] rs,
    output [4:0] rt,
    output [4:0] rd,
    output [4:0] RegWrite,
    output [31:0] writeData,
    output [2:0] ALUOp,
    output [1:0] PCSrc
);
wire zero,PCWre,ALUSrcA,ALUSrcB,DBDataSrc,RegWre,InsMenRW,mRD,mWR,RegDst,ExtSel;
wire [4:0] sa;wire [15:0] immediate;wire [31:0] Extend;wire [31:0] Dataout;
wire [31:0] NewAddress;

PC pc(clk,Reset,PCWre,NewAddress,address);
//pc
InstructionMemory ins(InsMenRW,address,OP,rs,rt,rd,sa,immediate);
//取指
control cont(OP,zero,PCWre,ALUSrcA,ALUSrcB,DBDataSrc,RegWre,InsMenRW,mRD,mWR,RegDst,ExtSel,PCSrc,ALUOp);
//译码
SignExtend SE(immediate,ExtSel,Extend);
//符号拓展
RegFile RF(clk,Reset,RegWre,rs,rt,RegWrite,writeData,data3,data4);
//读写寄存器
ALU ALU(ALUOp,data1,data2,result,zero);
//计算
DataMemory Data(clk,Reset,result,data4,mRD,mWR,Dataout);
//数据内存读写
MUX5 M1(rt,rd,RegDst,RegWrite);
//选择数据寄存器的写入地址
MUX532 M2(data3,sa,ALUSrcA,data1);
//选择ALU的data1
MUX32 M3(data4,Extend,ALUSrcB,data2);
//选择ALU的data2
MUX32 M4(result,Dataout,DBDataSrc,writeData);
//选择写入内存数字
GetAddress get(PCSrc,Extend,address,NewAddress);
//算地址

endmodule