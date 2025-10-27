#include <supp/RandomAccessPriorityQueue.h>

#include <utest/utest.h>

struct T : supp::RandomAccessPriorityQueueNode {
    T(int p) : priority(p) {}
    int priority = 0;
};

using Comp = decltype([](const T& a, const T& b) { return a.priority < b.priority; });

TEST(test_empty) {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    TEST_ASSERT_EQUAL(0, p.size());
    TEST_ASSERT_EQUAL(3, p.capacity());
    TEST_ASSERT_TRUE(p.empty());
}

TEST(test_one_element) {
    supp::RandomAccessPriorityQueue<T, 1, Comp> p;

    T e{10};
    p.push(&e);
    TEST_ASSERT_EQUAL(1, p.size());
    TEST_ASSERT_FALSE(p.empty());
    TEST_ASSERT_TRUE(e.connected());

    SECTION("front") {
        TEST_ASSERT_EQUAL(10, p.front()->priority);
    }

    SECTION("pop") {
        TEST_ASSERT_EQUAL(10, p.pop()->priority);
        TEST_ASSERT_TRUE(p.empty());
    }

    SECTION("erase") {
        TEST_ASSERT_TRUE(p.erase(&e));
        TEST_ASSERT_TRUE(p.empty());
    }
}

TEST(test_multiple_elements) {
    supp::RandomAccessPriorityQueue<T, 3, Comp> p;

    T e1{30}, e2{10}, e3{20};
    p.push(&e1);
    p.push(&e2);
    p.push(&e3);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    SECTION("front") {
        TEST_ASSERT_EQUAL(10, p.front()->priority);
    }

    SECTION("pop") {
        TEST_ASSERT_EQUAL(10, p.pop()->priority);
        TEST_ASSERT_EQUAL(20, p.pop()->priority);
        TEST_ASSERT_EQUAL(30, p.pop()->priority);
        TEST_ASSERT_TRUE(p.empty());
    }

    SECTION("erase front") {
        TEST_ASSERT_TRUE(p.erase(&e2));
        TEST_ASSERT_EQUAL(2, p.size());
        TEST_ASSERT_EQUAL(20, p.pop()->priority);
        TEST_ASSERT_EQUAL(30, p.pop()->priority);
        TEST_ASSERT_TRUE(p.empty());
    }

    SECTION("erase mid") {
        TEST_ASSERT_TRUE(p.erase(&e3));
        TEST_ASSERT_EQUAL(2, p.size());
        TEST_ASSERT_EQUAL(10, p.pop()->priority);
        TEST_ASSERT_EQUAL(30, p.pop()->priority);
        TEST_ASSERT_TRUE(p.empty());
    }

    SECTION("erase back") {
        TEST_ASSERT_TRUE(p.erase(&e1));
        TEST_ASSERT_EQUAL(2, p.size());
        TEST_ASSERT_EQUAL(10, p.pop()->priority);
        TEST_ASSERT_EQUAL(20, p.pop()->priority);
        TEST_ASSERT_TRUE(p.empty());
    }

    SECTION("clear") {
        p.clear();
        TEST_ASSERT_TRUE(p.empty());
    }
}

TEST(test_more_operations) {
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

TEST(test_more_operations_with_erase) {
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

TESTS_MAIN
