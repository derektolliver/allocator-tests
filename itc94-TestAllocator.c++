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

TEST(TestAllocator2, free1) {
  my_allocator<int, 100> x;
  x.free(0, 12);
  ASSERT_EQ(x[0], 4);
}

TEST(TestAllocator2, free2) {
  my_allocator<int, 100> x;
  x.free(0, 100);
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, free3) {
  my_allocator<int, 100> x;
  x.free(0, 40);
  ASSERT_EQ(x[0], 32);
}

TEST(TestAllocator2, busy1) {
  my_allocator<int, 100> x;
  x.busy(0, 12);
  ASSERT_EQ(x[0], -4);
}

TEST(TestAllocator2, busy2) {
  my_allocator<int, 100> x;
  x.busy(0, 100);
  ASSERT_EQ(x[0], -92);
}

TEST(TestAllocator2, busy3) {
  my_allocator<int, 100> x;
  x.busy(0, 40);
  ASSERT_EQ(x[0], -32);
}

TEST(TestAllocator2, reserve1) {
  my_allocator<int, 100> x;
  x.reserve(0, 12);
  ASSERT_EQ(x[0], 4);
}

TEST(TestAllocator2, reserve2) {
  my_allocator<int, 100> x;
  x.reserve(0, 100);
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, reserve3) {
  my_allocator<int, 100> x;
  x.reserve(0, 40);
  ASSERT_EQ(x[0], 32);
}

TEST(TestAllocator2, reserve4) {
  my_allocator<int, 100> x;
  x.reserve(0, -12);
  ASSERT_EQ(x[0], -4);
}

TEST(TestAllocator2, reserve5) {
  my_allocator<int, 100> x;
  x.reserve(0, -100);
  ASSERT_EQ(x[0], -92);
}

TEST(TestAllocator2, reserve6) {
  my_allocator<int, 100> x;
  x.reserve(0, -40);
  ASSERT_EQ(x[0], -32);
}

TEST(TestAllocator2, valid_address1) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.valid_address(&x[0]));
}

TEST(TestAllocator2, valid_address2) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.valid_address(&x[1]));
}

TEST(TestAllocator2, valid_address3) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.valid_address(&x[99]));
}

TEST(TestAllocator2, valid_address4) {
  my_allocator<int, 100> x;
  ASSERT_FALSE(x.valid_address(&x[0] - 1));
}

TEST(TestAllocator2, valid_address5) {
  my_allocator<int, 100> x;
  ASSERT_FALSE(x.valid_address(&x[99] + 1));
}

TEST(TestAllocator2, allocate1) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.allocate(1) != nullptr);
}

TEST(TestAllocator2, allocate2) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.allocate(23) != nullptr);
}

TEST(TestAllocator2, allocate3) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.allocate(24) == nullptr);
}

TEST(TestAllocator2, allocate4) {
  my_allocator<int, 100> x;
  ASSERT_THROW(x.allocate(0), std::bad_alloc);
}

TEST(TestAllocator2, deallocate1) {
  my_allocator<int, 100> x;
  ASSERT_THROW(x.deallocate(&x[0] - 1, 1), std::invalid_argument);
}

TEST(TestAllocator2, deallocate2) {
  my_allocator<int, 100> x;
  // will trigger undefined behavior as deallocating a value not returned from
  // allocate
  ASSERT_THROW(x.deallocate(x.allocate(1) + 1, 1), std::invalid_argument);
}

TEST(TestAllocator2, deallocate3) {
  my_allocator<int, 100> x;
  x.deallocate(x.allocate(1), 1);
}

TEST(TestAllocator2, deallocate4) {
  my_allocator<int, 100> x;
  x.deallocate(x.allocate(10), 10);
}

TEST(TestAllocator2, valid1) {
  my_allocator<int, 100> x;
  x.allocate(1);
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator2, valid2) {
  my_allocator<int, 100> x;
  x.allocate(10);
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator2, valid3) {
  my_allocator<int, 100> x;
  x.allocate(23);
  ASSERT_TRUE(x.valid());
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
