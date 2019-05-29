#include "multiphysics.hpp"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

// #include "QUnit.hpp"
#include <iostream>


TEST_GROUP(idx_tests)
{
};

TEST(idx_tests, FirstTest)
{
   FAIL("Fail me!");
}

int main(int ac, char** av){
  return RUN_ALL_TESTS(ac, av);

    // int mesh_geometry[3];
    // QUNIT_IS_EQUAL(42, 41);

    // return qunit.errors();
}
