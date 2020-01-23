`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/09 14:19:01
// Design Name: 
// Module Name: D_state
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


module D_state(
    input clk,
    input reset,
    input [2:0] nxt_state,
    output reg [2:0] state
);

initial begin
    state=3'b000;
end

always @(posedge clk)begin
    if(!reset)state<=3'b000;
    else state<=nxt_state;
end

endmodule
