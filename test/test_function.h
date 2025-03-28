#include <unity.h>
#include "ArxContainer.h"
#include "Function.h"

void test_function_without_capture() {
    static bool called = false;

    arx::stdx::function<void()> f = []() {
        called = true;
    };

    f();
    TEST_ASSERT_TRUE_MESSAGE(called, "Function was not invoked");
}

void test_function_with_capture() {
    bool called = false;
    arx::stdx::function<void()> f = [&called]() { called = true; };

    f();

    TEST_ASSERT_MESSAGE(called, "function was not called");
}

void test_function_with_arg() {
    int expectedArgument = 10;

    arx::stdx::function<void(int)> f = [&expectedArgument](int argument) {
        TEST_ASSERT_EQUAL_INT(expectedArgument, argument);
    };

    f(expectedArgument);
}

struct TestClass {
    String name;
    int value;
};

void test_function_with_arg_struct() {
    auto testObj = TestClass{"testName", 10};

    arx::stdx::function<void(TestClass&)> f = [](TestClass &argument) {
        argument.name = "newTestName";
        argument.value = 20;
    };

    f(testObj);

    TEST_ASSERT_TRUE(testObj.name == "newTestName");
    TEST_ASSERT_EQUAL_INT(20, testObj.value);
}

void test_function_with_return() {
    arx::stdx::function<int()> f = []() { return 42; };
    TEST_ASSERT_EQUAL_INT(42, f());
}

void test_function_with_return_and_capture() {
    int expectedReturn = 10;

    arx::stdx::function<int()> f = [&expectedReturn]() {
        return expectedReturn;
    };

    TEST_ASSERT_EQUAL_INT(expectedReturn, f());
}

void test_function_with_state_preservation() {
    int counter = 0;
    arx::stdx::function<void()> f = [&counter]() { counter++; };

    f();
    f();

    TEST_ASSERT_EQUAL(2, counter);
}

void test_function_with_multiple_arguments() {
    arx::stdx::function<int(int, int)> multiple = [](int a, int b) { return a * b; };
    TEST_ASSERT_EQUAL(20, multiple(4, 5));
}


void test_function_with_reference_argument() {
    int value = 0;
    arx::stdx::function<void(int &)> setValue = [](int &x) { x = 100; };
    setValue(value);
    TEST_ASSERT_EQUAL(100, value);
}


void test_function_with_move_constructor() {
    bool moved = false;
    arx::stdx::function<void()> f1 = [&moved]() { moved = true; };
    arx::stdx::function<void()> f2 = arx::stdx::move(f1);

    f2();
    TEST_ASSERT_TRUE(moved);
    TEST_ASSERT_FALSE(static_cast<bool>(f1));
}

static int static_func(int x) { return x * 2; }

void test_function_with_pointer() {
    arx::stdx::function<int(int)> f = static_func;
    TEST_ASSERT_EQUAL(10, f(5));
}

struct Functor {
    int operator()(int x) const { return x + 5; }
};

void test_function_functor_support() {
    arx::stdx::function<int(int)> f = Functor();
    TEST_ASSERT_EQUAL(8, f(3));
}

void test_function_reassignment() {
    bool firstCalled = false;
    bool secondCalled = false;

    arx::stdx::function<void()> f = [&firstCalled]() { firstCalled = true; };
    f = [&secondCalled]() { secondCalled = true; };

    f();

    TEST_ASSERT_FALSE(firstCalled);
    TEST_ASSERT_TRUE(secondCalled);
}

void test_function_clear() {
    bool called = false;
    arx::stdx::function<void()> f = [&called]() { called = true; };
    f = nullptr;

    TEST_ASSERT_FALSE(static_cast<bool>(f));
    TEST_ASSERT_FALSE(called); // Shouldn't be called after clear
}

void test_function_operator_nullptr() {
    arx::stdx::function<void()> f = []() {};

    TEST_ASSERT_TRUE(f != nullptr);

    f = nullptr;

    TEST_ASSERT_TRUE(f == nullptr);
}

void test_function_operator_bool() {
    arx::stdx::function<void()> f = []() {};

    TEST_ASSERT_TRUE(f);

    f = nullptr;

    TEST_ASSERT_FALSE(f);
}
