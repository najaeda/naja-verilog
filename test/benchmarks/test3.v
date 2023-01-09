module mod0(input i0, output o0);
endmodule

module test(input i, output o, inout io);
  wire net0;
  wire net1, net2, net3;
  wire [31:0] net4;
  supply0 constant0;
  supply1 constant1;

  mod0 inst0(.i0(net0), .o0(/*empty*/));
  mod0 inst1(.i0(net4[21]), .o0(net4[5]));

endmodule