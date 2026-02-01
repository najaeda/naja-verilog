`include "preprocess_include.v"
`define TOP_EN

`ifdef TOP_EN
module top (input i, output o);
  inc_mod u0 (.i(i), .o(o));
endmodule
`endif
