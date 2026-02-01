`define ESCAPED_MACRO replaced

module string_escape (input i, output o);
  wire w0;
  assign w0 = i;
  assign o = w0;
  // Ensure macro is not expanded inside string and escaped quote path executes.
  assign o = "text with \\\" and `ESCAPED_MACRO inside";
endmodule
