module block_comment (input i, output o);
  /* start comment `MACRO_SHOULD_NOT_EXPAND
     still comment */ assign o = i;
endmodule
