`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/08 10:47:49
// Design Name: 
// Module Name: PC_Jump
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


module PC_Jump(
    input [31:0]PC4,
    input [31:0]immediate,
    output reg [31:0]PCjump
);
always@(*)begin
    PCjump={ PC4[31:28],immediate[27:2],2'b00 };
end
endmodule
