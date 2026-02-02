`ifdef NEVER_DEFINED
`include "preprocess_include.v"
`endif

module include_inactive_mod (input i, output o);
  wire w0;
  assign w0 = i;
  assign o = w0;
endmodule
