/*
  Focus on assign test
*/
module test(\asqrt[33] );
  wire n0, n1;
  wire [3:0] n2;

  assign n0 = n1;
  assign n1 = 1'b0;
  assign { n2[3:2], n2[1:0] } = { n0, n1, 2'h2 };
endmodule