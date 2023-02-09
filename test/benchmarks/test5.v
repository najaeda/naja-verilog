module test(\asqrt[33] );
  output \asqrt[33] ;
  wire \asqrt[33] ;

  wire _0746_;
  wire _0747_;
  wire _0748_; 

  (* option1 = 32'd1 *)
  (* src = "/path1/path2/test.v:7.24-7.93" *)
  LUT4 #(
    .INIT(8'h2b)
  ) _4370_ (
    .I0(_0746_),
    .I1(_0747_),
    .I2(\asqrt[33] ),
    .I3(1'h0),
    .Q(_0748_)
  );

endmodule