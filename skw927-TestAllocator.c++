// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------------

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

template <typename A> struct TestAllocator1 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       Allocator<int, 100>, Allocator<double, 100>>
    my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator1, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
  const Allocator<int, 100> x;
  ASSERT_EQ(x[0], 100);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
  ASSERT_EQ(x[0], 100);
}

TEST(TestAllocator2, const_index_really_big) {
  const Allocator<int, 100000> x;
  ASSERT_EQ(x[0], 100000);
}

TEST(TestAllocator2, index_really_big) {
  Allocator<int, 100000> x;
  ASSERT_EQ(x[0], 100000);
}

// a test specifically checking for allocation bounds
TEST(TestAllocator2, bounds_check) {
  Allocator<int, 12> x;
  ASSERT_EQ(x[0], 12);
  ASSERT_EQ(x[8], 12);
  int *p = x.allocate(1);
  ASSERT_EQ(x[0], -12);
  ASSERT_EQ(x[8], -12);
  x.construct(p, 42);
  ASSERT_EQ(x[4], 42);
  ASSERT_THROW(x.allocate(1), std::bad_alloc);
}

// double merge
TEST(TestAllocator2, double_merge) {
  Allocator<int, 36> x;
  ASSERT_EQ(x[0], 36);
  ASSERT_EQ(x[32], 36);
  int *p = x.allocate(1);
  int *q = x.allocate(1);
  int *r = x.allocate(1);
  ASSERT_EQ(x[0], -12);
  ASSERT_EQ(x[8], -12);
  ASSERT_EQ(x[12], -12);
  ASSERT_EQ(x[20], -12);
  ASSERT_EQ(x[24], -12);
  ASSERT_EQ(x[32], -12);
  x.construct(q, 42);
  ASSERT_EQ(*q, 42);
  ASSERT_THROW(x.allocate(1), std::bad_alloc);
  x.destroy(p);
  x.deallocate(p, 1);
  x.destroy(r);
  x.deallocate(r, 1);
  ASSERT_EQ(x[0], 12);
  ASSERT_EQ(x[8], 12);
  ASSERT_EQ(x[24], 12);
  ASSERT_EQ(x[32], 12);
  x.destroy(q);
  x.deallocate(q, 1);
  ASSERT_EQ(x[0], 36);
  ASSERT_EQ(x[32], 36);
}

// --------------
// TestAllocator3
// --------------

template <typename A> struct TestAllocator3 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<int, 100>, Allocator<double, 100>> my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator3, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}
