#include "gtest/gtest.h"

#include <filesystem>
#include <fstream>

#include "VerilogConstructor.h"

using namespace naja::verilog;

#include "VerilogConstructorTest.h"

#ifndef NAJA_VERILOG_BENCHMARKS
#define NAJA_VERILOG_BENCHMARKS "Undefined"
#endif

TEST(NajaVerilogTest3, test0) {
  VerilogConstructorTest constructor;
  std::filesystem::path test3Path(
      std::filesystem::path(NAJA_VERILOG_BENCHMARKS)
      / std::filesystem::path("benchmarks")
      / std::filesystem::path("test3.v"));
  constructor.parse(test3Path);
  EXPECT_EQ(2, constructor.modules_.size());
  auto mod0 = constructor.modules_[0];
  EXPECT_EQ("mod0", mod0.name_);
  EXPECT_EQ(2, mod0.ports_.size());
  EXPECT_EQ("i0", mod0.ports_[0].name_);
  EXPECT_EQ("o0", mod0.ports_[1].name_);
  EXPECT_EQ(naja::verilog::Port::Direction::Input, mod0.ports_[0].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, mod0.ports_[1].direction_);
  auto test = constructor.modules_[1];
  EXPECT_EQ("test", constructor.modules_[1].name_);
  EXPECT_EQ(3, test.ports_.size());
  EXPECT_EQ("i", test.ports_[0].name_);
  EXPECT_EQ("o", test.ports_[1].name_);
  EXPECT_EQ("io", test.ports_[2].name_);
  EXPECT_EQ(naja::verilog::Port::Direction::Input, test.ports_[0].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::Output, test.ports_[1].direction_);
  EXPECT_EQ(naja::verilog::Port::Direction::InOut, test.ports_[2].direction_);
}

#if 0
TEST_F(SNLVRLDumperTest1, test0) {
  auto lib = db_->getLibrary(SNLName("MYLIB"));  
  ASSERT_TRUE(lib);
  auto top = lib->getDesign(SNLName("top"));
  ASSERT_TRUE(top);

  SNLInstance* instance1 = top->getInstance(SNLName("instance1"));
  ASSERT_TRUE(instance1);
  SNLInstance* instance2 = top->getInstance(SNLName("instance2"));
  ASSERT_TRUE(instance2);
  SNLNet* bus0 = top->getNet(SNLName("bus0"));
  ASSERT_TRUE(bus0);
  SNLNet* bus1 = top->getNet(SNLName("bus1"));
  ASSERT_TRUE(bus1);

  //connect instance1.o0 to instance2.i0 with bus0
  instance1->setTermNet(instance1->getModel()->getTerm(SNLName("o0")), bus0);
  instance2->setTermNet(instance2->getModel()->getTerm(SNLName("i0")), bus0);

  //connect instance1.o1 to instance2.i1 with bus1
  instance1->setTermNet(instance1->getModel()->getTerm(SNLName("o1")), bus1);
  instance2->setTermNet(instance2->getModel()->getTerm(SNLName("i1")), bus1);

  std::filesystem::path outPath(SNL_VRL_DUMPER_TEST_PATH);
  outPath = outPath / "test1Test0";
  if (std::filesystem::exists(outPath)) {
    std::filesystem::remove_all(outPath);
  }
  std::filesystem::create_directory(outPath);
  SNLVRLDumper dumper;
  dumper.setTopFileName(top->getName().getString());
  dumper.setSingleFile(true);
  dumper.dumpDesign(top, outPath);

  std::filesystem::path referencePath(SNL_VRL_DUMPER_REFERENCES_PATH);
  referencePath = referencePath / "test1Test0" / "top.v";
  ASSERT_TRUE(std::filesystem::exists(referencePath));
  std::string command = "diff " + outPath.string() + " " + referencePath.string();
  EXPECT_FALSE(std::system(command.c_str()));
}

