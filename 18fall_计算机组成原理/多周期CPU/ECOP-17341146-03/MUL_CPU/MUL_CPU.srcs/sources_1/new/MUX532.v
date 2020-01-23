`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/15 14:14:11
// Design Name: 
// Module Name: MUX
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


module MUX532(
    input [31:0]data0,[4:0]data1,
    input op,
    output reg [31:0]result
);
always@(*)begin
    result=(op==0)?data0:{27'b0000,data1};
end
endmodule
