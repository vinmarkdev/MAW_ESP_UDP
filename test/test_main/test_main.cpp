#include <Arduino.h>
#include <unity.h>

extern const char *AP_SSID;
extern const char *AP_PASS;
extern const int LED_PIN;
extern const uint16_t UDP_PORT;

void test_constants_are_expected() {
    TEST_ASSERT_EQUAL_STRING("AA_Link", AP_SSID);
    TEST_ASSERT_EQUAL_STRING("TroLoLo_AA", AP_PASS);
    TEST_ASSERT_EQUAL_INT(LED_BUILTIN, LED_PIN);
    TEST_ASSERT_EQUAL_UINT16(14550, UDP_PORT);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_constants_are_expected);
    UNITY_END();
}

void loop() {
    // nothing to do here for unit test
}