TEST_F(SNLVRLDumperTest1, test1) {
  auto lib = db_->getLibrary(SNLName("MYLIB"));  
  ASSERT_TRUE(lib);
  auto top = lib->getDesign(SNLName("top"));
  ASSERT_TRUE(top);

  SNLInstance* instance1 = top->getInstance(SNLName("instance1"));
  ASSERT_TRUE(instance1);
  SNLInstance* instance2 = top->getInstance(SNLName("instance2"));
  ASSERT_TRUE(instance2);
  SNLNet* bus0 = top->getNet(SNLName("bus0"));
  ASSERT_TRUE(bus0);
  SNLNet* bus1 = top->getNet(SNLName("bus1"));
  ASSERT_TRUE(bus1);

  //connect instance1.o0 to instance2.i0 with bus0
  instance1->setTermNet(instance1->getModel()->getTerm(SNLName("o1")), bus0);
  instance2->setTermNet(instance2->getModel()->getTerm(SNLName("i1")), bus0);

  //connect instance1.o1 to instance2.i1 with bus1
  instance1->setTermNet(instance1->getModel()->getTerm(SNLName("o0")), bus1);
  instance2->setTermNet(instance2->getModel()->getTerm(SNLName("i0")), bus1);

  std::filesystem::path outPath(SNL_VRL_DUMPER_TEST_PATH);
  outPath = outPath / "test1Test1";
  if (std::filesystem::exists(outPath)) {
    std::filesystem::remove_all(outPath);
  }
  std::filesystem::create_directory(outPath);
  SNLVRLDumper dumper;
  dumper.setTopFileName(top->getName().getString());
  dumper.setSingleFile(true);
  dumper.dumpDesign(top, outPath);

  std::filesystem::path referencePath(SNL_VRL_DUMPER_REFERENCES_PATH);
  referencePath = referencePath / "test1Test1" / "top.v";
  ASSERT_TRUE(std::filesystem::exists(referencePath));
  std::string command = "diff " + outPath.string() + " " + referencePath.string();
  EXPECT_FALSE(std::system(command.c_str()));
}

//split busses in 2
TEST_F(SNLVRLDumperTest1, test2) {
  auto lib = db_->getLibrary(SNLName("MYLIB"));  
  ASSERT_TRUE(lib);
  auto top = lib->getDesign(SNLName("top"));
  ASSERT_TRUE(top);

  SNLInstance* instance1 = top->getInstance(SNLName("instance1"));
  ASSERT_TRUE(instance1);
  SNLInstance* instance2 = top->getInstance(SNLName("instance2"));
  ASSERT_TRUE(instance2);
  SNLNet* bus0 = top->getNet(SNLName("bus0"));
  ASSERT_TRUE(bus0);
  SNLNet* bus2 = top->getNet(SNLName("bus2"));
  ASSERT_TRUE(bus2);

  //connect instance1.o0 to instance2.i0 with bus0
  //instance1->getInstTerm(instance1->getModel()->getBusTerm());
  instance1->setTermNet(instance1->getModel()->getTerm(SNLName("o0")), 2, 0, bus0, 2, 0);
  instance1->setTermNet(instance1->getModel()->getTerm(SNLName("o0")), -1, -2, bus2, -1, -2);
  instance2->setTermNet(instance2->getModel()->getTerm(SNLName("i0")), 2, 0, bus0, 2, 0);
  instance2->setTermNet(instance2->getModel()->getTerm(SNLName("i0")), -1, -2, bus2, -1, -2);

  std::filesystem::path outPath(SNL_VRL_DUMPER_TEST_PATH);
  outPath = outPath / "test1Test2";
  if (std::filesystem::exists(outPath)) {
    std::filesystem::remove_all(outPath);
  }
  std::filesystem::create_directory(outPath);
  SNLVRLDumper dumper;
  dumper.setTopFileName(top->getName().getString());
  dumper.setSingleFile(true);
  dumper.dumpDesign(top, outPath);

  std::filesystem::path referencePath(SNL_VRL_DUMPER_REFERENCES_PATH);
  referencePath = referencePath / "test1Test2" / "top.v";
  ASSERT_TRUE(std::filesystem::exists(referencePath));
  std::string command = "diff " + outPath.string() + " " + referencePath.string();
  EXPECT_FALSE(std::system(command.c_str()));
}

//connect with scalars concatenation
TEST_F(SNLVRLDumperTest1, test3) {
  auto lib = db_->getLibrary(SNLName("MYLIB"));  
  ASSERT_TRUE(lib);
  auto top = lib->getDesign(SNLName("top"));
  ASSERT_TRUE(top);

  SNLInstance* instance1 = top->getInstance(SNLName("instance1"));
  ASSERT_TRUE(instance1);
  SNLInstance* instance2 = top->getInstance(SNLName("instance2"));
  ASSERT_NE(nullptr, instance2);
  using Nets = std::vector<SNLBitNet*>;
  Nets nets;
  for (int i=0; i<5; ++i) {
    std::string netName = "net_" + std::to_string(i);
    nets.push_back(SNLScalarNet::create(top, SNLName(netName)));
  }
  using Terms = std::vector<SNLBitTerm*>;
  auto model = instance1->getModel();
  auto o0BusTerm = model->getBusTerm(SNLName("o0"));
  ASSERT_NE(nullptr, o0BusTerm);
  Terms terms(o0BusTerm->getBits().begin(), o0BusTerm->getBits().end());
  instance1->setTermsNets(terms, nets);

  std::filesystem::path outPath(SNL_VRL_DUMPER_TEST_PATH);
  outPath = outPath / "test1Test3";
  if (std::filesystem::exists(outPath)) {
    std::filesystem::remove_all(outPath);
  }
  std::filesystem::create_directory(outPath);
  SNLVRLDumper dumper;
  dumper.setTopFileName(top->getName().getString());
  dumper.setSingleFile(true);
  dumper.dumpDesign(top, outPath);

  std::filesystem::path referencePath(SNL_VRL_DUMPER_REFERENCES_PATH);
  referencePath = referencePath / "test1Test3" / "top.v";
  ASSERT_TRUE(std::filesystem::exists(referencePath));
  std::string command = "diff " + outPath.string() + " " + referencePath.string();
  EXPECT_FALSE(std::system(command.c_str()));
}

