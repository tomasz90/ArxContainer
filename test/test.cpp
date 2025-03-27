#define UNITY_OUTPUT_CHAR(a) Serial.write(a)
#include <Arduino.h>
#include <unity.h>

void test() {
    TEST_ASSERT(true);
}
void setup() {
    Serial.begin(BAUD_RATE);

    while (!Serial);
    delay(1000);

    UNITY_BEGIN();

    Serial.println("\n=== Running Set Tests ===");

    RUN_TEST(test);

    UNITY_END();
}

void loop() {}