#include <unity.h>
#include <Arduino.h>
#include "test_set.h"
#include "test_function.h"

void setup() {
    Serial.begin(115200);

    UNITY_BEGIN();

    Serial.println("\n=== Running Set Tests ===");

    RUN_TEST(test_set_insert);
    RUN_TEST(test_set_insert_after_clear);
    RUN_TEST(test_set_erase);
    RUN_TEST(test_set_erase_boundaries);
    RUN_TEST(test_set_clear);
    RUN_TEST(test_set_duplicate_insert);
    RUN_TEST(test_set_comparisons);
    RUN_TEST(test_set_capacity_limit);
    RUN_TEST(test_set_copy_constructor);

    Serial.println("\n=== Running Function Tests ===");

    RUN_TEST(test_function_without_capture);
    RUN_TEST(test_function_with_capture);
    RUN_TEST(test_function_with_arg);
    RUN_TEST(test_function_with_return);
    RUN_TEST(test_function_with_return_and_capture);
    RUN_TEST(test_function_with_state_preservation);
    RUN_TEST(test_function_with_multiple_arguments);
    RUN_TEST(test_function_with_reference_argument);
    RUN_TEST(test_function_with_move_constructor);
    RUN_TEST(test_function_with_pointer);
    RUN_TEST(test_function_functor_support);
    RUN_TEST(test_function_with_arg_struct);
    RUN_TEST(test_function_reassignment);
    RUN_TEST(test_function_clear);
    RUN_TEST(test_function_operator_nullptr);
    RUN_TEST(test_function_operator_bool);

    UNITY_END();
}

void loop() {}