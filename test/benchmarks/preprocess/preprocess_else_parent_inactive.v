`ifdef OUTER_FALSE
`ifdef INNER
module should_not_exist0 ();
endmodule
`else
module should_not_exist1 ();
endmodule
`endif
`endif

module else_parent_inactive_ok (input i, output o);
  wire w0;
  assign w0 = i;
  assign o = w0;
endmodule
