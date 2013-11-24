/*
 * This file runs all the tests. Make sure you include the test you want to run
 * below
 */
#include "gtest/gtest.h"

// Tests 
#include "SampleTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
