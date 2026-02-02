`define KEEP_ME keep_me
`define TEMP
`undef TEMP

`ifdef NEVER_DEFINED
`undef KEEP_ME
`endif

module undef_mod (input i, output o);
  wire `KEEP_ME;
  assign `KEEP_ME = i;
  assign o = `KEEP_ME;
endmodule
