//Testing escape identifiers

module \mod% ();
  output[40:0] \asqrt ;
  wire \asqrt[33] ;

  //parameter \mypar^  = 32'd1; FIXME: need to support parameters

  \$$MOD  #(
    .INIT(8'h2b)
  ) \ins@2  (
    .I0(\busa+index ),
    .I1(\-clock ),
    .I2(\asqrt[33] ),
    .I3(\asqrt [33]),
    .Q(\{a,b} )
  );

endmodule