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
  x.construct(p, v);
  ASSERT_EQ(v, *p);
  x.destroy(p);
  x.deallocate(p, s);
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

TEST(TestAllocator2, test3) {
  my_allocator<int, 100> x;
  int *p = x.allocate(23);
  x.deallocate(p, 23);
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
  x.construct(p, v);
  ASSERT_EQ(v, *p);
  x.destroy(p);
  x.deallocate(p, s);
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

// --------------
// TestAllocator4
// --------------

TEST(TestAllocator4, test1) {
  my_allocator<int, 300> x;
  const size_t s = 4;
  int *p1 = x.allocate(s);
  int num = *(p1 + s + 1);
  ASSERT_EQ(num, 268);
  x.deallocate(p1, s);
}

TEST(TestAllocator4, test2) {
  my_allocator<int, 300> x;
  const size_t s = 2;
  int *p1 = x.allocate(s);
  int *p2 = x.allocate(s + 1);
  x.deallocate(p1, s);
  int *p3 = x.allocate(s + 1);
  x.deallocate(p2, s + 1);
  x.deallocate(p3, s + 1);
  ASSERT_EQ(x[0], 292);
}

TEST(TestAllocator4, test3) {
  my_allocator<int, 300> x;
  const size_t s = 4;
  int *p1 = x.allocate(s);
  ASSERT_EQ(x[300 - sizeof(int)], 268);
  x.deallocate(p1, s);
}

TEST(TestAllocator4, test4) {
  my_allocator<double, 500> x;
  const size_t s = 4;
  double *p1 = x.allocate(s);
  double *p2 = x.allocate(s + 2);
  ASSERT_EQ(x[500 - sizeof(int)], 396);
  x.deallocate(p1, s);
  x.deallocate(p2, s + 2);
}

TEST(TestAllocator4, test5) {
  my_allocator<int, 72> x;
  int *p1 = x.allocate(3);
  int *p2 = x.allocate(5);
  int *p3 = x.allocate(4);
  x.deallocate(p2, 5);
  p2 = x.allocate(3);
  x.deallocate(p3, 4);
  x.deallocate(p2, 3);
  x.deallocate(p1, 3);
}

TEST(TestAllocator4, test6) {
  my_allocator<int, 96> x;
  int *p1 = x.allocate(3);
  int *p2 = x.allocate(5);
  int *p3 = x.allocate(4);
  int *p4 = x.allocate(4);
  x.deallocate(p2, 5);
  x.deallocate(p3, 4);
  p2 = x.allocate(10);
  x.deallocate(p2, 3);
  x.deallocate(p1, 3);
  x.deallocate(p4, 4);
}

// --------------
// TestAllocator5
// --------------

TEST(TestAllocator5, test1) {
  my_allocator<double, 500> x;
  const size_t s = 4;
  double *p1 = x.allocate(s);
  double *p2 = x.allocate(s + 2);
  x.deallocate(p1, s);
  ASSERT_EQ(x[sizeof(int) + s * sizeof(double)], 32);
  x.deallocate(p2, s + 2);
}

TEST(TestAllocator5, test2) {
  my_allocator<double, 500> x;
  const size_t s = 4;
  double *p1 = x.allocate(s);
  double *p2 = x.allocate(s + 2);
  double *p3 = x.allocate(s + 2);
  x.deallocate(p1, s);
  p1 = x.allocate(s);
  x.deallocate(p3, s + 2);
  x.deallocate(p1, s);
  x.deallocate(p2, s + 2);
  ASSERT_EQ(x[0], 492);
}

TEST(TestAllocator5, test3) {
  my_allocator<double, 32> x;
  const size_t s = 3;
  double *p1 = x.allocate(s);
  x.deallocate(p1, s);
  ASSERT_EQ(*(reinterpret_cast<int *>(p1) - 1), 32 - sizeof(int) * 2);
}

TEST(TestAllocator5, test4) {
  my_allocator<double, 500> x;
  const size_t s = 4;
  double *p1 = x.allocate(s);
  double *p2 = x.allocate(s + 2);
  double *p3 = x.allocate(s + 2);
  x.deallocate(p2, s + 2);
  x.deallocate(p1, s);
  x.deallocate(p3, s + 2);
  ASSERT_EQ(*(reinterpret_cast<int *>(p1) - 1), 492);
}

// --------------
// TestAllocator6
// --------------

template <typename A> struct TestAllocator6 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       my_allocator<int, 200>, my_allocator<double, 200>>
    my_types_3;

TYPED_TEST_CASE(TestAllocator6, my_types_3);

// Test random allocates and deallocates.
TYPED_TEST(TestAllocator6, test_7) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const int n = 5;
  const value_type v = 2;
  pointer p[n];
  for (int i = 0; i < n; i++) {
    p[i] = x.allocate(s);
    x.construct(p[i], v);
  }
  x.destroy(p[0]);
  x.deallocate(p[0], s);
  x.destroy(p[3]);
  x.deallocate(p[3], s);
  x.destroy(p[4]);
  x.deallocate(p[4], s);
  p[4] = 0;

  p[3] = x.allocate(s + 1);
  p[0] = x.allocate(s);
  x.construct(p[0], v);
  x.construct(p[3], v);
  x.construct(p[3] + 1, v);
  p[4] = p[3] + 1;

  bool all_true = true;
  for (int j = 0; j < n; ++j) {
    if (*p[j] != v) {
      all_true = false;
      break;
    }
  }
  ASSERT_TRUE(all_true);
  for (int k = 0; k < n; ++k) {
    if (k == 3) {
      x.destroy(p[k]);
      x.destroy(p[k + 1]);
      x.deallocate(p[k], s + 1);
      ++k;
    } else {
      x.destroy(p[k]);
      x.deallocate(p[k], s);
    }
  }
}

TYPED_TEST(TestAllocator6, test_8) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const int n = 8;
  pointer p[n];

  for (int i = 0; i < n; ++i) {
    p[i] = x.allocate(s);
  }

  for (int j = 0; j < n; ++j) {
    x.deallocate(p[j], s);
  }

  for (int i = 0; i < n; ++i) {
    p[i] = x.allocate(s);
  }

  for (int j = n - 1; j >= 0; --j) {
    x.deallocate(p[j], s);
  }
}
