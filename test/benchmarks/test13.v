// defparam with escape ids
//

module ins_decode (
  k,
  inst,
  inst_fast_7,
  inst_fast_8,
  inst_fast_0,
  inst_fast_1,
  aluop,
  alub_sel,
  alua_sel,
  pc_sel28_3,
  pc_sel29_2,
  N_23,
  N_92,
  aluz,
  N_168,
  N_24,
  bdpol,
  status_c_we,
  status_z_we,
  tris_we,
  w_we,
  pc_sel28_3_i,
  clock_c,
  resetn_c,
  skip_1z,
  f_we_i,
  f_we
)
;
input [4:0] k ;
input [11:5] inst ;
input inst_fast_7 ;
input inst_fast_8 ;
input inst_fast_0 ;
input inst_fast_1 ;
output [3:0] aluop ;
output [1:0] alub_sel ;
output [1:0] alua_sel ;
input pc_sel28_3 ;
input pc_sel29_2 ;
output N_23 ;
output N_92 ;
input aluz ;
output N_168 ;
output N_24 ;
output bdpol ;
output status_c_we ;
output status_z_we ;
output tris_we ;
output w_we ;
input pc_sel28_3_i ;
input clock_c ;
input resetn_c ;
output skip_1z ;
output f_we_i ;
output f_we ;
wire inst_fast_7 ;
wire inst_fast_8 ;
wire inst_fast_0 ;
wire inst_fast_1 ;
wire pc_sel28_3 ;
wire pc_sel29_2 ;
wire N_23 ;
wire N_92 ;
wire aluz ;
wire N_168 ;
wire N_24 ;
wire bdpol ;
wire status_c_we ;
wire status_z_we ;
wire tris_we ;
wire w_we ;
wire pc_sel28_3_i ;
wire clock_c ;
wire resetn_c ;
wire skip_1z ;
wire f_we_i ;
wire f_we ;
wire [12:3] decodes_in;
wire [7:7] decodes_in_0_0_1_0_Z;
wire [7:7] decodes_in_0_0_4_Z;
wire [7:7] decodes_in_0_0_a3_0_3;
wire [8:8] decodes_in_0_a2_i_0_Z;
wire [5:2] decodes_in_i_0_0_Z;
wire [5:5] decodes_in_i_0_a3_0;
wire [3:3] decodes_in_0_a2_0_a3_3_Z;
wire [2:2] decodes_in_i_0_1_Z;
wire [7:7] decodes_in_0_0_2_Z;
wire [11:11] decodes_in_0_0_0_Z;
wire [2:2] decodes_in_i_0_2_Z;
wire VCC ;
wire skip_in ;
wire GND ;
wire N_22_i ;
wire N_8_i ;
wire N_11_i ;
wire N_23_i ;
wire N_21_i ;
wire N_19_i ;
wire N_16_i ;
wire N_20_i ;
wire decodes_in94 ;
wire N_61 ;
wire N_67 ;
wire N_261 ;
wire N_38 ;
wire N_35_i ;
wire N_25 ;
wire N_34 ;
wire N_49 ;
wire N_73 ;
wire N_65 ;
wire N_262 ;
wire N_75 ;
wire N_84 ;
wire N_26 ;
wire N_40_i ;
wire N_60 ;
wire N_27 ;
wire N_33 ;
wire skip_in_0_0_Z ;
wire N_31 ;
wire N_74 ;
  CFG1 \decodes_RNIL3NV2[4]  (
	.A(f_we),
	.Y(f_we_i)
);
defparam \decodes_RNIL3NV2[4] .INIT=2'h1;
// @7:58
  SLE skip (
	.Q(skip_1z),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(skip_in),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[13]  (
	.Q(alua_sel[1]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(pc_sel28_3_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[12]  (
	.Q(alua_sel[0]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(decodes_in[12]),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[11]  (
	.Q(alub_sel[1]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(decodes_in[11]),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[10]  (
	.Q(alub_sel[0]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_22_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[9]  (
	.Q(aluop[3]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_8_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[8]  (
	.Q(aluop[2]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_11_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[7]  (
	.Q(aluop[1]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(decodes_in[7]),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[6]  (
	.Q(aluop[0]),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_23_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[5]  (
	.Q(w_we),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_21_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[4]  (
	.Q(f_we),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_19_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[3]  (
	.Q(tris_we),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(decodes_in[3]),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[2]  (
	.Q(status_z_we),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_16_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[1]  (
	.Q(status_c_we),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(N_20_i),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:58
  SLE \decodes[0]  (
	.Q(bdpol),
	.ADn(VCC),
	.ALn(resetn_c),
	.CLK(clock_c),
	.D(decodes_in94),
	.EN(VCC),
	.LAT(GND),
	.SD(GND),
	.SLn(VCC)
);
// @7:114
  CFG4 \decodes_in_0_0[7]  (
	.A(N_61),
	.B(N_67),
	.C(decodes_in_0_0_1_0_Z[7]),
	.D(decodes_in_0_0_4_Z[7]),
	.Y(decodes_in[7])
);
defparam \decodes_in_0_0[7] .INIT=16'hFFFE;
// @7:114
  CFG4 \decodes_in_0_0_1_0[7]  (
	.A(N_261),
	.B(decodes_in_0_0_a3_0_3[7]),
	.C(inst[8]),
	.D(N_24),
	.Y(decodes_in_0_0_1_0_Z[7])
);
defparam \decodes_in_0_0_1_0[7] .INIT=16'h5054;
// @7:114
  CFG2 \decodes_in_0_0_a3_3_1[7]  (
	.A(inst[6]),
	.B(inst[7]),
	.Y(N_168)
);
defparam \decodes_in_0_0_a3_3_1[7] .INIT=4'h4;
// @7:114
  CFG2 \decodes_in_0_a2_i_o3[8]  (
	.A(inst[9]),
	.B(inst[10]),
	.Y(N_38)
);
defparam \decodes_in_0_a2_i_o3[8] .INIT=4'hD;
// @7:78
  CFG2 skip_in_0_x2 (
	.A(inst[8]),
	.B(aluz),
	.Y(N_35_i)
);
defparam skip_in_0_x2.INIT=4'h6;
// @7:78
  CFG2 skip_in_0_a2 (
	.A(inst_fast_7),
	.B(inst_fast_8),
	.Y(N_92)
);
defparam skip_in_0_a2.INIT=4'h4;
// @7:114
  CFG2 \decodes_in_0_a2_0_o2[3]  (
	.A(k[1]),
	.B(k[0]),
	.Y(N_23)
);
defparam \decodes_in_0_a2_0_o2[3] .INIT=4'hE;
// @7:114
  CFG2 \decodes_in_i_0_o2[5]  (
	.A(inst[8]),
	.B(inst[9]),
	.Y(N_25)
);
defparam \decodes_in_i_0_o2[5] .INIT=4'hE;
// @7:114
  CFG2 \decodes_in_0_0_o2[11]  (
	.A(inst[9]),
	.B(inst[6]),
	.Y(N_261)
);
defparam \decodes_in_0_0_o2[11] .INIT=4'hE;
// @7:114
  CFG2 \decodes_in_i_0_o2_0[4]  (
	.A(inst[10]),
	.B(inst[5]),
	.Y(N_34)
);
defparam \decodes_in_i_0_o2_0[4] .INIT=4'hE;
// @7:114
  CFG2 \decodes_in_0_a2_i_o3[9]  (
	.A(inst[8]),
	.B(inst[7]),
	.Y(N_49)
);
defparam \decodes_in_0_a2_i_o3[9] .INIT=4'hB;
// @7:114
  CFG2 \decodes_in_0_a2_0_o2_0[3]  (
	.A(inst_fast_0),
	.B(inst_fast_1),
	.Y(N_24)
);
defparam \decodes_in_0_a2_0_o2_0[3] .INIT=4'hE;
// @7:114
  CFG3 \decodes_in_0_a2_i_0[8]  (
	.A(inst[8]),
	.B(inst[11]),
	.C(inst[6]),
	.Y(decodes_in_0_a2_i_0_Z[8])
);
defparam \decodes_in_0_a2_i_0[8] .INIT=8'hCD;
// @7:114
  CFG4 \decodes_in_i_0_a3[6]  (
	.A(inst[7]),
	.B(inst[6]),
	.C(inst[9]),
	.D(inst[8]),
	.Y(N_73)
);
defparam \decodes_in_i_0_a3[6] .INIT=16'h0900;
// @7:114
  CFG4 \decodes_in_0_0_a3_3[7]  (
	.A(inst[7]),
	.B(inst[10]),
	.C(inst[8]),
	.D(inst[6]),
	.Y(N_65)
);
defparam \decodes_in_0_0_a3_3[7] .INIT=16'h0020;
// @7:114
  CFG4 \decodes_in_0_0_a2[11]  (
	.A(inst[7]),
	.B(inst[10]),
	.C(inst[9]),
	.D(inst[6]),
	.Y(N_262)
);
defparam \decodes_in_0_0_a2[11] .INIT=16'h2000;
// @7:114
  CFG4 decodes_in94_0_a3 (
	.A(inst[11]),
	.B(inst[10]),
	.C(inst[9]),
	.D(inst[8]),
	.Y(decodes_in94)
);
defparam decodes_in94_0_a3.INIT=16'h0004;
// @7:114
  CFG3 \decodes_in_i_0_a3_1[6]  (
	.A(inst[9]),
	.B(inst[10]),
	.C(inst[6]),
	.Y(N_75)
);
defparam \decodes_in_i_0_a3_1[6] .INIT=8'h20;
// @7:114
  CFG3 \decodes_in_0_a2_0_a2[3]  (
	.A(inst[8]),
	.B(inst[7]),
	.C(inst[9]),
	.Y(N_84)
);
defparam \decodes_in_0_a2_0_a2[3] .INIT=8'h01;
// @7:78
  CFG3 skip_in_0_o2 (
	.A(k[4]),
	.B(k[3]),
	.C(k[2]),
	.Y(N_26)
);
defparam skip_in_0_o2.INIT=8'hFE;
// @7:114
  CFG3 \decodes_in_0_i_x2_0[1]  (
	.A(inst[8]),
	.B(inst[6]),
	.C(inst[9]),
	.Y(N_40_i)
);
defparam \decodes_in_0_i_x2_0[1] .INIT=8'h56;
// @7:114
  CFG4 \decodes_in_i_0_0[2]  (
	.A(inst[11]),
	.B(inst[10]),
	.C(inst[9]),
	.D(inst[8]),
	.Y(decodes_in_i_0_0_Z[2])
);
defparam \decodes_in_i_0_0[2] .INIT=16'h5444;
// @7:114
  CFG4 \decodes_in_i_0_0[5]  (
	.A(inst[11]),
	.B(inst[10]),
	.C(N_34),
	.D(N_25),
	.Y(decodes_in_i_0_0_Z[5])
);
defparam \decodes_in_i_0_0[5] .INIT=16'h7250;
// @7:114
  CFG4 \decodes_in_i_0_a3_0_0[5]  (
	.A(inst[9]),
	.B(inst[8]),
	.C(inst[11]),
	.D(inst[7]),
	.Y(decodes_in_i_0_a3_0[5])
);
defparam \decodes_in_i_0_a3_0_0[5] .INIT=16'h0001;
// @7:114
  CFG4 \decodes_in_0_a2_0_a3_3[3]  (
	.A(k[2]),
	.B(N_23),
	.C(inst[11]),
	.D(inst[6]),
	.Y(decodes_in_0_a2_0_a3_3_Z[3])
);
defparam \decodes_in_0_a2_0_a3_3[3] .INIT=16'h0008;
// @7:114
  CFG4 \decodes_in_0_0_a3_0_4[7]  (
	.A(k[2]),
	.B(N_23),
	.C(inst[10]),
	.D(inst[7]),
	.Y(decodes_in_0_0_a3_0_3[7])
);
defparam \decodes_in_0_0_a3_0_4[7] .INIT=16'h0008;
// @7:78
  CFG4 skip_in_0_a3_1 (
	.A(f_we),
	.B(k[0]),
	.C(k[1]),
	.D(N_26),
	.Y(N_60)
);
defparam skip_in_0_a3_1.INIT=16'h0020;
// @7:114
  CFG4 \decodes_in_0_0[12]  (
	.A(inst[11]),
	.B(inst[10]),
	.C(inst[9]),
	.D(N_49),
	.Y(decodes_in[12])
);
defparam \decodes_in_0_0[12] .INIT=16'h5455;
// @7:114
  CFG4 \decodes_in_0_0_a3[7]  (
	.A(inst[8]),
	.B(N_38),
	.C(inst[7]),
	.D(inst[6]),
	.Y(N_61)
);
defparam \decodes_in_0_0_a3[7] .INIT=16'h0201;
// @7:114
  CFG4 \decodes_in_i_0_o2_1[5]  (
	.A(k[2]),
	.B(N_24),
	.C(k[0]),
	.D(k[1]),
	.Y(N_27)
);
defparam \decodes_in_i_0_o2_1[5] .INIT=16'hFFFE;
// @7:114
  CFG4 \decodes_in_0_0_o3[7]  (
	.A(inst[11]),
	.B(inst[10]),
	.C(inst[9]),
	.D(inst[8]),
	.Y(N_33)
);
defparam \decodes_in_0_0_o3[7] .INIT=16'h2E2A;
// @7:114
  CFG4 \decodes_in_i_0_1[2]  (
	.A(decodes_in_i_0_0_Z[2]),
	.B(pc_sel29_2),
	.C(inst[11]),
	.D(inst[9]),
	.Y(decodes_in_i_0_1_Z[2])
);
defparam \decodes_in_i_0_1[2] .INIT=16'hAEAA;
// @7:114
  CFG4 \decodes_in_0_0_2[7]  (
	.A(inst[11]),
	.B(inst[8]),
	.C(N_65),
	.D(N_33),
	.Y(decodes_in_0_0_2_Z[7])
);
defparam \decodes_in_0_0_2[7] .INIT=16'hFFF8;
// @7:78
  CFG4 skip_in_0_0 (
	.A(inst[11]),
	.B(inst[10]),
	.C(inst[9]),
	.D(N_35_i),
	.Y(skip_in_0_0_Z)
);
defparam skip_in_0_0.INIT=16'h6222;
// @7:114
  CFG4 \decodes_in_0_0_0[11]  (
	.A(inst[10]),
	.B(N_261),
	.C(N_262),
	.D(N_49),
	.Y(decodes_in_0_0_0_Z[11])
);
defparam \decodes_in_0_0_0[11] .INIT=16'hF0F4;
// @7:58
  CFG4 \decodes_RNO[9]  (
	.A(pc_sel28_3),
	.B(N_49),
	.C(inst[6]),
	.D(inst[9]),
	.Y(N_8_i)
);
defparam \decodes_RNO[9] .INIT=16'h2022;
// @7:58
  CFG3 \decodes_RNO[8]  (
	.A(N_38),
	.B(pc_sel29_2),
	.C(decodes_in_0_a2_i_0_Z[8]),
	.Y(N_11_i)
);
defparam \decodes_RNO[8] .INIT=8'h01;
// @7:58
  CFG4 \decodes_RNO[4]  (
	.A(inst[10]),
	.B(inst[9]),
	.C(inst[11]),
	.D(inst[5]),
	.Y(N_19_i)
);
defparam \decodes_RNO[4] .INIT=16'h0702;
// @7:58
  CFG4 \decodes_RNO[1]  (
	.A(pc_sel28_3),
	.B(N_40_i),
	.C(inst[7]),
	.D(inst[9]),
	.Y(N_20_i)
);
defparam \decodes_RNO[1] .INIT=16'h0220;
// @7:114
  CFG4 \decodes_in_i_0_2[2]  (
	.A(inst[10]),
	.B(inst[11]),
	.C(decodes_in_i_0_1_Z[2]),
	.D(N_25),
	.Y(decodes_in_i_0_2_Z[2])
);
defparam \decodes_in_i_0_2[2] .INIT=16'hF4FC;
// @7:114
  CFG4 \decodes_in_0_0_4[7]  (
	.A(inst[5]),
	.B(inst[10]),
	.C(N_84),
	.D(decodes_in_0_0_2_Z[7]),
	.Y(decodes_in_0_0_4_Z[7])
);
defparam \decodes_in_0_0_4[7] .INIT=16'hFF20;
// @7:78
  CFG4 skip_in_0 (
	.A(skip_in_0_0_Z),
	.B(N_60),
	.C(aluz),
	.D(N_262),
	.Y(skip_in)
);
defparam skip_in_0.INIT=16'hFEEE;
// @7:114
  CFG4 \decodes_in_0_0_a3_5[7]  (
	.A(inst[10]),
	.B(inst[6]),
	.C(N_84),
	.D(N_27),
	.Y(N_67)
);
defparam \decodes_in_0_0_a3_5[7] .INIT=16'h0040;
// @7:114
  CFG4 \decodes_in_0_0[11]  (
	.A(inst[10]),
	.B(inst[11]),
	.C(decodes_in_0_0_0_Z[11]),
	.D(N_25),
	.Y(decodes_in[11])
);
defparam \decodes_in_0_0[11] .INIT=16'hF4FC;
// @7:114
  CFG4 \decodes_in_0_a2_0_a3[3]  (
	.A(N_24),
	.B(N_34),
	.C(N_84),
	.D(decodes_in_0_a2_0_a3_3_Z[3]),
	.Y(decodes_in[3])
);
defparam \decodes_in_0_a2_0_a3[3] .INIT=16'h1000;
// @7:114
  CFG4 \decodes_in_i_0_o2[2]  (
	.A(inst[6]),
	.B(inst[5]),
	.C(N_23),
	.D(N_26),
	.Y(N_31)
);
defparam \decodes_in_i_0_o2[2] .INIT=16'h7775;
// @7:58
  CFG4 \decodes_RNO[10]  (
	.A(pc_sel29_2),
	.B(N_25),
	.C(inst[11]),
	.D(inst[10]),
	.Y(N_22_i)
);
defparam \decodes_RNO[10] .INIT=16'h0F0E;
// @7:114
  CFG4 \decodes_in_i_0_a3_0[6]  (
	.A(inst[10]),
	.B(inst[7]),
	.C(inst[8]),
	.D(N_31),
	.Y(N_74)
);
defparam \decodes_in_i_0_a3_0[6] .INIT=16'h0504;
// @7:58
  CFG4 \decodes_RNO[5]  (
	.A(decodes_in_i_0_a3_0[5]),
	.B(inst[6]),
	.C(decodes_in_i_0_0_Z[5]),
	.D(N_27),
	.Y(N_21_i)
);
defparam \decodes_RNO[5] .INIT=16'h050D;
// @7:58
  CFG3 \decodes_RNO[2]  (
	.A(N_31),
	.B(decodes_in_i_0_2_Z[2]),
	.C(N_84),
	.Y(N_16_i)
);
defparam \decodes_RNO[2] .INIT=8'h13;
// @7:58
  CFG4 \decodes_RNO[6]  (
	.A(N_73),
	.B(N_75),
	.C(N_74),
	.D(N_33),
	.Y(N_23_i)
);
defparam \decodes_RNO[6] .INIT=16'h0001;
  GND GND_Z (
	.Y(GND)
);
  VCC VCC_Z (
	.Y(VCC)
);
endmodule /* ins_decode */
