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
                       my_allocator<int, 100>, my_allocator<double, 100>>
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
  const my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
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

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_2;

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

// ---------------
// TestConstructor
// ---------------

TEST(TestConstructor, smallest_int_allocation) {
  my_allocator<int, 12> x;
  ASSERT_EQ(x[0], 4);
  ASSERT_EQ(x[8], 4);
}

TEST(TestConstructor, smallest_double_allocation) {
  my_allocator<int, 16> x;
  ASSERT_EQ(x[0], 8);
  ASSERT_EQ(x[12], 8);
}

TEST(TestConstructor, bad_allocation) {
  bool err = false;
  try {
    my_allocator<double, 15> x;
  } catch (std::bad_alloc &e) {
    err = true;
  }
  ASSERT_TRUE(err);
}

TEST(TestConstructor, large_double_allocation) {
  my_allocator<double, 10000> x;
  ASSERT_EQ(x[0], 9992);
  ASSERT_EQ(x[9996], 9992);
}

// ---------
// TestValid
// ---------

TEST(TestValid, valid1) {
  my_allocator<int, 36> x;
  x[0] = 4;
  x[8] = 4;
  x[12] = -4;
  x[20] = -4;
  x[24] = 4;
  x[32] = 4;
  ASSERT_TRUE(x.valid());
}

TEST(TestValid, valid2) {
  my_allocator<double, 80> x;
  x[0] = -8;
  x[12] = -8;
  x[16] = 32;
  x[52] = 32;
  x[56] = -16;
  x[76] = -16;
  ASSERT_TRUE(x.valid());
}

TEST(TestValid, not_coalesced) {
  my_allocator<int, 48> x;
  x[0] = -4;
  x[8] = -4;
  x[12] = -4;
  x[20] = -4;
  x[24] = 4;
  x[32] = 4;
  x[36] = 4;
  x[44] = 4;
  ASSERT_FALSE(x.valid());
}

TEST(TestValid, invalid_sentinal) {
  my_allocator<int, 12> x;
  x[0] = 0;
  ASSERT_FALSE(x.valid());
}

TEST(TestValid, out_of_bounds) {
  my_allocator<double, 16> x;
  x[0] = 9;
  ASSERT_FALSE(x.valid());
}

// ------------
// TestAllocate
// ------------

TEST(TestAllocate, fill_exact) {
  my_allocator<int, 40> x;
  x.allocate(1);
  x.allocate(2);
  x.allocate(1);
  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], -8);
  ASSERT_EQ(x[24], -8);
  ASSERT_EQ(x[28], -4);
  ASSERT_EQ(x[36], -4);
}

TEST(TestAllocate, fill_more_than_expected) {
  my_allocator<double, 24> x;
  x.allocate(1);
  ASSERT_EQ(x[0], -16);
  ASSERT_EQ(x[20], -16);
}

TEST(TestAllocate, N_less_than_zero) {
  my_allocator<int, 12> x;
  bool err = false;
  try {
    x.allocate(-1);
  } catch (std::bad_alloc &e) {
    err = true;
  }
  ASSERT_TRUE(err);
}

TEST(TestAllocate, no_space) {
  my_allocator<double, 16> x;
  bool err = false;
  try {
    x.allocate(9);
  } catch (std::bad_alloc &e) {
    err = true;
  }
  ASSERT_TRUE(err);
}

// --------------
// TestDeallocate
// --------------

TEST(TestDeallocate, deallocate1) {
  // Test1, fully allocated ints
  my_allocator<int, 24> x;
  ASSERT_EQ(x[0], 16);
  int *p0 = x.allocate(4);
  ASSERT_EQ(x[0], -16);
  x.deallocate(p0, 4);
  ASSERT_EQ(x[0], 16);
  // Test2, free space after
  int *p1 = x.allocate(1);
  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[12], 4);
  x.deallocate(p1, 1);
  ASSERT_EQ(x[0], 16);
  // Test3, no free space after
  p0 = x.allocate(1);
  p1 = x.allocate(1);
  x.deallocate(p0, 1);
  ASSERT_EQ(x[0], 4);
  ASSERT_EQ(x[12], -4);
  // Test4, free space before
  x.deallocate(p1, 1);
  ASSERT_EQ(x[0], 16);
  // Test5, no free space before
  p0 = x.allocate(1);
  p1 = x.allocate(1);
  x.deallocate(p1, 1);
  ASSERT_EQ(x[12], 4);
  ASSERT_EQ(x[0], -4);
  x.deallocate(p0, 1);
  ASSERT_EQ(x[0], 16);
  // Test6, space for 3 doubles, test between
  my_allocator<double, 48> y;
  double *p2 = y.allocate(1);
  double *p3 = y.allocate(1);
  double *p4 = y.allocate(1);
  y.deallocate(p3, 1);
  ASSERT_EQ(y[16], 8);
  // Test7, test free space before and after
  p3 = y.allocate(1);
  y.deallocate(p2, 1);
  y.deallocate(p4, 1);
  y.deallocate(p3, 1);
  ASSERT_EQ(y[0], 40);
  // Test8, test free space after
  p2 = y.allocate(1);
  p3 = y.allocate(1);
  y.deallocate(p3, 1);
  ASSERT_EQ(y[0], -8);
  ASSERT_EQ(y[16], 24);
  // Test9, test free space before
  p3 = y.allocate(1);
  p4 = y.allocate(1);
  y.deallocate(p2, 1);
  y.deallocate(p3, 1);
  ASSERT_EQ(y[0], 24);
  ASSERT_EQ(y[32], -8);
  y.deallocate(p4, 1);
}
