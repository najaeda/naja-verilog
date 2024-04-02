//multiple ports on the same line or multi-line

module test(i1, \i2% , i3, o1, o2, io1, io2);
  input i1, \i2% , i3;
  output [3:0] o1,
   o2;
  inout io1, io2;
endmodule