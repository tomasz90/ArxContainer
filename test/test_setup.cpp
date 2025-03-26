#include <unity.h>
#include <Arduino.h>
#include <set>
#include <ArxContainer.h>

// TO RUN TESTS: `pio test -vvv`
arx::stdx::set<int, 5> customSet;   // Test with capacity 5
std::set<int> referenceSet;         // For comparison

// Test function prototypes
void testInsertAndExist();
void testErase();
void testDuplicateInsert();
void testSetComparisons();
void testCapacityLimit();

template<size_t N>
void ASSERT_ORDER(const arx::stdx::set<int, N>& set);

void setup() {
    Serial.begin(115200);

    UNITY_BEGIN();

    RUN_TEST(testInsertAndExist);
    RUN_TEST(testErase);
    RUN_TEST(testDuplicateInsert);
    RUN_TEST(testSetComparisons);
    RUN_TEST(testCapacityLimit);

    UNITY_END();
}

void loop() {}

void setUp() {
    customSet.clear();
    referenceSet.clear();
}

void tearDown() {}

void testInsertAndExist() {
    Serial.println("\n=== Testing Insert/Exist ===");

    customSet.insert(3);
    customSet.insert(1);
    customSet.insert(4);

    TEST_ASSERT_MESSAGE(customSet.exist(1), "1 should exist");
    TEST_ASSERT_MESSAGE(customSet.exist(3), "3 should exist");
    TEST_ASSERT_MESSAGE(customSet.exist(4), "4 should exist");

    ASSERT_ORDER(customSet);
}

void testErase() {
    Serial.println("\n=== Testing Erase ===");

    customSet.insert(3);
    customSet.insert(1);
    customSet.insert(4);

    TEST_ASSERT_MESSAGE(customSet.erase(3), "Should erase 3");
    TEST_ASSERT_MESSAGE(!customSet.exist(3), "3 should be removed");
    TEST_ASSERT_MESSAGE(customSet.size() == 2, "Size should be 2");

    TEST_ASSERT_MESSAGE(!customSet.erase(9), "Shouldn't erase non-existent");

    ASSERT_ORDER(customSet);
}

void testDuplicateInsert() {
    Serial.println("\n=== Testing Duplicates ===");

    customSet.insert(1);
    customSet.insert(4);
    size_t initialSize = customSet.size();

    customSet.insert(1); // Duplicate
    customSet.insert(4); // Duplicate

    TEST_ASSERT_MESSAGE(customSet.size() == initialSize, "Size shouldn't change");

    ASSERT_ORDER(customSet);
}

void testSetComparisons() {
    Serial.println("\n=== Testing Comparisons ===");

    arx::stdx::set<int, 5> set1 {1, 2, 3};
    arx::stdx::set<int, 5> set2 {3, 2, 1};
    arx::stdx::set<int, 5> set3 {1, 2};

    TEST_ASSERT_MESSAGE(set1 == set1, "Set should equal itself");
    TEST_ASSERT_MESSAGE(set1 == set2, "Different order should be equal");
    TEST_ASSERT_MESSAGE(set1 != set3, "Different sizes should be unequal");

    TEST_ASSERT_MESSAGE(set3 < set1, "{1,2} < {1,2,3}");
}

void testCapacityLimit() {
    Serial.println("\n=== Testing Capacity ===");

    arx::stdx::set<int, 3> smallSet;
    smallSet.insert(1);
    smallSet.insert(2);
    smallSet.insert(3);
    smallSet.insert(4); // Should be ignored

    TEST_ASSERT_MESSAGE(smallSet.size() == 3, "Should respect capacity");
    TEST_ASSERT_MESSAGE(!smallSet.exist(4), "4 shouldn't be inserted");

    ASSERT_ORDER(customSet);
}

template<size_t N>
void ASSERT_ORDER(const arx::stdx::set<int, N>& set) {
    int last = -10000;
    for(int e : set) {
        TEST_ASSERT_MESSAGE(last < e, "Set should be ordered");
        last = e;
    }
}