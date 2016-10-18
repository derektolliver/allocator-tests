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
// TestAllocator1: Test One
// --------------

// TEST(TestAllocator1, test_one){
//      A x;
//     const size_type     size = 4;
//     const value_type    value = 10;
//     const pointer       ptr = x.allocate(s);
    

//     x.construct(ptr, value);
//     ASSERT(*p, v);

//     x.destroy(ptr);
//     x.deallocate(ptr, size); 
// }
// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, const_index2) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, const_index3) {
    const my_allocator<int, 50> x;
    ASSERT_EQ(x[0], 42);}

TEST(TestAllocator2, const_index4) {
    const my_allocator<int, 50> x;
    ASSERT_EQ(x[46], 42);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index2) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, index3) {
    my_allocator<int, 50> x;
    ASSERT_EQ(x[0], 42);}

TEST(TestAllocator2, index4) {
    my_allocator<int, 50> x;
    ASSERT_EQ(x[46], 42);}

TEST(TestAllocator2, freeandbusy) {
    my_allocator<int, 100> x;
    x.allocate(1);
    ASSERT_EQ(x[0], -4);
    ASSERT_EQ(x[8], -4);

    ASSERT_EQ(x[12], 80);
    ASSERT_EQ(x[96], 80);
    }

TEST(TestAllocator2, freeandbusy2) {
    my_allocator<int, 50> x;
    x.allocate(1);
    ASSERT_EQ(x[0], -4);
    ASSERT_EQ(x[8], -4);

    ASSERT_EQ(x[12], 30);
    ASSERT_EQ(x[46], 30);
    }

TEST(TestAllocator2, busy) {
    my_allocator<int, 50> x;
    x.allocate(1);
    ASSERT_EQ(x[0], -4);
    ASSERT_EQ(x[8], -4);
    }

TEST(TestAllocator2, mergeright) {
    my_allocator<int, 50> x;
    int *ptr1 = x.allocate(1);
    int *ptr2 = x.allocate(1);
    x.allocate(1);
    x.deallocate(ptr2, 1);
    x.deallocate(ptr1, 1);
    ASSERT_EQ(x[0], 16);
    ASSERT_EQ(x[50-sizeof(int)], 6);
}

TEST(TestAllocator2, mergeleft) {
    my_allocator<int, 50> x;
    int *ptr1 = x.allocate(1);
    int *ptr2 = x.allocate(1);

    x.allocate(1);
    x.deallocate(ptr1, 1);
    x.deallocate(ptr2, 1);
    ASSERT_EQ(x[0], 16);
    ASSERT_EQ(x[50-sizeof(int)], 6);
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