//mix scalars with bus subrange
TEST_F(SNLVRLDumperTest1, test4) {
  auto lib = db_->getLibrary(SNLName("MYLIB"));  
  ASSERT_TRUE(lib);
  auto top = lib->getDesign(SNLName("top"));
  ASSERT_TRUE(top);
  SNLBusNet* bus0 = top->getBusNet(SNLName("bus0"));
  ASSERT_TRUE(bus0);

  SNLInstance* instance1 = top->getInstance(SNLName("instance1"));
  ASSERT_TRUE(instance1);
  SNLInstance* instance2 = top->getInstance(SNLName("instance2"));
  ASSERT_NE(nullptr, instance2);
  using Nets = std::vector<SNLBitNet*>;
  Nets nets;
  nets.push_back(SNLScalarNet::create(top, SNLName("net_0")));
  nets.push_back(bus0->getBit(-2));
  nets.push_back(SNLScalarNet::create(top, SNLName("net_2")));
  nets.push_back(bus0->getBit(-1));
  nets.push_back(bus0->getBit(0));

  using Terms = std::vector<SNLBitTerm*>;
  auto model = instance1->getModel();
  auto o0BusTerm = model->getBusTerm(SNLName("o0"));
  ASSERT_NE(nullptr, o0BusTerm);
  Terms terms(o0BusTerm->getBits().begin(), o0BusTerm->getBits().end());
  instance1->setTermsNets(terms, nets);

  std::filesystem::path outPath(SNL_VRL_DUMPER_TEST_PATH);
  outPath = outPath / "test1Test4";
  if (std::filesystem::exists(outPath)) {
    std::filesystem::remove_all(outPath);
  }
  std::filesystem::create_directory(outPath);
  SNLVRLDumper dumper;
  dumper.setTopFileName(top->getName().getString());
  dumper.setSingleFile(true);
  dumper.dumpDesign(top, outPath);

  std::filesystem::path referencePath(SNL_VRL_DUMPER_REFERENCES_PATH);
  referencePath = referencePath / "test1Test4" / "top.v";
  ASSERT_TRUE(std::filesystem::exists(referencePath));
  std::string command = "diff " + outPath.string() + " " + referencePath.string();
  EXPECT_FALSE(std::system(command.c_str()));
}

//mix scalars with bus subrange and holes
TEST_F(SNLVRLDumperTest1, test5) {
  auto lib = db_->getLibrary(SNLName("MYLIB"));  
  ASSERT_TRUE(lib);
  auto top = lib->getDesign(SNLName("top"));
  ASSERT_TRUE(top);
  SNLBusNet* bus0 = top->getBusNet(SNLName("bus0"));
  ASSERT_TRUE(bus0);

  SNLInstance* instance1 = top->getInstance(SNLName("instance1"));
  ASSERT_TRUE(instance1);
  SNLInstance* instance2 = top->getInstance(SNLName("instance2"));
  ASSERT_NE(nullptr, instance2);

  auto model = instance1->getModel();
  auto o0BusTerm = model->getBusTerm(SNLName("o0"));
  ASSERT_NE(nullptr, o0BusTerm);
  instance1->setTermNet(o0BusTerm, -2, -2, SNLScalarNet::create(top, SNLName("net_0")), 0, 0);
  instance1->setTermNet(o0BusTerm, 1, 2, bus0, -1, 0);

  std::filesystem::path outPath(SNL_VRL_DUMPER_TEST_PATH);
  outPath = outPath / "test1Test5";
  if (std::filesystem::exists(outPath)) {
    std::filesystem::remove_all(outPath);
  }
  std::filesystem::create_directory(outPath);
  SNLVRLDumper dumper;
  dumper.setTopFileName(top->getName().getString());
  dumper.setSingleFile(true);
  dumper.dumpDesign(top, outPath);

  std::filesystem::path referencePath(SNL_VRL_DUMPER_REFERENCES_PATH);
  referencePath = referencePath / "test1Test5" / "top.v";
  ASSERT_TRUE(std::filesystem::exists(referencePath));
  std::string command = "diff " + outPath.string() + " " + referencePath.string();
  EXPECT_FALSE(std::system(command.c_str()));
}
#endif
