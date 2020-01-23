//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/14 16:57:57
// Design Name: 
// Module Name: Ins Memory
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

module Ins_Memory(
    input InsMenRW,
    input [31:0] address,
    output reg [31:0] ins
);
reg [7:0] Memory [255:0];
initial begin
    $readmemb("D:/vivado/MUL_CPU_2/input.txt", Memory);
end

always@(address or InsMenRW)begin
    if(InsMenRW)begin
        ins={ Memory[address] , Memory[address+1] , Memory[address+2] , Memory[address+3] };
    end
end
endmodule