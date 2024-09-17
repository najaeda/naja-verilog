// defparam test
//

module test12 ();
wire f_we;
wire f_we_i;

CFG1 ins1 (
  .A(f_we),
  .Y(f_we_i)
);
defparam ins1.INIT=2'h1;

RAM64x18 mem_regfile_mem_regfile_0_0 ();
defparam mem_regfile_mem_regfile_0_0.RAMINDEX="mem_regfile[7:0]%32%8%SPEED%0%0%MICRO_RAM";

CFG1 \$$ins2@@ (
  .A(f_we_i),
  .Y(f_we)
);
defparam \$$ins2@@ .INIT=2'h2;


endmodule /* test12 */