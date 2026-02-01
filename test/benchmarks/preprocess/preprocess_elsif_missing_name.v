`ifdef SOME_FLAG
module should_not_parse ();
endmodule
`elsif
module should_not_parse2 ();
endmodule
`endif
