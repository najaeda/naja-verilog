/*
  test17: Test "generic RTL" uncovered gates.
*/

module test17(IN1, IN2, OUT);
   input IN1, IN2;
   output OUT;

   nand (wire0, IN1, IN2);
   nor (wire1, IN1, IN2);
   xnor (OUT, wire0, wire1);
 
endmodule