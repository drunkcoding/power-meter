#include <thread>
#include <chrono>

#include "gpu_meter.h"
#include "logging.h"
#include "gtest/gtest.h"

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  FooTest() {
    // You can do set-up work for each test here.
  }

  ~FooTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
    Logger::setLogLevel(Logger::LogLevel::DEBUG);
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// // Tests that the Foo::Bar() method does Abc.
// TEST_F(FooTest, MethodBarDoesAbc) {
//   const std::string input_filepath = "this/package/testdata/myinputfile.dat";
//   const std::string output_filepath =
//   "this/package/testdata/myoutputfile.dat"; Foo f;
//   EXPECT_EQ(f.Bar(input_filepath, output_filepath), 0);
// }

// Tests that Foo does Xyz.
TEST_F(FooTest, GPUMeterSanity) {
  // Exercises the Xyz feature of Foo.
  GPU_METER->startMonitor();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  GPU_METER->stopMonitor();
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }