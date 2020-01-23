`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/07 15:16:42
// Design Name: 
// Module Name: Mul_CPU
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


module Mul_CPU(
    input clk,Reset,
    output [31:0] address,
    output [31:0] NewAddress,
    output [31:0] result,
    output [31:0] Regout1,
    output [31:0] Regout2,
    output [4:0] rs,
    output [4:0] rt,
    output [31:0] writeData
);

wire zero,PCWre,IRWre,ALUSrcA,ALUSrcB,WrRegDSrc,DBDataSrc,RegWre,InsMenRW,mRD,mWR,ExtSel;
wire [31:0] inst,data1,data2,data3,data4;
wire [31:0] DAddr,Extend,Dataout,PC4,PCjump,PCbranch,wData1,wData;
wire [4:0] rd,sa,RegWrite;wire [15:0] immediate;
wire [5:0] OP;wire [2:0] ALUOp;wire [1:0] RegDst,PCSrc;
parameter endReg = 5'b11111;

PC pc(clk,Reset,PCWre,NewAddress,address);
//get_PC
PC_Add add(address,PC4);
//PC+4
Ins_Memory ins(InsMenRW,address,inst);
//??
Ins_Reg insreg(clk,inst,OP,rs,rt,rd,sa,immediate);
//?????
control ctr(clk,Reset,OP,zero,PCWre,IRWre,ALUSrcA,ALUSrcB,WrRegDSrc,DBDataSrc,RegWre,InsMenRW,mRD,mWR,ExtSel,RegDst,PCSrc,ALUOp);
//??
SignExtend SE(immediate,ExtSel,Extend);
//??¦¨?
PC_branch branch(PC4,Extend,PCbranch);
PC_Jump jump(PC4,Extend,PCjump);
//PC_jump
RegFile RF(clk,Reset,RegWre,rs,rt,RegWrite,writeData,Regout1,Regout2);
//?§Õ???
D_trigger D1(clk,Regout1,data3);
D_trigger D2(clk,Regout2,data4);
//ALU?????
MUX532 M1(data3,sa,ALUSrcA,data1);
MUX32 M2(data4,Extend,ALUSrcB,data2);
//ALU?????
ALU ALU(ALUOp,data1,data2,result,zero);
//??
D_trigger D3(clk,result,DAddr);
//???????
DataMemory Data(clk,Reset,DAddr,data4,mRD,mWR,Dataout);
//?????§Õ
MUX3 M3(RegDst,endReg,rt,rd,RegWrite);
//????????§Õ???
MUX32 M4(result,Dataout,DBDataSrc,wData1);
D_trigger D4(clk,wData1,wData);
//???§Õ????
MUX32 M5(PC4,wData,WrRegDSrc,writeData);
//??§Õ??????
Get_Address geta(PCSrc,PC4,PCbranch,Regout1,PCjump,NewAddress);

endmodule
