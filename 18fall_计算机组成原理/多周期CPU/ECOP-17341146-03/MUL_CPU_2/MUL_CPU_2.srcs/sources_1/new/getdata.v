`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/19 19:26:50
// Design Name: 
// Module Name: getdata
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


module getdata(
    input [1:0]type,
    input [31:0] NowAddress,
    input [31:0] NxtAddress,
    input [31:0] result,
    input [31:0] datas,
    input [31:0] datat,
    input [4:0] rs,
    input [4:0] rt,
    inout [31:0] writeData,
    output reg [3:0]A,
    output reg [3:0]B,
    output reg [3:0]C,
    output reg [3:0]D
);

always @( type or NowAddress or NxtAddress or rs or rt or result or datas or datat ) begin
    case (type)
        2'b00: begin
           A=NowAddress[7:4];B=NowAddress[3:0];
           C=NxtAddress[7:4];D=NxtAddress[3:0];
        end
        2'b01: begin
            A={3'b000,rs[4:4]};B=rs[3:0];
            C=datas[7:4];D=datas[3:0];
        end
        2'b10: begin
            A={3'b000,rt[4:4]};B=rt[3:0];
            C=datat[7:4];D=datat[3:0];
        end
        2'b11: begin
            A=result[7:4];B=result[3:0];
            C=writeData[7:4];D=writeData[3:0];
        end
    endcase
end   
 
endmodule
