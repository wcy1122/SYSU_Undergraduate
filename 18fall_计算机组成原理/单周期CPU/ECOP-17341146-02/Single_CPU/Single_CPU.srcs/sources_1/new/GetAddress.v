`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/20 23:24:50
// Design Name: 
// Module Name: GetAddress
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


module GetAddress(
    inout [1:0] PCSrc,
    inout [31:0] immediate,
    input [31:0] address,
    output reg [31:0] NewAddress
);

reg [31:0] new;
always @(address or PCSrc)begin
    new=address+4;
    case (PCSrc) 
	   2'b00: NewAddress=new;
	   2'b01: NewAddress=new+immediate*4;
	   2'b10: NewAddress={ new[31:28],immediate[27:2],2'b00 };
    endcase
end

endmodule
