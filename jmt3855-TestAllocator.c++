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

TEST(TestAllocator2, const_index1) {
  const my_allocator<int, 1000> x;
  ASSERT_EQ(x[0], 992);
}

TEST(TestAllocator2, index1) {
  my_allocator<int, 1000> x;
  ASSERT_EQ(x[0], 992);
}

TEST(TestAllocator2, const_index2) {
  const my_allocator<int, 150> x;
  ASSERT_EQ(x[0], 142);
}

TEST(TestAllocator2, index2) {
  my_allocator<int, 150> x;
  ASSERT_EQ(x[0], 142);
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

//------------
// Test valid()
//------------

TEST(validity, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x.valid(), true);
}

TEST(validity, index1) {
  my_allocator<int, 100> x;
  int *p = x.allocate(10);
  x.deallocate(p, 10);
  ASSERT_EQ(x.valid(), true);
}

TEST(validity, index2) {
  my_allocator<int, 100> x;
  *reinterpret_cast<int *>(&x[96]) = 10;
  ASSERT_EQ(x.valid(), false);
}

//-------------
// Test allocate
//-------------

TEST(allocate, index) {
  my_allocator<int, 100> x;
  x.allocate(1);
  ASSERT_EQ(x[0], -4);
}

TEST(allocate, index1) {
  my_allocator<int, 100> x;
  x.allocate(15);
  x.allocate(1);
  ASSERT_EQ(x[0], -60);
}

TEST(allocate, index2) {
  my_allocator<int, 100> x;
  x.allocate(15);
  x.allocate(1);
  ASSERT_EQ(x[68], -4);
}

//---------------
// Test deallocate
//---------------

TEST(deallocate, index) {
  my_allocator<int, 48> x;
  int *p = x.allocate(10);
  x.deallocate(p, 10);
  ASSERT_EQ(x[0], 40);
}

TEST(deallocate, index1) {
  my_allocator<int, 100> x;
  int *p = x.allocate(2);
  x.deallocate(p, 2);

  ASSERT_EQ(x[0], 92);
}

TEST(deallocate, index2) {
  my_allocator<int, 100> x;
  int *p = x.allocate(2);
  ASSERT_EQ(x[0], -8);
  ASSERT_EQ(x[12], -8);
  x.deallocate(p, 2);
  my_allocator<int, 100> y = x;
  ASSERT_EQ(y[0], 92);
  ASSERT_EQ(y[96], 92);
}
