module test0;
endmodule

module test1();
endmodule

module test2(input i0, input i1, output o0, output o1, inout io);
endmodule

module test3(input i0, i1, output o0, o1, inout io);
endmodule

module test4(i0, i1, o0, o1, io);
input i0;
input i1;
output o0;
output o1;
inout io;
endmodule

module test5(input [3:0] i0, i1, output [0:3] o0, o1, inout io);
endmodule