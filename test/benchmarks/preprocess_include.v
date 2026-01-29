`define INC_WIRE w

module inc_mod (input i, output o);
  wire `INC_WIRE;
  assign `INC_WIRE = i;
  assign o = `INC_WIRE;
endmodule
