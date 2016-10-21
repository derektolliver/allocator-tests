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
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
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

// --------------
// TestAllocator4
// --------------

TEST(TestAllocator4, constructor1) {
    bool exception = false;

    try {
        my_allocator<int, 100> x;  
        ASSERT_EQ(92, x[0]);
        ASSERT_EQ(92, x[100 - sizeof(int)]);
    } catch(std::bad_alloc& e) {
        exception = true;
    }

    ASSERT_FALSE(exception);
}

TEST(TestAllocator4, constructor2) {
    bool exception = false;

    try {
        my_allocator<int, 10> x;  
    } catch(std::bad_alloc& e) {
        exception = true;
    }

    ASSERT_TRUE(exception);
}

TEST(TestAllocator4, constructor3) {
    bool exception = false;

    try {
        my_allocator<double, 16> x;  
        ASSERT_EQ(8, x[0]);
        ASSERT_EQ(8, x[16 - sizeof(int)]);
    } catch(std::bad_alloc& e) {
        exception = true;
    }

    ASSERT_FALSE(exception);
}

// --------------
// TestAllocator5
// --------------

TEST(TestAllocator5, allocate1) {
    my_allocator<int, 100> x;
    x.allocate(1);

    ASSERT_EQ(x[0], -4); 
    ASSERT_EQ(x[8], -4);
    ASSERT_EQ(x[12], 80);
    ASSERT_EQ(x[96], 80);
}

TEST(TestAllocator5, allocate2) {
    my_allocator<double, 100> x;
    x.allocate(2);

    ASSERT_EQ(x[0], -16); 
    ASSERT_EQ(x[20], -16);
    ASSERT_EQ(x[24], 68);
    ASSERT_EQ(x[96], 68);
}

TEST(TestAllocator5, allocate3) {
    my_allocator<int, 100> x;
    x.allocate(23);

    ASSERT_EQ(x[0], -92); 
    ASSERT_EQ(x[96], -92);
}

// --------------
// TestAllocator6
// --------------

TEST(TestAllocator6, deallocate1) {
    my_allocator<int, 50> x;

    int *p = x.allocate(1);
    x.deallocate(p, 1);  

    ASSERT_EQ(x[0], 42);
    ASSERT_EQ(x[46], 42); 
}

TEST(TestAllocator6, deallocate2) {
    my_allocator<int, 80> x;

    int *p = x.allocate(2);
    x.deallocate(p, 2);

    ASSERT_EQ(x[0], 72); 
    ASSERT_EQ(x[76], 72);
}

TEST(TestAllocator6, deallocate3) {
    my_allocator<int, 100> x;

    const size_t s = 1; 
    int *p1 = x.allocate(s);
    int *p2 = x.allocate(s);
    int *p3 = x.allocate(s);
    x.deallocate(p1, s);
    x.deallocate(p2, s);
    x.deallocate(p3, s);

    ASSERT_EQ(x[0], 92); 
    ASSERT_EQ(x[96], 92);
}

// --------------
// TestAllocator7
// --------------

TEST(TestAllocator7, valid1) {
    my_allocator<int, 100> x;

    ASSERT_TRUE(x.valid()); 
}

TEST(TestAllocator7, valid2) {
    my_allocator<int, 100> x;
    int *p = x.allocate(3);
    x.deallocate(p, 3);

    ASSERT_TRUE(x.valid());
}

TEST(TestAllocator7, valid3) {
    my_allocator<int, 100> x;
    x.allocate(1);
    x.allocate(1);

    ASSERT_TRUE(x.valid()); 
}

// --------------
// TestAllocator8
// --------------

TEST(TestAllocator8, coalesce1) {
    my_allocator<int, 80> x;

    int *p = x.allocate(4);
    x.deallocate(p, 4);  

    ASSERT_EQ(x[0], 72);
    ASSERT_EQ(x[76], 72); 
}

TEST(TestAllocator8, coalesce2) {
    my_allocator<int, 50> x;

    int *p = x.allocate(1);
    x.deallocate(p, 1);  

    ASSERT_EQ(x[0], 42);
    ASSERT_EQ(x[46], 42); 
}
TEST(TestAllocator8, coalesce3) {
    my_allocator<int, 100> x;

    int *p = x.allocate(2);
    int *q = x.allocate(2);
    x.deallocate(p, 2);
    x.deallocate(q, 2);

    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92); 
}

