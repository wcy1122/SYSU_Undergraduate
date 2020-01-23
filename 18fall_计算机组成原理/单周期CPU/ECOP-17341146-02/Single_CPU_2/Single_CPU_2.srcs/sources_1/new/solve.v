`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/16 22:25:19
// Design Name: 
// Module Name: solve
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


module solve(
    input clk,
    input tag,
    input Reset,
    input [1:0]type,
    output [7:0]dispcode,
    output [3:0]dispseg
);
wire clk1,clk2;
wire [31:0] NowAddress;
wire [31:0] NxtAddress;
wire [31:0] result;
wire [31:0] datas;
wire [31:0] datat;
wire [31:0] writeData;
wire [4:0] rs;
wire [4:0] rt;
wire [3:0]A,B,C,D;

getclk get(clk,tag,Reset,clk1,clk2);
//时钟处理
Single_CPU SCPU(clk1,Reset,NowAddress,NxtAddress,result,datas,datat,rs,rt,writeData);
//CPU
getdata data(clk1,type,NowAddress,NxtAddress,result,datas,datat,rs,rt,writeData,A,B,C,D);
//获得写板数据
display dis(clk2,A,B,C,D,dispcode,dispseg);
//写板

endmodule
