`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/08 20:02:18
// Design Name: 
// Module Name: Get_Op
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


module Get_Op(
    input [2:0]state,
    inout [2:0]nxt_state,
    input [5:0]OP,
    input zero,
    output reg PCWre,IRWre,ALUSrcA,ALUSrcB,
    output reg WrRegDSrc,DBDataSrc,RegWre,
    output reg InsMemRW,mRD,mWR,Extsel,
    output reg [1:0] RegDst,
    output reg [1:0] PCSrc,
    output reg [2:0] ALUOp
);
parameter [2:0] IF=3'b000,ID=3'b001,aEXE=3'b010,bEXE=3'b101,
                 cEXE=3'b110,MEM=3'b011,aWB=3'b111,cWB=3'b100;
parameter [5:0] add=6'b000000,sub=6'b000001,addiu=6'b000010,And=6'b010000,
                 andi=6'b010001,ori=6'b010010,xori=6'b010011,sll=6'b011000,
                 slti=6'b100110,slt=6'b100111,sw=6'b110000,lw=6'b110001,
                 beq=6'b110100,bne=6'b110101,bltz=6'b110110,
                 j=6'b111000,jr=6'b111001,jal=6'b111010,halt=6'b111111;

initial begin
    PCWre=0;ALUSrcA=0;ALUSrcB=0;DBDataSrc=0;RegWre=0;
    WrRegDSrc=1;InsMemRW=0;mRD=0;mWR=0;IRWre=0;Extsel=1;
    PCSrc=2'b00;RegDst=2'b00;ALUOp=3'b000;
end

always@( state or nxt_state or OP or zero )begin
    PCWre=( nxt_state==IF && OP!=halt )?1:0;//
    ALUSrcA=( OP==sll )?1:0;
    ALUSrcB=( OP==lw || OP==sw || OP==addiu || OP==andi || OP==ori || OP==xori || OP==slti )?1:0;
    DBDataSrc=( OP==lw )?1:0;
    //RegWre=( OP==bne || OP==beq || OP==bltz || OP==halt || OP==sw || OP==j || OP==jr )?0:1;
    RegWre=( state==aWB || state==cWB || ( OP==jal && state==ID ) )?1:0;
    WrRegDSrc=( OP==jal )?0:1;
    InsMemRW=1;
    mRD=( OP==lw )?1:0;
    mWR=( OP==sw )?1:0;
    IRWre=( state==ID )?1:0;
    Extsel=( OP==andi || OP==ori || OP==xori )?0:1;
    if( OP==j || OP==jal )PCSrc=2'b11;
    else if( OP==jr )PCSrc=2'b10;
    else if( (OP==beq && zero==1) || (OP==bne && zero==0) || (OP==bltz && zero==0) )PCSrc=2'b01;
    else PCSrc=2'b00;
    //PCSrc
    if( OP==add || OP==sub || OP==And || OP==sll || OP==slt )RegDst=2'b10;
    else if( OP==jal )RegDst=2'b00;
    else RegDst=2'b01;
    //RegDst
    if( OP==lw || OP==sw || OP==add || OP==addiu )ALUOp=3'b000;
    else if( OP==beq || OP==bne || OP==sub )ALUOp=3'b001;
    else if( OP==sll )ALUOp=3'b010;
    else if( OP==ori )ALUOp=3'b011;
    else if( OP==And || OP==andi )ALUOp=3'b100;
    else if( OP==xori )ALUOp=3'b111;
    else ALUOp=3'b110;
    //ALUOp
end

endmodule
