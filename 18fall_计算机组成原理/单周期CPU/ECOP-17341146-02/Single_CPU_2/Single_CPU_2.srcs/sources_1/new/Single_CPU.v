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
    output [31:0] NewAddress,
    output [31:0] result,
    output [31:0] data3,
    output [31:0] data4,
    output [4:0] rs,
    output [4:0] rt,
    output [31:0] writeData
);
wire zero,PCWre,ALUSrcA,ALUSrcB,DBDataSrc,InsMenRW,RegDst,ExtSel,RegWre,mRD,mWR;
wire [1:0] PCSrc;wire [2:0] ALUOp;wire [5:0] OP;
wire [4:0] rd;wire [4:0] sa;wire [15:0] immediate;
wire [31:0] data1;wire [31:0] data2;
wire [4:0] RegWrite;wire [31:0] Extend;
wire [31:0] Dataout;

PC pc(clk,Reset,PCWre,NewAddress,address);
//pc
InstructionMemory ins(InsMenRW,address,OP,rs,rt,rd,sa,immediate);
//ȡָ
control cont(OP,zero,PCWre,ALUSrcA,ALUSrcB,DBDataSrc,RegWre,InsMenRW,mRD,mWR,RegDst,ExtSel,PCSrc,ALUOp);
//����
SignExtend SE(immediate,ExtSel,Extend);
//������չ
RegFile RF(clk,Reset,RegWre,rs,rt,RegWrite,writeData,data3,data4);
//��д�Ĵ���
ALU ALU(ALUOp,data1,data2,result,zero);
//����
DataMemory Data(clk,Reset,result,data4,mRD,mWR,Dataout);
//�����ڴ��д
MUX5 M1(rt,rd,RegDst,RegWrite);
//ѡ�����ݼĴ�����д���ַ
MUX532 M2(data3,sa,ALUSrcA,data1);
//ѡ��ALU��data1
MUX32 M3(data4,Extend,ALUSrcB,data2);
//ѡ��ALU��data2
MUX32 M4(result,Dataout,DBDataSrc,writeData);
//ѡ��д��Ĵ�������
GetAddress get(clk,Reset,PCSrc,Extend,address,NewAddress);
//���ַ

endmodule