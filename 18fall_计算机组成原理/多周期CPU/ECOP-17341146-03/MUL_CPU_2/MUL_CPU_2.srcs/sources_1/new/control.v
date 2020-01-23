`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/15 18:58:19
// Design Name: 
// Module Name: control
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


module control(
    input clk,
    input reset,
    input [5:0] OP,
    input zero,
    output PCWre,IRWre,ALUSrcA,ALUSrcB,
    output WrRegDSrc,DBDataSrc,RegWre,
    output InsMemRW,mRD,mWR,Extsel,
    output [1:0] RegDst,
    output [1:0] PCSrc,
    output [2:0] ALUOp
);
wire [2:0] state;
wire [2:0] nxt_state;

Get_State gets(reset,state,OP,nxt_state);
Get_Op geto(state,nxt_state,OP,zero,PCWre,IRWre,ALUSrcA,ALUSrcB,WrRegDSrc,DBDataSrc,RegWre,InsMemRW,mRD,mWR,Extsel,RegDst,PCSrc,ALUOp);
D_state Ds(clk,reset,nxt_state,state);

endmodule