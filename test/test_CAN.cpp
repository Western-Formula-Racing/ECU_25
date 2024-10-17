#include "unity.h"
#include "CAN.h"
#include <unity.h>

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_testing(void)
{
  TEST_ASSERT_EQUAL(0, give_zero());
}



extern "C" void app_main()
{
  UNITY_BEGIN();

  RUN_TEST(test_testing);

  UNITY_END();
}