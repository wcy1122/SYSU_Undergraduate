`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/14 16:57:57
// Design Name: 
// Module Name: PC
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
module PC(
    input clk,
    input Reset,
    input PCWre,
    input [31:0] address,
    output reg[31:0] NowAddress
);
reg [31:0]new;

initial begin
    NowAddress<=0;
end
	 
always @(posedge clk or negedge Reset)
begin
    if (Reset == 0) NowAddress<=0;
	else begin
	   if (PCWre) NowAddress<=address;
	   else NowAddress<=NowAddress;
	end
end

endmodule