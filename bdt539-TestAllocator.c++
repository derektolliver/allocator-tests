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
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
  my_allocator<int, 100> y;
  ASSERT_EQ(y[0], 92);
  ASSERT_EQ(y[96], 92);
}

TEST(TestAllocator2, allocate_index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
  x.allocate(1);
  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
}

TEST(TestAllocator2, allocate_index_double) {
  my_allocator<double, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
  x.allocate(1);
  ASSERT_EQ(x[0], -8);
  ASSERT_EQ(x[12], -8);
}

TEST(TestAllocator2, allocate_index2) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
  x.allocate(5);
  ASSERT_EQ(x[0], -20);
  ASSERT_EQ(x[24], -20);
  ASSERT_EQ(x[28], 64);
  ASSERT_EQ(x[96], 64);
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

// --------------
// TestAllocator4
// --------------

TEST(TestAllocator4, test_container_undersize) {
  /* test that allocator's constructor throws bad_alloc if
   * N<(sizeof(T)+(2*sizeof(int)))
   */
  try {
    const my_allocator<double, 12> x;
    FAIL() << "Execpted bad_alloc";
  } catch (std::bad_alloc) {
    return;
  }
}

TEST(TestAllocator4, test_container_oversize) {
  /* test that allocator's constructor works for
   * N == (sizeof(T)+(2*sizeof(int)))
   */
  try {
    const my_allocator<double, 16> x;
  } catch (std::bad_alloc) {
    FAIL() << "could not construct allocator of minimum size";
  }
}

TEST(TestAllocator4, max_allocate_1) {
  /* test that allocator can allocate up to max size */
  my_allocator<int, 100> x;
  try {
    int *p = x.allocate(23);
    ASSERT_NE(p, nullptr);
  } catch (std::bad_alloc) {
    FAIL() << "could not allocate block";
  }
}

TEST(TestAllocator4, max_allocate_2) {
  /* test that allocator can allocate up to max size */
  my_allocator<double, 100> x;
  try {
    double *p = x.allocate(11);
    ASSERT_NE(p, nullptr);
  } catch (std::bad_alloc) {
    FAIL() << "could not allocate block";
  }
}

TEST(TestAllocator4, max_allocate_3) {
  /* test that allocator can allocate up to max size */
  my_allocator<int, 100> x;
  try {
    int *p1 = x.allocate(11);
    int *p2 = x.allocate(10);
    ASSERT_NE(p1, nullptr);
    ASSERT_NE(p2, nullptr);
  } catch (std::bad_alloc) {
    FAIL() << "could not allocate block";
  }
}

// --------------
// TestAllocator5
// --------------

TEST(TestAllocator5, deallocate_1) {
  /* make sure dellocate works by allocating max space after deallocation */
  my_allocator<double, 16> x;
  double *p = x.allocate(1);
  ASSERT_NE(p, nullptr);
  x.deallocate(p, 1);
  p = x.allocate(1);
  ASSERT_NE(p, nullptr);
}

TEST(TestAllocator5, deallocate_2) {
  /* make sure dellocate works by allocating max space after deallocation */
  my_allocator<double, 32> x;
  double *p = x.allocate(3);
  ASSERT_NE(p, nullptr);
  x.deallocate(p, 3);
  p = x.allocate(3);
  ASSERT_NE(p, nullptr);
}

TEST(TestAllocator5, deallocate_3) {
  /* make sure dellocate works for int type */
  my_allocator<int, 100> x;
  int *p = x.allocate(23);
  ASSERT_NE(p, nullptr);
  x.deallocate(p, 23);
  p = x.allocate(23);
  ASSERT_NE(p, nullptr);
}

TEST(TestAllocator5, deallocate_4) {
  /* make sure coalescing works */
  my_allocator<int, 100> x;
  int *p1 = x.allocate(8);
  int *p2 = x.allocate(3);
  int *p3 = x.allocate(8);
  ASSERT_NE(p1, nullptr);
  ASSERT_NE(p2, nullptr);
  ASSERT_NE(p3, nullptr);
  x.deallocate(p1, 8);
  x.deallocate(p3, 8);
  x.deallocate(p2, 3);
  int *p = x.allocate(23);
  ASSERT_NE(p, nullptr);
}

TEST(TestAllocator5, deallocate_5) {
  /* make sure coalescing works */
  my_allocator<int, 100> x;
  int *p1 = x.allocate(1);
  int *p2 = x.allocate(1);
  int *p3 = x.allocate(1);
  int *p4 = x.allocate(1);
  int *p5 = x.allocate(1);
  int *p6 = x.allocate(1);
  ASSERT_NE(p1, nullptr);
  ASSERT_NE(p2, nullptr);
  ASSERT_NE(p3, nullptr);
  x.deallocate(p1, 1);
  x.deallocate(p3, 1);
  x.deallocate(p2, 1);
  x.deallocate(p6, 1);
  x.deallocate(p4, 1);
  x.deallocate(p5, 1);
  int *p = x.allocate(23);
  ASSERT_NE(p, nullptr);
}
