`define INNER

`ifdef OUTER_FALSE
`ifdef INNER
module should_not_exist0 ();
endmodule
`elsif INNER
module should_not_exist1 ();
endmodule
`endif
`endif

`define FIRST
`define SECOND

`ifdef FIRST
module first_branch (input i, output o);
  wire w0;
  assign w0 = i;
  assign o = w0;
endmodule
`elsif SECOND
module should_not_exist2 ();
endmodule
`endif
