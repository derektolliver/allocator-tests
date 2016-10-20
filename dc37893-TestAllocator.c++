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
#include <exception>

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

TEST(TestAllocator2, const_index_double) {
    const my_allocator<double, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, index_double) {
    my_allocator<double, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, index_double1) {
    const my_allocator<double, 1000> x;
    ASSERT_EQ(x[0], 992);
    ASSERT_EQ(x[996], 992);}

TEST(TestAllocator2, fail_index) {
    try
    {
      const my_allocator<int, 4> x;
    }catch (...) {
      ASSERT_TRUE(true);
    }
    ASSERT_FALSE(false);
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
template <typename A>
struct TestAllocator4 : testing::Test {
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

TYPED_TEST_CASE(TestAllocator4, my_types_2);
TYPED_TEST(TestAllocator4, bad_allocate_25) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type      s = 25;
    const size_type      st = 1;
    const value_type     v = 2;
    pointer p = nullptr;
    ASSERT_THROW(x.allocate(s), std::bad_alloc);
    ASSERT_THROW(x.deallocate(p, s), std::invalid_argument);
    pointer b = x.allocate(st);
    x.deallocate(b, st);
    ASSERT_THROW(x.deallocate(b, st), std::invalid_argument);
  }

// --------------
// TestAllocator5
// --------------

template <typename A>
struct TestAllocator5 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int, 100>>
        my_types_3;

TYPED_TEST_CASE(TestAllocator5, my_types_3);

TYPED_TEST(TestAllocator5, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type      s = 1;
    const pointer        p = x.allocate(s);
    ASSERT_EQ(*(p - 1), -4);
    x.deallocate(p, s);
    ASSERT_EQ(*(p-1), 92);

    const pointer p1 = x.allocate(s);
    const pointer p2 = x.allocate(s);
    const pointer p3 = x.allocate(s);
    x.deallocate(p1, s);
    x.deallocate(p2, s);
    ASSERT_EQ(*(p1-1), 16);
    x.deallocate(p3, s);

    const pointer p4 = x.allocate(s);
    const pointer p5 = x.allocate(s);
    const pointer p6 = x.allocate(s);
    x.deallocate(p6, s);
    x.deallocate(p5, s);
    ASSERT_EQ(*(p5-1), 80);
    x.deallocate(p4, s);

    const pointer p7 = x.allocate(s);
    const pointer p8 = x.allocate(s);
    const pointer p9 = x.allocate(s);
    x.deallocate(p7, s);
    x.deallocate(p9, s);
    x.deallocate(p8, s);
    ASSERT_EQ(*(p8-1), 80);
}

    
