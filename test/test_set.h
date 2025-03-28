#include <unity.h>
#if defined(ESP32)
#include <set>
#endif
#include "ArxContainer.h"

// TO RUN TESTS: `pio test -vvv`
arx::stdx::set<int, 5> customSet;   // Test with capacity 5
std::set<int> referenceSet;         // For comparison

template<size_t N>
void ASSERT_ORDER(const arx::stdx::set<int, N> &set);

void setUp() {
    customSet.clear();
    referenceSet.clear();
}

void tearDown() {}

void test_set_insert() {
    customSet.insert(3);
    customSet.insert(1);
    customSet.insert(4);

    TEST_ASSERT_MESSAGE(customSet.exist(1), "1 should exist");
    TEST_ASSERT_MESSAGE(customSet.exist(3), "3 should exist");
    TEST_ASSERT_MESSAGE(customSet.exist(4), "4 should exist");

    ASSERT_ORDER(customSet);
}

void test_set_insert_after_clear() {
    arx::stdx::set<int, 3> set;
    set.insert(1);
    set.insert(2);
    set.clear();

    TEST_ASSERT(set.empty());
    TEST_ASSERT(set.insert(5));
    TEST_ASSERT_EQUAL(1, set.size());
    TEST_ASSERT(set.exist(5));
}

void test_set_erase() {
    customSet.insert(3);
    customSet.insert(1);
    customSet.insert(4);

    TEST_ASSERT_MESSAGE(customSet.erase(3), "Should erase 3");
    TEST_ASSERT_MESSAGE(!customSet.exist(3), "3 should be removed");
    TEST_ASSERT_MESSAGE(customSet.size() == 2, "Size should be 2");

    TEST_ASSERT_MESSAGE(!customSet.erase(9), "Shouldn't erase non-existent");

    ASSERT_ORDER(customSet);
}

void test_set_erase_boundaries() {
    arx::stdx::set<int, 5> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    set.insert(4);
    set.insert(5);

    // Erase first
    TEST_ASSERT(set.erase(1));
    TEST_ASSERT_EQUAL(4, set.size());
    TEST_ASSERT_TRUE(!set.exist(1));

    // Erase middle
    TEST_ASSERT(set.erase(3));
    TEST_ASSERT_EQUAL(3, set.size());
    TEST_ASSERT_TRUE(!set.exist(3));

    // Erase last
    TEST_ASSERT(set.erase(5));
    TEST_ASSERT_EQUAL(2, set.size());
    TEST_ASSERT_TRUE(!set.exist(5));
}

void test_set_clear() {
    customSet.insert(3);
    customSet.insert(1);
    customSet.insert(4);

    customSet.clear();

    TEST_ASSERT_MESSAGE(customSet.empty(), "Set should be empty");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, customSet.size(), "Set is 0 sized");

    ASSERT_ORDER(customSet);
}

void test_set_duplicate_insert() {
    TEST_ASSERT_MESSAGE(customSet.insert(1), "Shouldn insert");
    TEST_ASSERT_MESSAGE(customSet.insert(4), "Shouldn insert");
    size_t sizeAfter = customSet.size();
    TEST_ASSERT_MESSAGE(!customSet.insert(1), "Shouldn't insert duplicates");
    TEST_ASSERT_MESSAGE(!customSet.insert(4), "Shouldn't insert duplicates");
    TEST_ASSERT_MESSAGE(customSet.size() == sizeAfter, "Size shouldn't change");

    ASSERT_ORDER(customSet);
}

void test_set_comparisons() {
    arx::stdx::set<int, 5> set1{1, 2, 3};
    arx::stdx::set<int, 5> set2{3, 2, 1};
    arx::stdx::set<int, 5> set3{1, 2};

    TEST_ASSERT_MESSAGE(set1 == set1, "Set should equal itself");
    TEST_ASSERT_MESSAGE(set1 == set2, "Different order should be equal");
    TEST_ASSERT_MESSAGE(set1 != set3, "Different sizes should be unequal");

    TEST_ASSERT_MESSAGE(set3 < set1, "{1,2} < {1,2,3}");
}

void test_set_capacity_limit() {
    arx::stdx::set<int, 3> smallSet;
    smallSet.insert(1);
    smallSet.insert(2);
    smallSet.insert(3);
    smallSet.insert(4); // Should be ignored

    TEST_ASSERT_MESSAGE(smallSet.size() == 3, "Should respect capacity");
    TEST_ASSERT_MESSAGE(!smallSet.exist(4), "4 shouldn't be inserted");

    ASSERT_ORDER(customSet);
}

void test_set_copy_constructor() {
    arx::stdx::set<int, 5> original;
    original.insert(3);
    original.insert(1);
    original.insert(4);

    arx::stdx::set<int, 5> copy(original); // Copy
    TEST_ASSERT(copy == original);

    // Modify original
    original.erase(3);
    TEST_ASSERT(copy != original); // Copy remains unchanged
}

template<size_t N>
void ASSERT_ORDER(const arx::stdx::set<int, N> &set) {
    int last = -10000;
    for (int e: set) {
        TEST_ASSERT_MESSAGE(last < e, "Set should be ordered");
        last = e;
    }
}