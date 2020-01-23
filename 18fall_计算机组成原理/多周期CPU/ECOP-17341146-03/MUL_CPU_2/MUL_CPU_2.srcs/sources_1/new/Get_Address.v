`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/20 23:24:50
// Design Name: 
// Module Name: Get_Address
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


module Get_Address(
    input [1:0] PCSrc,
    input [31:0] A,
    input [31:0] B,
    input [31:0] C,
    input [31:0] D,
    output reg [31:0] NewAddress
);

always @(*)begin
    case (PCSrc) 
	   2'b00: NewAddress=A;
	   2'b01: NewAddress=B;
	   2'b10: NewAddress=C;
	   2'b11: NewAddress=D;
    endcase
end

endmodule
