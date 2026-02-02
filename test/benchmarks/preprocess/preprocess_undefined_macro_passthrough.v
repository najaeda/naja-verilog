module undefined_macro_passthrough (input i, output o);
  wire `UNDEF_MACRO;
  assign o = i;
endmodule
