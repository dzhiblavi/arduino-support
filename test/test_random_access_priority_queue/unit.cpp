#include <supp/RandomAccessPriorityQueue.h>

#include <unity.h>

struct T : supp::RandomAccessPriorityQueueNode {
    T(int p) : priority(p) {}
    int priority = 0;
};

using Comp = decltype([](const T& a, const T& b) { return a.priority < b.priority; });

void test_empty() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    TEST_ASSERT_EQUAL(0, p.size());
    TEST_ASSERT_EQUAL(3, p.capacity());
    TEST_ASSERT_TRUE(p.empty());
}

void test_one_element_front() {
    supp::RandomAccessPriorityQueue<T, 1, Comp> p;

    T e{10};
    p.push(&e);
    TEST_ASSERT_EQUAL(1, p.size());
    TEST_ASSERT_FALSE(p.empty());
    TEST_ASSERT_TRUE(e.connected());

    TEST_ASSERT_EQUAL(10, p.front()->priority);
}

void test_one_element_pop() {
    supp::RandomAccessPriorityQueue<T, 1, Comp> p;

    T e{10};
    p.push(&e);
    TEST_ASSERT_EQUAL(1, p.size());
    TEST_ASSERT_FALSE(p.empty());
    TEST_ASSERT_TRUE(e.connected());

    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    TEST_ASSERT_TRUE(p.empty());
}

void test_one_element_erase() {
    supp::RandomAccessPriorityQueue<T, 1, Comp> p;

    T e{10};
    p.push(&e);
    TEST_ASSERT_EQUAL(1, p.size());
    TEST_ASSERT_FALSE(p.empty());
    TEST_ASSERT_TRUE(e.connected());

    TEST_ASSERT_TRUE(p.erase(&e));
    TEST_ASSERT_TRUE(p.empty());
}

void test_multiple_elements_front() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{30}, e2{10}, e3{20};
    p.push(&e1);
    p.push(&e2);
    p.push(&e3);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_EQUAL(10, p.front()->priority);
}

void test_multiple_elements_pop() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{30}, e2{10}, e3{20};
    p.push(&e1);
    p.push(&e2);
    p.push(&e3);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    TEST_ASSERT_EQUAL(20, p.pop()->priority);
    TEST_ASSERT_EQUAL(30, p.pop()->priority);
    TEST_ASSERT_TRUE(p.empty());
}

void test_multiple_elements_pop_erase_front() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{30}, e2{10}, e3{20};
    p.push(&e1);
    p.push(&e2);
    p.push(&e3);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_TRUE(p.erase(&e2));
    TEST_ASSERT_EQUAL(2, p.size());
    TEST_ASSERT_EQUAL(20, p.pop()->priority);
    TEST_ASSERT_EQUAL(30, p.pop()->priority);
    TEST_ASSERT_TRUE(p.empty());
}

void test_multiple_elements_pop_erase_mid() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{30}, e2{10}, e3{20};
    p.push(&e1);
    p.push(&e2);
    p.push(&e3);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_TRUE(p.erase(&e3));
    TEST_ASSERT_EQUAL(2, p.size());
    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    TEST_ASSERT_EQUAL(30, p.pop()->priority);
    TEST_ASSERT_TRUE(p.empty());
}

void test_multiple_elements_pop_erase_back() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{30}, e2{10}, e3{20};
    p.push(&e1);
    p.push(&e2);
    p.push(&e3);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    TEST_ASSERT_TRUE(p.erase(&e1));
    TEST_ASSERT_EQUAL(2, p.size());
    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    TEST_ASSERT_EQUAL(20, p.pop()->priority);
    TEST_ASSERT_TRUE(p.empty());
}

void test_multiple_elements_pop_erase_clear() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{30}, e2{10}, e3{20};
    p.push(&e1);
    p.push(&e2);
    p.push(&e3);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    p.clear();
    TEST_ASSERT_TRUE(p.empty());
}

void test_more_operations() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{10}, e2{20}, e3{30}, e4{40}, e5{50}, e6{60};

    p.push(&e3);
    p.push(&e1);
    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    p.push(&e1);
    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    p.push(&e4);
    TEST_ASSERT_EQUAL(30, p.pop()->priority);
    TEST_ASSERT_EQUAL(40, p.pop()->priority);
    p.push(&e1);
    p.push(&e5);
    p.push(&e2);
    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    TEST_ASSERT_EQUAL(20, p.pop()->priority);
    p.push(&e6);
    TEST_ASSERT_EQUAL(50, p.pop()->priority);
    TEST_ASSERT_EQUAL(60, p.pop()->priority);
}

void test_more_operations_with_erase() {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{10}, e2{20}, e3{30}, e4{40}, e5{50}, e6{60};

    p.push(&e3);
    p.push(&e1);
    TEST_ASSERT_TRUE(p.erase(&e3));
    TEST_ASSERT_EQUAL(10, p.front()->priority);
    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    p.push(&e4);
    TEST_ASSERT_EQUAL(40, p.pop()->priority);
    p.push(&e1);
    p.push(&e5);
    p.push(&e2);
    TEST_ASSERT_TRUE(p.erase(&e2));
    TEST_ASSERT_EQUAL(10, p.pop()->priority);
    p.push(&e6);
    TEST_ASSERT_EQUAL(50, p.pop()->priority);
    TEST_ASSERT_EQUAL(60, p.pop()->priority);
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty);
    RUN_TEST(test_one_element_front);
    RUN_TEST(test_one_element_pop);
    RUN_TEST(test_one_element_erase);
    RUN_TEST(test_multiple_elements_front);
    RUN_TEST(test_multiple_elements_pop);
    RUN_TEST(test_multiple_elements_pop_erase_front);
    RUN_TEST(test_multiple_elements_pop_erase_mid);
    RUN_TEST(test_multiple_elements_pop_erase_back);
    RUN_TEST(test_multiple_elements_pop_erase_clear);
    RUN_TEST(test_more_operations);
    RUN_TEST(test_more_operations_with_erase);
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
