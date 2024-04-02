//multiple ports on the same line or multi-line

module test(i1, \i2% , i3, o1, o2, io1, io2);
  input i1, \i2% , i3;
  output [3:0] o1,
   o2;
  inout io1, io2;

  wire macu_n_280, macu_n_281, macu_n_282, \macu_result[0]_203 ,
    \macu_result[1]_204 , \macu_result[2]_205 , \macu_result[3]_206
    , \macu_result[4]_207 ;
endmodule