`ifdef NEVER_DEFINED
`define SHOULD_NOT_EXIST foo
`endif

module define_inactive_mod (input i, output o);
  wire w0;
  assign w0 = i;
  assign o = w0;
endmodule
