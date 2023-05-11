module top(input in, output out);
  wire n;
  mod0 ins0(in, n);
  mod1 ins1(n, out);
endmodule