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
#include <vector>
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
  my_allocator<int, 50> x;
  ASSERT_EQ(x[0], 42);
}

TEST(TestAllocator2, index_2) {
  my_allocator<int, 49> x;
  ASSERT_EQ(x[0], 41);
}
TEST(TestAllocator2, index_3) {
  my_allocator<int, 33> x;
  ASSERT_EQ(x[0], 25);
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

TEST(TestAllocator4, allocate_int) {

  my_allocator<int, 50> x;
  x.allocate(1);

  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], 30);
  ASSERT_EQ(x[46], 30);
}

TEST(TestAllocator4, allocate_double) {

  my_allocator<double, 50> x;
  x.allocate(2);

  ASSERT_EQ(x[0], -16);
  ASSERT_EQ(x[20], -16);
  ASSERT_EQ(x[24], 18);
  ASSERT_EQ(x[46], 18);
}

TEST(TestAllocator4, allocate_multiple) {
  my_allocator<int, 100> x;

  x.allocate(1);
  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], 80);
  ASSERT_EQ(x[96], 80);

  x.allocate(2);
  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], -8);
  ASSERT_EQ(x[24], -8);
  ASSERT_EQ(x[28], 64);
  ASSERT_EQ(x[96], 64);
}
TEST(TestAllocator4, allocate_bad_alloc) {
  bool error_occured = false;
  my_allocator<int, 40> x;
  try {
    x.allocate(25);
  } catch (bad_alloc &e) {
    error_occured = true;
  }
  ASSERT_EQ(error_occured, true);
}
TEST(TestAllocator4, allocate_invalid_size) {
  my_allocator<int, 12> x;
  ASSERT_EQ(x.allocate(0), nullptr);
}

TEST(TestAllocator5, valid1) {
  my_allocator<int, 50> x;
  x[0] = 10;
  x[14] = -10;
  ASSERT_EQ(x.valid(), false);
}

TEST(TestAllocator5, valid2) {
  my_allocator<int, 50> x;
  x[0] = -4;
  x[8] = -4;
  x[12] = 30;
  x[46] = 30;
  ASSERT_EQ(x.valid(), true);
}
TEST(TestAllocator5, valid3) {
  my_allocator<int, 50> x;
  x[0] = -4;
  x[8] = -4;
  x[12] = -4;
  x[20] = -4;
  x[24] = 18;
  x[46] = 18;
  ASSERT_EQ(x.valid(), true);
}

TEST(TestAllocator6, deallocate_1) {
  my_allocator<int, 100> x;

  int *p1 = x.allocate(3);
  int *p2 = x.allocate(10);

  x.deallocate(p1, 1);
  x.deallocate(p2, 1);
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator6, deallocate_2) {
  my_allocator<int, 100> x;
  int *p = x.allocate(1);

  x.deallocate(p, 1);
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator6, deallocate_3) {
  my_allocator<int, 100> x;
  vector<int *> v;
  for (int i = 0; i < 5; i++) {
    v.push_back(x.allocate(1));
  }

  for (int i = 0; i < v.size(); i++) {
    x.deallocate(v[i], 1);
  }

  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}
