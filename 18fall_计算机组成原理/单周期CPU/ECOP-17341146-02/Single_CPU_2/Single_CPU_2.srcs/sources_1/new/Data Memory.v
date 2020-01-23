`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/14 19:06:59
// Design Name: 
// Module Name: Data Memory
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


module DataMemory(
    input clk,reset,
    input [31:0] address,
    input [31:0] writeData,
    input mRD,
    input mWR,
    output reg [31:0] Dataout
);
reg [7:0] ram [0:60];

// ¶Á
always @(address or mRD) begin
    Dataout[7:0] = (mRD==1)?ram[address + 3]:8'bz; // z Îª¸ß×èÌ¬
    Dataout[15:8] = (mRD==1)?ram[address + 2]:8'bz;
    Dataout[23:16] = (mRD==1)?ram[address + 1]:8'bz;
    Dataout[31:24] = (mRD==1)?ram[address ]:8'bz;
end

// Ð´
always@( negedge clk ) begin
    if( mWR==1 ) begin
        ram[address] <= writeData[31:24];
        ram[address+1] <= writeData[23:16];
        ram[address+2] <= writeData[15:8];
        ram[address+3] <= writeData[7:0];
    end
end
endmodule
