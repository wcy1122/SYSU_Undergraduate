`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/12/07 16:22:20
// Design Name: 
// Module Name: MUX3
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


module MUX3(
    input [1:0]op,
    input [4:0]data0,[4:0]data1,[4:0]data2,
    output reg [4:0]result
);

always@(*)begin
    case (op)
        2'b00: result=data0;
        2'b01: result=data1;
        2'b10: result=data2;
    endcase
end
endmodule
