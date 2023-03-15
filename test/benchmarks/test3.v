/*
  Various Verilog constructs to test different aspects

*/

module mod0(input i0, output o0);
endmodule

module mod1(input[3:0] i0, output[0:3] o0);
endmodule

module test(input i, output o, inout io);
  wire net0;
  wire net1, net2, net3;
  wire [31:0] net4;
  wire [-2:1] net5;
  supply0 constant0;
  supply1 constant1;

  mod0 inst0(.i0(net0), .o0(/*empty*/));
  mod0 inst1(.i0(net4[21]), .o0(net4[5]));
  mod1 inst2(.i0(net4[3:6]), .o0(net5));
  mod1 inst3(.i0({net0, net1, net2, net5[-2]}), .o0(net5));

endmodule