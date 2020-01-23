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
    input [5:0] OP,
    input zero,
    output reg PCWre,ALUSrcA,ALUSrcB,
    output reg DBDataSrc,RegWre,InsMenRW,
    output reg mRD,mWR,RegDst,ExtSel,
    output reg [1:0] PCSrc,
    output reg [2:0] ALUOp
);

initial begin
    InsMenRW=0;
end

always@(OP or zero)begin
    PCWre=( OP==6'b111111 )?0:1;
    ALUSrcA=( OP==6'b011000 )?1:0;
    ALUSrcB=( OP==6'b000010 || OP==6'b010000 || OP==6'b010010 || OP==6'b011100 || OP==6'b100110 || OP==6'b100111 )?1:0;
    DBDataSrc=( OP==6'b100111 )?1:0;
    RegWre=( OP==6'b110000 || OP==6'b110001 || OP==6'b110010 || OP==6'b100110 )?0:1;
    InsMenRW=0;
    mRD=( OP==6'b100111 )?1:0;
    mWR=( OP==6'b100110 )?1:0;
    RegDst=( OP==6'b000000 || OP==6'b000001 || OP==6'b010001 || OP==6'b010011 || OP==6'b011000 )?1:0;
    ExtSel=( OP==6'b010000 || OP==6'b010010 )?0:1;
    PCSrc[1]=( OP==6'b111000 )?1:0;
    PCSrc[0]=( (OP==6'b110000 && zero==1) || (OP==6'b110001 && zero==0) || (OP==6'b110010 && zero==0) )?1:0;
    ALUOp[2]=( OP==6'b010001 || OP==6'b011100 || OP==6'b110010 )?1:0;
    ALUOp[1]=( OP==6'b010011 || OP==6'b010010 || OP==6'b011000 || OP==6'b011100 || OP==6'b110010 )?1:0;
    ALUOp[0]=( OP==6'b000001 || OP==6'b010011 || OP==6'b010010 || OP==6'b110000 || OP==6'b110001 )?1:0;
end

endmodule
