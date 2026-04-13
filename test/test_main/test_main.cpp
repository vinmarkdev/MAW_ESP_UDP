#include <Arduino.h>
#include <unity.h>
#include "config.h"

extern const int LED_PIN;
extern const uint16_t TCP_PORT;

void test_constants_are_expected() {
    TEST_ASSERT_EQUAL_STRING("AA_Link_00010", AP_SSID);
    TEST_ASSERT_EQUAL_STRING("TroLoLo_AA", AP_PASS);
    TEST_ASSERT_EQUAL_INT(LED_BUILTIN, LED_PIN);
    TEST_ASSERT_EQUAL_UINT16(14550, TCP_PORT);
    TEST_ASSERT_TRUE(AP_IP == IPAddress(192, 168, 4, 1));
    TEST_ASSERT_TRUE(AP_NETMASK == IPAddress(255, 255, 255, 0));
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_constants_are_expected);
    UNITY_END();
}

void loop() {
    // nothing to do here for unit test
}
