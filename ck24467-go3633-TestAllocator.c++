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
    my_allocator<double, 100> x;
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


/* Gilad Chirag tests */
// check bad_alloc exception in constructor
TEST(TestAllocator4, Constructor1) {
    try {
        my_allocator<int, 8> x;
        ASSERT_EQ(true, false);
    } 
    catch (std::bad_alloc &e) {
        ASSERT_EQ(true, true);
    }
}

TEST(TestAllocator4, Constructor2) {
    const my_allocator<int, 12> x;
    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[8], 4);
}

TEST(TestAllocator4, Constructor3) {
    const my_allocator<double, 16> x;
    ASSERT_EQ(x[0], 8);
    ASSERT_EQ(x[12], 8);
}


// check for bad_alloc excpetion in allocate
TEST(TestAllocator5, Allocate1) {
    try {
        my_allocator<int, 100> x;
        x.allocate(100);
        ASSERT_EQ(true, false);
    } catch (std::bad_alloc &e) {
        ASSERT_EQ(true, true);
    }
}

// basic test one allocation
TEST(TestAllocator5, Allocate2) {
    my_allocator<int, 100> x;
    x.allocate(3);
    const my_allocator<int, 100> y = x;
    ASSERT_EQ(-12,y[0]);
    ASSERT_EQ(-12,y[16]);
    ASSERT_EQ(72, y[20]);
    ASSERT_EQ(72, y[96]);
}

// testing for double one allocation, has free space after
TEST(TestAllocator5, Allocate3) {
    my_allocator<double, 100> x;
    x.allocate(3);
    const my_allocator<double,100> y = x;
    ASSERT_EQ(-24,y[0]);
    ASSERT_EQ(-24,y[28]);
    ASSERT_EQ(60 ,y[32]);
    ASSERT_EQ(60,y[96]);
}

//testing for double one allocation, no free space left.
//gave more than requested.
TEST(TestAllocator5, Allocate4) {
    my_allocator<double, 100> x;
    x.allocate(10);
    const my_allocator<double,100> y = x;
    ASSERT_EQ(-92,y[0]);
    ASSERT_EQ(-92,y[96]);
}

//allocate, check if sentinels are right
//allocate again, check if new sentinels are right
TEST(TestAllocator5, Allocate5) {
    my_allocator<int, 100> x;
    x.allocate(10);
    const my_allocator<int,100> y = x;
    ASSERT_EQ(-40, y[0]);
    ASSERT_EQ(-40, y[44]);
    ASSERT_EQ(44, y[48]);
    ASSERT_EQ(44, y[96]);
    x.allocate(1);
    const my_allocator<int,100> z = x;
    ASSERT_EQ(-40, z[0]);
    ASSERT_EQ(-40, z[44]);
    ASSERT_EQ(-4, z[48]);
    ASSERT_EQ(-4, z[56]);
    ASSERT_EQ(32, z[60]);
    ASSERT_EQ(32, z[96]);
}

//test case where you give the allocation more than requested
TEST(TestAllocator5, Allocate6) {
  my_allocator<int, 16> x;
  x.allocate(1);
  const my_allocator<int,16> y = x;
  ASSERT_EQ(y[0], -8);
  ASSERT_EQ(y[12], -8);
}

//test case where you give the allocation exactly what was requested
//next allocation fills the entire heap up
TEST(TestAllocator5, Allocate7) {
  my_allocator<int, 24> x;
  x.allocate(1);
  const my_allocator<int,24> y = x;
  ASSERT_EQ(y[0], -4);
  ASSERT_EQ(y[8], -4);
  ASSERT_EQ(y[12], 4);
  ASSERT_EQ(y[20], 4);
  x.allocate(1);
  const my_allocator<int, 24> z = x;
  ASSERT_EQ(z[0], -4);
  ASSERT_EQ(z[8], -4);
  ASSERT_EQ(z[12], -4);
  ASSERT_EQ(z[20], -4);
}


// check for invalid_arg exception in deallocate
TEST(TestAllocator6, Deallocate1) {
    try {
        my_allocator<int, 100> x;
        x.deallocate(nullptr, 100);
        ASSERT_EQ(true, false);
    } catch (std::invalid_argument &e) {
        ASSERT_EQ(true, true);
    }
}

//test case where you allocate then deallocate, no coalescing.
TEST(TestAllocator6, Deallocate2) {
  my_allocator<int, 16> x;
  int *p1 = x.allocate(1);
  const my_allocator<int,16> y = x;
  ASSERT_EQ(y[0], -8);
  ASSERT_EQ(y[12], -8);
  x.deallocate(p1, 1);
  const my_allocator<int,16> z = x;
  ASSERT_EQ(z[0], 8);
  ASSERT_EQ(z[12], 8);
}

//test case where you allocate whole space then deallocate, no coalescing.
TEST(TestAllocator6, Deallocate3) {
  my_allocator<int, 12> x;
  int *p1 = x.allocate(1);
  const my_allocator<int,12> y = x;
  ASSERT_EQ(y[0], -4);
  ASSERT_EQ(y[8], -4);
  x.deallocate(p1, 1);
  const my_allocator<int,12> z = x;
  ASSERT_EQ(z[0], 4);
  ASSERT_EQ(z[8], 4);
}

//deallocate, left coalesce
TEST(TestAllocator6, Deallocate4) {
  my_allocator<int, 100> x;
  int *p1 = x.allocate(1);
  int *p2 = x.allocate(1);
  x.allocate(1);
  x.deallocate(p1, 1);
  x.deallocate(p2, 1);
  const my_allocator<int, 100> y = x;
  ASSERT_EQ(y[0], 16);
  ASSERT_EQ(y[20], 16);
}

//deallocate, right coalesce
TEST(TestAllocator6, Deallocate5) {
  my_allocator<int, 100> x;
  int *p1 = x.allocate(1);
  int *p2 = x.allocate(1);
  x.allocate(1);
  x.deallocate(p2, 1);
  x.deallocate(p1, 1);
  const my_allocator<int, 100> y = x;
  ASSERT_EQ(y[0], 16);
  ASSERT_EQ(y[20], 16);
}

//deallocate, right and left coalesce
TEST(TestAllocator6, Deallocate6) {
  my_allocator<int, 100> x;
  int *p1 = x.allocate(1);
  int *p2 = x.allocate(1);
  int *p3 = x.allocate(1);
  x.deallocate(p3, 1);
  x.deallocate(p1, 1);
  x.deallocate(p2, 1);
  const my_allocator<int, 100> y = x;
  ASSERT_EQ(y[0], 92);
  ASSERT_EQ(y[96], 92);
}