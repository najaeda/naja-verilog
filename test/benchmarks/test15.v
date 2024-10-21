/*
  test more attributes (pragmas)
*/
(* test *)
module test();
  (* test, a *)
  wire [7:0] a;
  (* test, b *)
  wire [7:0] b;
  (* test, s *)
  wire s;
  (* test, adder_inst *)
  adder adder_inst (
    .a(a),
    .b(b),
    .s(s)
  );
endmodule