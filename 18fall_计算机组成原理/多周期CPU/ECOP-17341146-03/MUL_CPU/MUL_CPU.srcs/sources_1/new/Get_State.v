`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/08 19:59:23
// Design Name: 
// Module Name: Get_State
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


module Get_State(
    input reset,
    input [2:0] state,
    input [5:0] OP,
    output reg [2:0] nxt_state
);
parameter [2:0] IF=3'b000,ID=3'b001,aEXE=3'b010,bEXE=3'b101,
                 cEXE=3'b110,MEM=3'b011,aWB=3'b111,cWB=3'b100;
parameter [5:0] add=6'b000000,sub=6'b000001,addiu=6'b000010,And=6'b010000,
                 andi=6'b010001,ori=6'b010010,xori=6'b010011,sll=6'b011000,
                 slti=6'b100110,slt=6'b100111,sw=6'b110000,lw=6'b110001,
                 beq=6'b110100,bne=6'b110101,bltz=6'b110110,
                 j=6'b111000,jr=6'b111001,jal=6'b111010,halt=6'b111111;
                 
always@( state or OP )begin
    //if(!reset) nxt_state<=IF;
    //else 
    begin
        case (state)
            IF: nxt_state<=ID;
            ID: begin
                    if( OP==j || OP==jal || OP==jr || OP==halt )nxt_state<=IF;
                    else if( OP==sw || OP==lw )nxt_state<=aEXE;
                    else if( OP==beq || OP==bne || OP==bltz )nxt_state<=bEXE;
                    else nxt_state<=cEXE;
                end
            aEXE: nxt_state=MEM;
            bEXE: nxt_state=IF;
            cEXE: nxt_state=cWB;
            MEM: begin
                    if( OP==lw )nxt_state=aWB;
                    else nxt_state=IF;
                 end
            aWB: nxt_state=IF;
            cWB: nxt_state=IF;
        endcase
    end
end
endmodule
