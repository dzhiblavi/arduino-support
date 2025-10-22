#include <supp/PriorityQueue.h>

#include <log.h>
#include <utest/utest.h>

using Comp = decltype([](int x, int y) { return x < y; });

TEST(test_empty) {
    supp::PriorityQueue<int, 3, Comp> p;

    TEST_ASSERT_EQUAL(0, p.size());
    TEST_ASSERT_EQUAL(3, p.capacity());
    TEST_ASSERT_TRUE(p.empty());
}

TEST(test_one_element) {
    supp::PriorityQueue<int, 1, Comp> p;

    p.push(10);
    TEST_ASSERT_EQUAL(1, p.size());
    TEST_ASSERT_FALSE(p.empty());

    SECTION("front") {
        TEST_ASSERT_EQUAL(10, p.front());
    }

    SECTION("pop") {
        TEST_ASSERT_EQUAL(10, p.pop());
        TEST_ASSERT_TRUE(p.empty());
    }
}

TEST(test_multiple_elements) {
    supp::PriorityQueue<int, 3, Comp> p;

    p.push(30);
    p.push(10);
    p.push(20);
    TEST_ASSERT_EQUAL(3, p.size());
    TEST_ASSERT_FALSE(p.empty());

    SECTION("front") {
        TEST_ASSERT_EQUAL(10, p.front());
    }

    SECTION("pop") {
        TEST_ASSERT_EQUAL(10, p.pop());
        TEST_ASSERT_EQUAL(20, p.pop());
        TEST_ASSERT_EQUAL(30, p.pop());
        TEST_ASSERT_TRUE(p.empty());
    }

    SECTION("clear") {
        p.clear();
        TEST_ASSERT_TRUE(p.empty());
    }
}

TEST(test_more_operations) {
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

TESTS_MAIN
