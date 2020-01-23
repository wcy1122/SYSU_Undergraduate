`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/19 19:23:07
// Design Name: 
// Module Name: getclk
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


module getclk(
    input clk,tag,Reset,
    output clk1,
    output reg clk2
);
integer cnt,sum0,sum1;
reg now;

initial begin
    now=0;clk2=0;cnt=0;sum0=0;sum1=0;
end

always @(posedge clk) begin
    cnt=cnt+1;
    if(cnt>500)begin
        clk2=!clk2;cnt=0;
    end
    if(tag==0)sum0=sum0+1;
    else sum0=0;
    if(tag==1)sum1=sum1+1;
    else sum1=0;
    if(sum1>4)now=1;
    if(sum0>100)now=0;
end
assign clk1=now;

endmodule
