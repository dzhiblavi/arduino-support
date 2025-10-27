#include <supp/PriorityQueue.h>

#include <unity.h>

using Comp = decltype([](int x, int y) { return x < y; });

void test_empty() {
    supp::PriorityQueue<int, 3, Comp> p;

    TEST_ASSERT_EQUAL(0, p.size());
    TEST_ASSERT_EQUAL(3, p.capacity());
    TEST_ASSERT_TRUE(p.empty());
}

void test_one_element_front() {
    supp::PriorityQueue<int, 1, Comp> p;

    p.push(10);
    TEST_ASSERT_EQUAL(1, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_EQUAL(10, p.front());
}

void test_one_element_pop() {
    supp::PriorityQueue<int, 1, Comp> p;

    p.push(10);
    TEST_ASSERT_EQUAL(1, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_EQUAL(10, p.pop());
    TEST_ASSERT_TRUE(p.empty());
}

void test_multiple_elements_front() {
    supp::PriorityQueue<int, 3, Comp> p;

    p.push(30);
    p.push(10);
    p.push(20);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_EQUAL(10, p.front());
}

void test_multiple_elements_pop() {
    supp::PriorityQueue<int, 3, Comp> p;

    p.push(30);
    p.push(10);
    p.push(20);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_EQUAL(10, p.pop());
    TEST_ASSERT_EQUAL(20, p.pop());
    TEST_ASSERT_EQUAL(30, p.pop());
    TEST_ASSERT_TRUE(p.empty());
}

void test_multiple_elements_clear() {
    supp::PriorityQueue<int, 3, Comp> p;

    p.push(30);
    p.push(10);
    p.push(20);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    p.clear();
    TEST_ASSERT_TRUE(p.empty());
}

void test_more_operations() {
    supp::PriorityQueue<int, 3, Comp> p;

    p.push(30);
    p.push(10);
    TEST_ASSERT_EQUAL(10, p.pop());
    p.push(15);
    TEST_ASSERT_EQUAL(15, p.pop());
    p.push(40);
    TEST_ASSERT_EQUAL(30, p.pop());
    TEST_ASSERT_EQUAL(40, p.pop());
    p.push(10);
    p.push(50);
    p.push(20);
    TEST_ASSERT_EQUAL(10, p.pop());
    TEST_ASSERT_EQUAL(20, p.pop());
    p.push(100);
    TEST_ASSERT_EQUAL(50, p.pop());
    TEST_ASSERT_EQUAL(100, p.pop());
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty);
    RUN_TEST(test_one_element_front);
    RUN_TEST(test_one_element_pop);
    RUN_TEST(test_multiple_elements_front);
    RUN_TEST(test_multiple_elements_pop);
    RUN_TEST(test_multiple_elements_clear);
    RUN_TEST(test_more_operations);
    return UNITY_END();
}

#if !defined(ARDUINO)

int main(void) {
    return runUnityTests();
}

#else

#include <Arduino.h>

void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);
    runUnityTests();
}

void loop() {}

#endif
