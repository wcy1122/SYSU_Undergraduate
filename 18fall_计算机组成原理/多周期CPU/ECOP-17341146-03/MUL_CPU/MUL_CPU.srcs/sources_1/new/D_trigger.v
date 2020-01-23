`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/07 16:17:01
// Design Name: 
// Module Name: D_trigger
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


module D_trigger(
    input clk,
    input [31:0]datain,
    input type,
    output reg [31:0]dataout
);

always @(posedge clk)begin
    if(type)dataout<=datain;
end
always @(negedge clk)begin
    if(!type)dataout<=datain;
end

endmodule
