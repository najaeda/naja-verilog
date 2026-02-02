`ifdef NEVER_DEFINED
`timescale 10ns/1ns
`endif

module timescale_inactive (input i, output o);
  wire w0;
  assign w0 = i;
  assign o = w0;
endmodule
