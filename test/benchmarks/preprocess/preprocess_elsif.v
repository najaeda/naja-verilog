`define HAS_ELSE

`ifdef UNKNOWN_MACRO
module should_not_exist ();
endmodule
`elsif HAS_ELSE
module elsif_mod (input i, output o);
  wire w0;
  assign w0 = i;
  assign o = w0;
endmodule
`else
module else_mod ();
endmodule
`endif

`ifdef OUTER_FALSE
`elsif ALSO_FALSE
module skipped_mod ();
endmodule
`else
`define INNER_DEFINED inner_net
module else_taken (input i, output o);
  wire `INNER_DEFINED;
  assign `INNER_DEFINED = i;
  assign o = `INNER_DEFINED;
endmodule
`endif
