`define PATHS_WIRE w0

module paths_with_macro (input i, output o);
  wire `PATHS_WIRE;
  assign `PATHS_WIRE = i;
  assign o = `PATHS_WIRE;
endmodule
