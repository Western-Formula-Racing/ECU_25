#include "unity.h"
#include "CAN.h"

TEST_CASE("Return 0", "[CAN]")
{
    TEST_ASSERT_EQUAL(0, get_0());
}
