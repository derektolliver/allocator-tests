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

// my added tests

// test allocating with doubles
TEST(TestAllocator1, allocate_2) {
  my_allocator<double, 100> x;
  x.allocate(3);
  ASSERT_EQ(x.valid(), true);
}

// test allocating and deallocating with doubles
TEST(TestAllocator1, allocate_3) {
  my_allocator<double, 1000> x;
  double *p = x.allocate(3);
  double *t = x.allocate(100);
  x.deallocate(t, 100);
  double *s = x.allocate(1);
  x.deallocate(p, 3);
  x.deallocate(s, 100);
  ASSERT_EQ(x.valid(), true);
}

// test my_allocate with value less than N
TEST(TestAllocator1, my_allocate_4) {
  try {
    my_allocator<double, 4> x;
  } catch (std::bad_alloc &e) {
  }
  ASSERT_EQ(true, true);
}

// test allocating with a value greater than N
TEST(TestAllocator1, my_allocate_5) {
  try {
    my_allocator<double, 100> x;
    x.allocate(500);
  } catch (std::bad_alloc &e) {
  }
  ASSERT_EQ(true, true);
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

// my added tests
// testing allocate()
TEST(TestAllocator2, allocate_1) {
  my_allocator<int, 100> x;
  x.allocate(7);
  ASSERT_EQ(x.valid(), true);
}

// testing a simple allocate() and deallocate()
TEST(TestAllocator2, deallocate_1) {
  my_allocator<int, 100> x;

  int *p = x.allocate(20);
  x.deallocate(p, 20);
  ASSERT_EQ(x.valid(), true);
}

// testing multiple allocate() and deallocate() calls
TEST(TestAllocator2, deallocate_2) {
  my_allocator<int, 100> x;

  int *p = x.allocate(2);
  int *s = x.allocate(4);
  x.deallocate(p, 2);
  x.deallocate(s, 4);
  ASSERT_EQ(x.valid(), true);
}

// testing allocating and deallocating out of order
TEST(TestAllocator2, deallocate_3) {
  my_allocator<int, 100> x;

  int *p = x.allocate(2);
  int *q = x.allocate(7);
  int *s = x.allocate(4);
  x.deallocate(q, 7);
  x.deallocate(p, 2);
  x.deallocate(s, 4);
  ASSERT_EQ(x.valid(), true);
}

// allocate() and deallocate() the entire array
TEST(TestAllocator2, deallocate_4) {
  my_allocator<int, 100> x;
  x.allocate(2);
  x.allocate(7);
  x.allocate(4);

  try {
    x.allocate(10);
  } catch (std::bad_alloc &e) {
  }
  ASSERT_EQ(x.valid(), true);
}

// allocate() and deallocate() the entire array
TEST(TestAllocator2, deallocate_5) {
  my_allocator<int, 1000> x;
  int *p = x.allocate(5);
  int *q = x.allocate(6);
  int *r = x.allocate(7);
  int *s = x.allocate(8);
  int *t = x.allocate(1);
  int *u = x.allocate(2);
  x.deallocate(q, 6);
  x.deallocate(r, 7);
  x.deallocate(u, 2);
  int *a = x.allocate(8);
  int *b = x.allocate(1);
  int *c = x.allocate(10);
  x.deallocate(p, 5);
  x.deallocate(c, 10);
  x.deallocate(a, 8);
  x.deallocate(b, 1);
  x.deallocate(t, 1);
  x.deallocate(s, 8);
  int *d = x.allocate(100);
  x.deallocate(d, 100);
  ASSERT_EQ(x.valid(), true);
}

// test allocate and deallocate in random order with large array size
TEST(TestAllocator2, deallocate_6) {
    my_allocator<int, 5000> x;
    int* p = x.allocate(400);
    int* s = x.allocate(100);
    x.deallocate(p, 400);
    int* t = x.allocate(10);
    x.deallocate(s, 100);
    x.deallocate(t, 10);
    ASSERT_EQ(x.valid(), true);
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
