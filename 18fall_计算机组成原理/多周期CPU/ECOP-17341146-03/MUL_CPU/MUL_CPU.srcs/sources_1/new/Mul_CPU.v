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
    output [31:0] address
);
wire zero,PCWre,IRWre,ALUSrcA,ALUSrcB,WrRegDSrc,DBDataSrc,RegWre,InsMenRW,mRD,mWR,ExtSel;
wire [31:0] result,inst,Regout1,Regout2,data1,data2,data3,data4;
wire [31:0] DAddr,writeData,Extend,Dataout,PC4,PCjump,PCbranch,NewAddress,wData1,wData;
wire [4:0] rs,rt,rd,sa,RegWrite;wire [15:0] immediate;
wire [5:0] OP;wire [2:0] ALUOp;wire [1:0] RegDst,PCSrc;
parameter endReg = 5'b11111;

PC pc(clk,Reset,PCWre,NewAddress,address);
//get_PC
PC_Add add(address,PC4);
//PC+4
Ins_Memory ins(InsMenRW,address,inst);
//ȡָ
Ins_Reg insreg(clk,inst,OP,rs,rt,rd,sa,immediate);
//ָ��Ĵ���
control ctr(clk,Reset,OP,zero,PCWre,IRWre,ALUSrcA,ALUSrcB,WrRegDSrc,DBDataSrc,RegWre,InsMenRW,mRD,mWR,ExtSel,RegDst,PCSrc,ALUOp);
//����
SignExtend SE(immediate,ExtSel,Extend);
//������չ
PC_branch branch(PC4,Extend,PCbranch);
PC_Jump jump(PC4,Extend,PCjump);
//PC_jump
RegFile RF(clk,Reset,RegWre,rs,rt,RegWrite,writeData,Regout1,Regout2);
//��д�Ĵ���
D_trigger D1(clk,Regout1,1'b1,data3);
D_trigger D2(clk,Regout2,1'b1,data4);
//ALU���봥����
MUX532 M1(data3,sa,ALUSrcA,data1);
MUX32 M2(data4,Extend,ALUSrcB,data2);
//ALU����ѡ����
ALU ALU(ALUOp,data1,data2,result,zero);
//����
D_trigger D3(clk,result,1'b1,DAddr);
//�ڴ��ַ������
DataMemory Data(clk,Reset,DAddr,data4,mRD,mWR,Dataout);
//�����ڴ��д
MUX3 M3(RegDst,endReg,rt,rd,RegWrite);
//ѡ�����ݼĴ�����д���ַ
MUX32 M4(result,Dataout,DBDataSrc,wData1);
D_trigger D4(clk,wData1,1'b1,wData);
//�Ĵ���д�봥����
MUX32 M5(PC4,wData,WrRegDSrc,writeData);
//ѡ��д��Ĵ�������
Get_Address geta(PCSrc,PC4,PCbranch,Regout1,PCjump,NewAddress);
//���µ�ַ

endmodule
