#include <unity.h>
#include <Arduino.h>

void test() {
    TEST_ASSERT(true);
}
void setup() {
    Serial.begin(9600);

    UNITY_BEGIN();

    Serial.println("\n=== Running Set Tests ===");

    RUN_TEST(test);

    UNITY_END();
}

void loop() {}