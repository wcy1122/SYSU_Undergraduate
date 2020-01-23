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

module InstructionMemory(
    input InsMenRW,
    input [31:0] address,
    output reg [5:0] OP,
    output reg [4:0] rs,
    output reg [4:0] rt,
    output reg [4:0] rd,
    output reg [4:0] sa,
    output reg [31:0] immediate
);
reg [7:0] Memory [255:0];
reg [31:0] now;
initial begin
    $readmemb("D:/vivado/Single_CPU/input.txt", Memory);
end

always@(address or InsMenRW)begin
    if(!InsMenRW)begin
        now={ Memory[address] , Memory[address+1] , Memory[address+2] , Memory[address+3] };
        OP= now [31:26];
        rs= now [25:21];
        rt= now [20:16];
        rd= now [15:11];
        sa= now [10:6];
        immediate= now [15:0];
    end
end
endmodule