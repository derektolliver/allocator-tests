// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------------

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
            my_allocator<int,    100>,
            my_allocator<double, 100>>
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
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

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
            my_allocator<int,    160>,
            my_allocator<double, 160>>
        my_types_3;

TYPED_TEST_CASE(TestAllocator3, my_types_3);
TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    size_type      s = 19;
    value_type     v = 2;
    pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);
    }

    my_allocator<int, 100> y;
    int *p2 = y.allocate(15);
    ASSERT_EQ(*(p2-1), -60);
    ASSERT_EQ(*(p2+15), -60);

    v = 5;

    p2 += 5;
    y.construct(p2, v);
    ASSERT_EQ(v, *p2);
}

TEST(TestAllocator4, test4) {
    const my_allocator<double, 500> x;
    ASSERT_EQ(492, x[0]);
    ASSERT_EQ(492, x[496]);
}

TEST(TestAllocator5, test5) {
    const my_allocator<double, 1000> x;
    ASSERT_EQ(992, x[0]);
    ASSERT_EQ(992, x[996]);
}

TEST(TestAllocator6, test6) {
    const my_allocator<double, 200> x;
    ASSERT_EQ(192, x[0]);
    ASSERT_EQ(192, x[196]);
}



template <typename A>
struct TestAllocator8 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::size_type  size_type;};

typedef testing::Types<
            my_allocator<int,    160>,
            my_allocator<double, 160>>
        my_types_4;

TYPED_TEST_CASE(TestAllocator8, my_types_4);
TYPED_TEST(TestAllocator8, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;

          allocator_type x;


    size_type      s = 10;

    my_allocator<int, 100> y;
    int *p2 = y.allocate(10);
    ASSERT_EQ(*(p2-1), -40);
    ASSERT_EQ(*(p2+10), -40);
}




template <typename A>
struct TestAllocator9 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<double,    160>>
        my_types_5;

TYPED_TEST_CASE(TestAllocator9, my_types_5);
TYPED_TEST(TestAllocator9, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;

          allocator_type x;


    size_type      s = 8;

    my_allocator<int, 100> y;
    int *p = y.allocate(8);
    ASSERT_EQ(*(p-1), -32);
    ASSERT_EQ(*(p+8), -32);

    // my_allocator<int, 100> y;
    // int *p2 = y.allocate(10);
    // ASSERT_EQ(*(p2-1), -40);
    // ASSERT_EQ(*(p2+10), -40);
}