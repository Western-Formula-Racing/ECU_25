#include "unity.h"
#include "CAN.h"
#include <unity.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "BMS.h"
#include "Inverter.h"
#include "Pedals.h"
#include "RearECU.h"
#include "IO.h"
#include "Sensors.h"

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

void test_BMS(void){
    // make sure singleton works correctly
    auto testAddress0 = BMS::Get();
    auto testAddress1 = BMS::Get();
    TEST_ASSERT_EQUAL (testAddress0, testAddress1);
}
void test_Inverter(void){
    // make sure singleton works correctly
    auto testAddress0 = Inverter::Get();
    auto testAddress1 = Inverter::Get();
    TEST_ASSERT_EQUAL (testAddress0, testAddress1);
}
void test_Pedals(void){
    // make sure singleton works correctly
    auto testAddress0 = Pedals::Get();
    auto testAddress1 = Pedals::Get();
    TEST_ASSERT_EQUAL (testAddress0, testAddress1);
}
void test_RearECU(void){
    // make sure singleton works correctly
    auto testAddress0 = RearECU::Get();
    auto testAddress1 = RearECU::Get();
    TEST_ASSERT_EQUAL (testAddress0, testAddress1);
}
void test_IO(void){
    // make sure singleton works correctly
    auto testAddress0 = IO::Get();
    auto testAddress1 = IO::Get();
    TEST_ASSERT_EQUAL (testAddress0, testAddress1);
}
void test_Sensors(void){
    // make sure singleton works correctly
    auto testAddress0 = Sensors::Get();
    auto testAddress1 = Sensors::Get();
    TEST_ASSERT_EQUAL (testAddress0, testAddress1);
}
extern "C" void app_main()
{
    UNITY_BEGIN();
    vTaskDelay(500);
    RUN_TEST(test_testing);
    RUN_TEST(test_BMS);
    RUN_TEST(test_Inverter);
    RUN_TEST(test_Pedals);
    RUN_TEST(test_RearECU);
    RUN_TEST(test_RearECU);
    RUN_TEST(test_IO);
    RUN_TEST(test_Sensors);

    UNITY_END();
}