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
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
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

// --------------
// TestAllocator4
// --------------
TEST(TestAllocator4, valid1) {
  Allocator<int, 100> y;
  y[0] = 10;
  y[14] = -10;
  ASSERT_EQ(y.valid(), false);
}

TEST(TestAllocator4, valid2) {
  Allocator<int, 100> x;
  x[0] = 42;
  x[46] = 42;
  x[50] = 42;
  x[96] = 42;
  ASSERT_EQ(x.valid(), false);
}

TEST(TestAllocator4, valid3) {
  Allocator<int, 100> x;
  x[0] = 42;
  x[46] = 42;
  x[50] = -42;
  x[96] = -42;
  ASSERT_EQ(x.valid(), true);
}
TEST(TestAllocator4, valid4) {
  Allocator<int, 100> x;
  x[0] = -4;
  x[8] = -4;
  x[12] = -4;
  x[20] = -4;
  x[24] = 68;
  x[96] = 68;
  ASSERT_EQ(x.valid(), true);
}

// --------------
// TestAllocator5
// --------------
// NOTE: ALL of these tests must be friends with Allocator.h

TEST(TestAllocator5, Constructor_1) {
  Allocator<int, 100> b;

  ASSERT_EQ(92, b[0]);
  ASSERT_EQ(92, b[96]);
}

TEST(TestAllocator5, Constructor_2) {
  Allocator<double, 1000> b;

  ASSERT_EQ(992, b[0]);
  ASSERT_EQ(992, b[996]);
}

TEST(TestAllocator5, Constructor_3) {
  Allocator<int, 500> b;

  ASSERT_EQ(492, b[0]);
  ASSERT_EQ(492, b[496]);
}

TEST(TestAllocator5, Allocate_1) {
  Allocator<int, 100> x;
  x.allocate(1);

  ASSERT_EQ(-4, x[0]);
  ASSERT_EQ(-4, x[8]);
  ASSERT_EQ(80, x[12]);
  ASSERT_EQ(80, x[96]);
}
TEST(TestAllocator5, Allocate_2) {
  Allocator<double, 100> x;
  x.allocate(1);

  ASSERT_EQ(-8, x[0]);
  ASSERT_EQ(-8, x[12]);
  ASSERT_EQ(76, x[16]);
  ASSERT_EQ(76, x[96]);
}

TEST(TestAllocator5, Allocate_3) {
  Allocator<int, 100> b;
  b.allocate(1);
  ASSERT_EQ(-4, b[0]);
  ASSERT_EQ(-4, b[8]);
  ASSERT_EQ(80, b[12]);
  ASSERT_EQ(80, b[96]);

  b.allocate(1);

  ASSERT_EQ(-4, b[0]);
  ASSERT_EQ(-4, b[8]);
  ASSERT_EQ(-4, b[12]);
  ASSERT_EQ(-4, b[20]);
  ASSERT_EQ(68, b[24]);
  ASSERT_EQ(68, b[96]);
}

TEST(TestAllocator5, Allocate_4) {
  Allocator<int, 13> b;
  b.allocate(1);
  ASSERT_EQ(-5, b[0]);
  ASSERT_EQ(-5, b[9]);
}

TEST(TestAllocator5, Allocate_5) {
  Allocator<int, 13> b;
  bool exceptionThrown = false;
  b.allocate(1);
  try {
    b.allocate(1);
  } catch (...) {
    exceptionThrown = true;
  }
  ASSERT_EQ(exceptionThrown, true);
}

TEST(TestAllocator5, Deallocate_1) {
  /// Test for sentiniels after deallocate.
  Allocator<int, 100> b;
  b.deallocate(b.allocate(1), 1);

  ASSERT_EQ(92, b[0]);
  ASSERT_EQ(92, b[96]);
}

TEST(TestAllocator5, Deallocate_2) {
  /// Coalesce left free memory.
  Allocator<int, 100> b;
  int *p1 = b.allocate(1);
  int *p2 = b.allocate(1);
  int *p3 = b.allocate(1);

  b.deallocate(p1, 1);
  b.deallocate(p2, 1);

  ASSERT_EQ(16, b[0]);
  ASSERT_EQ(16, b[20]);

  b.deallocate(p3, 1);
}

TEST(TestAllocator5, Deallocate_3) {
  Allocator<int, 100> b;
  bool exceptionThrown = false;
  try {
    int i;
    int *p = &i;
    b.deallocate(p, 2);
  } catch (...) {
    exceptionThrown = true;
  }
  ASSERT_EQ(true, exceptionThrown);
}
