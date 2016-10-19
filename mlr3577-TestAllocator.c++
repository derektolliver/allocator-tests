/** 
 * @file TestAllocator.c++
 * @brief Unit tests for Allocator using Google Test.
 * @copyright Copyright (C) 2016
 * @author Glenn P. Downing
 * @author Matt Ruston, Brian Cui
 */

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int,    100>,
            Allocator<double, 100>>
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type  x;
    const size_type       s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const Allocator<int, 100> x;
    ASSERT_EQ(x[0], (100 - (sizeof(int) * 2)));}

TEST(TestAllocator2, index) {
    Allocator<int, 100> x;
    ASSERT_EQ(x[0], (100 - (sizeof(int) * 2)));}

// ---------------------------------
// Tests for Allocator() constructor
// ---------------------------------

/** @brief Test constructor with size 200, validates sentinels. */
TEST(TestAllocator2, constructor1) {
    const int len = 200;
    Allocator<int, len> x;
    ASSERT_EQ(x[0], (len - (sizeof(int) * 2)));
    ASSERT_EQ(x[len - sizeof(int)], 
              (len - (sizeof(int) * 2)));
}

/** @brief Test constructor with double */
TEST(TestAllocator2, constructor2) {
    const int len = 100;
    Allocator<double, len> x;
    ASSERT_EQ(x[0], (len - (sizeof(int) * 2)));
    ASSERT_EQ(x[len - sizeof(int)], 
              (len - (sizeof(int) * 2)));
}

/** @brief Test constructor with size too small */
TEST(TestAllocator2, constructor3) {
    const int len = sizeof(double) - 1;
    ASSERT_THROW((Allocator<double, len>()), std::bad_alloc);
}

// --------------------
// Tests for allocate()
// --------------------

/** @brief Allocator has exactly enough room for 1 */
TEST(TestAllocator2, allocate1) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;

    int* loc = x.allocate(1);
    ASSERT_EQ(x[0], -s);
    ASSERT_EQ(x[s * 2], -s);
    ASSERT_EQ(loc, &x[s]);
}

/** @brief Allocator has slightly more room for 1
   but not for 2 blocks */
TEST(TestAllocator2, allocate2) {
    const int s = sizeof(int);
    const int i = 4;

    Allocator<int, i * s > x;
    
    int* loc = x.allocate(1);
    ASSERT_EQ(x[0], -s * 2);
    ASSERT_EQ(x[s * 3], -s * 2);
    ASSERT_EQ(loc, &x[s]);
}

/** @brief Allocator has enough room for 2 blocks */
TEST(TestAllocator2, allocate3) {
    const int s = sizeof(int);
    const int i = 6;

    Allocator<int, i * s > x;
    
    int* loc = x.allocate(1);
    ASSERT_EQ(x[0], -s);
    ASSERT_EQ(x[s * 2], -s);
    ASSERT_EQ(loc, &x[s]);
    ASSERT_EQ(x[s * 3], s);
    ASSERT_EQ(x[s * 5], s);
}

/** @brief Allocator doesn't have enough room for requests */
TEST(TestAllocator2, allocate4) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;

    x.allocate(1);
    ASSERT_THROW((x.allocate(1)), std::bad_alloc);
    ASSERT_THROW((x.allocate(40)), std::bad_alloc);
}

/** @brief Allocator has slightly more room for 1
   but not for 2 blocks. double version. */
TEST(TestAllocator2, allocate5) {
    const int s = sizeof(int);
    const int d = sizeof(double);

    Allocator<double, 2 * s + 2 * d > x;
    
    double* loc = x.allocate(1);
    ASSERT_EQ(x[0], -d * 2);
    ASSERT_EQ(x[s + d * 2], -d * 2);
    ASSERT_EQ(loc, reinterpret_cast<double *>(&x[s]));
}


// ----------------
// Tests for valid()
// ----------------

/** @brief Free heap should be valid */
TEST(TestAllocator2, valid1) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;
    int test_heap[] = { s, 12345, s };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));
    ASSERT_TRUE(x.valid());
}

/** @brief Heap with lost space should be invalid */
TEST(TestAllocator2, valid2) {
    const int s = sizeof(int);
    const int i = 4;

    Allocator<int, i * s > x;
    int test_heap[] = { s, 111, s, 0 };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));
    ASSERT_FALSE(x.valid());
}

/** @brief Heap with two contiguous free blocks should be invalid */
TEST(TestAllocator2, valid3) {
    const int s = sizeof(int);
    const int i = 7;

    Allocator<int, i * s > x;
    int test_heap[] = { s * 2, 111, 444, s * 2, s, 512, s };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));
    ASSERT_FALSE(x.valid());
}

/** @brief Heap with mismatched sentinels should be invalid */
TEST(TestAllocator2, valid4) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;
    int test_heap[] = { s * 2, 222, -s * 2 };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));
    ASSERT_FALSE(x.valid());
}

/** @brief Heap with zero value sentinels should be invalid */
TEST(TestAllocator2, valid5) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;
    int test_heap[] = { 0, 0, 0 };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));
    ASSERT_FALSE(x.valid());
}

/** @brief Heap with sentinels that are too large should be invalid */
TEST(TestAllocator2, valid6) {
    const int s = sizeof(int);
    const int i = 5;

    Allocator<int, i * s > x;
    int test_heap[] = { s * 999, 24, 11, 63, s * 999 };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));
    ASSERT_FALSE(x.valid());
}

