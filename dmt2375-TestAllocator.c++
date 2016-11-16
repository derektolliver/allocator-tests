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

using namespace std;

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
            my_allocator<int,    100>,
            my_allocator<double, 100>>
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

// ----------
// Test Cases
// ----------

TEST(TestAllocator4, valid_1) {
    my_allocator<int, 100> x;

    x[0] = -16;
    x[20] = -16;
    x[24] = 68;
    x[96] = 68;

    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator4, valid_2) {
    my_allocator<int, 1000> x;

    x[0] = -16;
    x[20] = -16;
    x[24] = 968;
    x[996] = 968;

    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator4, valid_3) {
    my_allocator<int, 100> x;

    x[0] = -50;
    x[54] = 50;

    ASSERT_EQ(x.valid(), false);}

TEST(TestAllocator4, constructor_1) {
    my_allocator<int, 100> x;

    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator4, constructor_2) {
    my_allocator<double, 504> x;

    ASSERT_EQ(x[0], 496);
    ASSERT_EQ(x[500], 496);}

TEST(TestAllocator4, constructor_3) {
    my_allocator<char, 1000> x;

    ASSERT_EQ(x[0], 992);
    ASSERT_EQ(x[996], 992);}

TEST(TestAllocator4, allocate_1) {
    my_allocator<int, 100> x;

    x.allocate(2);

    ASSERT_EQ(x[0], -8);
    ASSERT_EQ(x[12], -8);
    ASSERT_EQ(x[16], 76);
    ASSERT_EQ(x[96], 76);
}

TEST(TestAllocator4, allocate_2) {
    my_allocator<int, 100> x;

    x.allocate(23);

    ASSERT_EQ(x[0], -92);
    ASSERT_EQ(x[96], -92);
}

TEST(TestAllocator4, allocate_3) {
    my_allocator<double, 48> x;

    x.allocate(1);
    x.allocate(1);

    ASSERT_EQ(x[0], -8);
    ASSERT_EQ(x[12], -8);
    // Not enough space to split
    ASSERT_EQ(x[16], -24);
    ASSERT_EQ(x[44], -24);
}

TEST(TestAllocator4, deallocate_1) {
    my_allocator<int, 100> x;

    int *p1 = x.allocate(1);
    int *p2 = x.allocate(1);

    x.deallocate(p1, 1);

    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[8], 4);

    x.deallocate(p2, 1);
}

TEST(TestAllocator4, deallocate_2) {
    my_allocator<int, 500> x;

    int *p = x.allocate(1);
    
    ASSERT_THROW(x.deallocate(p - 1, 1), std::invalid_argument);
}

TEST(TestAllocator4, deallocate_3) {
    my_allocator<int, 100> x;

    int *p = x.allocate(1);
    ASSERT_THROW(x.deallocate(p + 1, 5), std::invalid_argument);
}
