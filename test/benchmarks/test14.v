/*
  test attributes (pragmas)
*/
(* top =  1  *)
(* src = "src/adder.v:1.1-8.10" *)
module adder(a, b, s);
  (* src = "src/adder.v:2.14-2.15" *)
  input [7:0] a;
  (* src = "src/adder.v:2.14-2.15" *)
  input [7:0] a;
  wire [7:0] a;
  (* src = "src/adder.v:3.14-3.15" *)
  input [7:0] b;
  wire [7:0] b;
  (* src = "src/adder.v:4.10-4.11" *)
  output s;
  wire s;
  (* module_not_derived = 32'd1 *)
  (* src = "/opt/homebrew/bin/../share/yosys/xilinx/lut_map.v:43.26-44.30" *)
  LUT2 #(
   .INIT(4'h1)
  ) _07_ (
    .I0(_04_[0]),
    .I1(_04_[1]),
    .O(_05_[4])
  );
endmodule