/** @brief A more extensive heap validation test, should be valid */
TEST(TestAllocator2, valid7) {
    const int s = sizeof(int);
    const int i = 16;

    Allocator<int, i * s > x;
    int test_heap[] = {     -s,  12,   -s, 
                         s * 2, 412, -252,  s * 2,
                        -s * 2, 214, 2152, -s * 2,
                         s * 3,   0,    1,      2, s * 3 };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));
    ASSERT_TRUE(x.valid());
}

// ----------------------
// Tests for deallocate()
// ----------------------

/** @brief Deallocate a normal used block. */
TEST(TestAllocator2, deallocate1) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;
    int test_heap[] = { -s, 1234, -s };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));

    int* p = &x[s];
    x.deallocate(p, 1);

    ASSERT_EQ(x[0], s);
    ASSERT_EQ(x[s * 2], s);
}

/** @brief Deallocate and coalesce blocks. */
TEST(TestAllocator2, deallocate2) {
    const int s = sizeof(int);
    const int i = 9;

    Allocator<int, i * s > x;
    int test_heap[] = { -s, 1234, -s, -s, 1234, -s, -s, 1234, -s };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));

    int* p1 = &x[s];
    int* p2 = &x[s * 4];
    int* p3 = &x[s * 7];

    x.deallocate(p1, 1);

    ASSERT_EQ(x[0], s);
    ASSERT_EQ(x[s * 2], s);

    x.deallocate(p3, 1);

    ASSERT_EQ(x[s * 6], s);
    ASSERT_EQ(x[s * 8], s);

    x.deallocate(p2, 1);

    ASSERT_EQ(x[0], s * 7);
    ASSERT_EQ(x[s * 8], s * 7);
}

/** @brief Deallocate but do not coalesce, left */
TEST(TestAllocator2, deallocate3) {
    const int s = sizeof(int);
    const int i = 6;

    Allocator<int, i * s > x;
    int test_heap[] = { s, 1234, s, -s, 1234, -s };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));

    int* p = &x[s * 4];

    x.deallocate(p, 1);

    ASSERT_EQ(x[0], s * 4);
    ASSERT_EQ(x[s * 5], s * 4);
}

/** @brief Deallocate but do not coalesce, right */
TEST(TestAllocator2, deallocate4) {
    const int s = sizeof(int);
    const int i = 6;

    Allocator<int, i * s > x;
    int test_heap[] = { -s, 1234, -s, s, 1234, s };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));

    int* p = &x[s];

    x.deallocate(p, 1);

    ASSERT_EQ(x[0], s * 4);
    ASSERT_EQ(x[s * 5], s * 4);
}

/** @brief Deallocate and coalesce right */
TEST(TestAllocator2, deallocate5) {
    const int s = sizeof(int);
    const int i = 9;

    Allocator<int, i * s > x;
    int test_heap[] = { s, 1234, s, -s, 1234, -s, -s, 1234, -s };
    std::copy(test_heap, test_heap + i, reinterpret_cast<int *>(x.a));

    int* p1 = &x[s * 4];
    int* p2 = &x[s * 7];

    x.deallocate(p1, 1);

    ASSERT_EQ(x[0], s * 4);
    ASSERT_EQ(x[s * 5], s * 4);

    x.deallocate(p2, 1);

    ASSERT_EQ(x[0], s * 7);
    ASSERT_EQ(x[s * 8], s * 7);
}

/** @brief Deallocating pointer outside of heap is invalid */
TEST(TestAllocator2, deallocate6) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;

    ASSERT_THROW((x.deallocate(&x[s * i + 1], 1)), std::invalid_argument);
    ASSERT_THROW((x.deallocate(&x[-1], 1)), std::invalid_argument);
}

/** @brief Deallocating pointer within start sentinel is invalid */
TEST(TestAllocator2, deallocate7) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;

    ASSERT_THROW((x.deallocate(&x[0], 1)), std::invalid_argument);
    ASSERT_THROW((x.deallocate(&x[s - 1], 1)), std::invalid_argument);
}

/** Deallocating pointer to unused or bad block is invalid */
TEST(TestAllocator2, deallocate8) {
    const int s = sizeof(int);
    const int i = 3;

    Allocator<int, i * s > x;

    int test_heap_unused[] = { s, 1234, s };
    std::copy(test_heap_unused, test_heap_unused + i, reinterpret_cast<int *>(x.a));

    ASSERT_THROW((x.deallocate(&x[s], 1)), std::invalid_argument);

    int test_heap_large[] = { s * 10, 1234, s * 10 };
    std::copy(test_heap_large, test_heap_large + i, reinterpret_cast<int *>(x.a));

    ASSERT_THROW((x.deallocate(&x[s], 1)), std::invalid_argument);

    int test_heap_mismatch[] = { s, 1234, 0 };
    std::copy(test_heap_mismatch, test_heap_mismatch + i, reinterpret_cast<int *>(x.a));

    ASSERT_THROW((x.deallocate(&x[s], 1)), std::invalid_argument);
}

// --------------
// TestAllocator3
// --------------

template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            Allocator<int,    100>,
            Allocator<double, 100>>
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;
    const value_type     v = 2;
    const pointer        b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}
