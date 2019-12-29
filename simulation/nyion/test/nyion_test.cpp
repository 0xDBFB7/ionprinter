#include "nyion.hpp"
// #include "visualize.hpp"
//

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(FirstTestGroup)
{
};

TEST(FirstTestGroup, FirstTest)
{
    testah();
   FAIL("Fail me!");
}



int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}
