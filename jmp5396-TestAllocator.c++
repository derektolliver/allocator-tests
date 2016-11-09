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
                       my_allocator<int, 100>,
                       my_allocator<double, 100>> my_types_1;

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

TEST(TestAllocator4, constructor_1) {
  my_allocator<int, 1000> x;
  ASSERT_EQ(x[0], 992);
  ASSERT_EQ(x[996], 992);
}

TEST(TestAllocator4, constructor_2) {
  my_allocator<int, 1618> x;
  ASSERT_EQ(x[0], 1610);
  ASSERT_EQ(x[1614], 1610);
}

TEST(TestAllocator4, constructor_3) {
  my_allocator<int, 11996> x;
  ASSERT_EQ(x[0], 11988);
  ASSERT_EQ(x[11992], 11988);
}

TEST(TestAllocator5, valid_1) {
  /* Checks for mismatching sentinels. */
  my_allocator<int, 100> x;
  x[0] = 12;
  x[16] = 11;
  x[20] = 80;
  x[96] = 80;
  ASSERT_FALSE(x.valid());
}

TEST(TestAllocator5, valid_2) {
  /* Checks for validity of uncoalesced free blocks */
  my_allocator<int, 100> x;
  x[0] = 12;
  x[16] = 12;
  x[20] = 72;
  x[96] = 72;
  ASSERT_FALSE(x.valid());
}

TEST(TestAllocator5, valid_3) {
  /* Checks for the possibility of free blocks before occupied blocks */
  my_allocator<int, 100> x;
  x[0] = 12;
  x[16] = 12;
  x[20] = -72;
  x[96] = -72;
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator6, allocate_1) {
  my_allocator<int, 100> x;
  x.allocate(2);
  ASSERT_EQ(x[0], -8);
  ASSERT_EQ(x[12], -8);
  ASSERT_EQ(x[16], 76);
  ASSERT_EQ(x[96], 76);
}

TEST(TestAllocator6, allocate_2) {
  /* Test for multiple allocations */
  my_allocator<int, 100> x;
  x.allocate(2);
  x.allocate(1);
  x.allocate(3);
  ASSERT_EQ(x[0], -8);
  ASSERT_EQ(x[12], -8);
  ASSERT_EQ(x[16], -4);
  ASSERT_EQ(x[24], -4);
  ASSERT_EQ(x[28], -12);
  ASSERT_EQ(x[44], -12);
  ASSERT_EQ(x[48], 44);
  ASSERT_EQ(x[96], 44);
}

TEST(TestAllocator6, allocate_3) {
  /* Test for "celing" allocation */
  my_allocator<double, 100> x;
  x.allocate(2);
  x.allocate(7);
  ASSERT_EQ(x[0], -16);
  ASSERT_EQ(x[20], -16);
  ASSERT_EQ(x[24], -68);
  ASSERT_EQ(x[96], -68);
}

TEST(TestAllocator6, allocate_4) {
  /* Test for allocation with no space */
  my_allocator<double, 100> x;
  ASSERT_THROW(x.allocate(12), std::bad_alloc);
}

TEST(TestAllocator6, allocate_5) {
  /* Test for null allocation */
  my_allocator<int, 100> x;
  ASSERT_THROW(x.allocate(0), std::bad_alloc);
}

TEST(TestAllocator7, deallocate_1) {
  my_allocator<int, 100> x;
  int *f = x.allocate(1);
  int *s = x.allocate(1);
  x.allocate(1);

  x.deallocate(f, 1);
  x.deallocate(s, 1);

  ASSERT_EQ(x[0], 16);
  ASSERT_EQ(x[20], 16);
  ASSERT_EQ(x[24], -4);
  ASSERT_EQ(x[32], -4);
  ASSERT_EQ(x[36], 56);
  ASSERT_EQ(x[96], 56);
}

TEST(TestAllocator7, deallocate_2) {
  my_allocator<int, 100> x;
  x.allocate(1);
  int *s = x.allocate(1);
  int *t = x.allocate(1);

  x.deallocate(t, 1);
  x.deallocate(s, 1);

  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], 80);
  ASSERT_EQ(x[96], 80);
}

TEST(TestAllocator7, deallocate_3) {
  my_allocator<int, 100> x;
  int *f = x.allocate(1);
  int *s = x.allocate(1);
  int *t = x.allocate(1);

  x.deallocate(f, 1);
  x.deallocate(t, 1);
  x.deallocate(s, 1);

  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator7, deallocate_4) {
  my_allocator<int, 100> x;
  int *f = x.allocate(1);

  ASSERT_THROW(x.deallocate(f - 5, 1), std::invalid_argument);
}

TEST(TestAllocator7, deallocate_6) {
  my_allocator<int, 100> x;
  x.allocate(1);

  ASSERT_THROW(x.deallocate(&x[16], 1), std::invalid_argument);
}

TEST(TestAllocator7, deallocate_7) {
  my_allocator<int, 100> x;
  int *f = x.allocate(1);

  ASSERT_THROW(x.deallocate(f + 5, 1), std::invalid_argument);
}
