#include <supp/IntrusivePriorityQueue.h>

#include <utest/utest.h>

struct T : supp::IntrusivePriorityQueueNode {
    T(int p) : priority(p) {}
    int priority = 0;
};

using Comp = decltype([](const T& a, const T& b) { return a.priority < b.priority; });

TEST(test_pq_default) {
    supp::IntrusivePriorityQueue<T, 3, Comp> pq;
    TEST_ASSERT_EQUAL(3, pq.capacity());
    TEST_ASSERT_EQUAL(0, pq.size());
    TEST_ASSERT_TRUE(pq.empty());
}

TEST(test_pq_insert) {
    supp::IntrusivePriorityQueue<T, 3, Comp> pq;

    T t1{10}, t2{20}, t3{30};
    pq.push(&t1);
    pq.push(&t3);
    pq.push(&t2);

    TEST_ASSERT_EQUAL(3, pq.size());
    TEST_ASSERT_EQUAL(10, pq.front()->priority);
    TEST_ASSERT_EQUAL(10, pq.pop()->priority);
    TEST_ASSERT_EQUAL(20, pq.pop()->priority);
    TEST_ASSERT_EQUAL(30, pq.pop()->priority);
    TEST_ASSERT_EQUAL(0, pq.size());
}

TEST(test_pq_clear) {
    supp::IntrusivePriorityQueue<T, 3, Comp> pq;

    T t1{10}, t2{20}, t3{30};
    pq.push(&t1);
    pq.push(&t3);
    pq.push(&t2);

    TEST_ASSERT_EQUAL(3, pq.size());
    pq.clear();
    TEST_ASSERT_EQUAL(0, pq.size());
}

TESTS_MAIN
