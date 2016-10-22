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

/* Nicole and Adam's Tests*/

template <typename A> struct TestAllocateDouble : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<double, 100>> my_double_allocate;

TYPED_TEST_CASE(TestAllocateDouble, my_double_allocate);

template <typename A> struct TestAllocateInt : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<int, 100>> my_int_allocate;

TYPED_TEST_CASE(TestAllocateInt, my_int_allocate);

// --------------
// Allocate Test 1
// --------------
TYPED_TEST(TestAllocateInt, test_allocate1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);

  const int left_sentinel = abs(*(reinterpret_cast<int *>(p) - 1));

  int type_size = sizeof(int);
  ASSERT_EQ(left_sentinel, type_size);
}

// --------------
// Allocate Test 2
// --------------
TYPED_TEST(TestAllocateDouble, test_allocate2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);

  const int left_sentinel = abs(*(reinterpret_cast<int *>(p) - 1));

  int type_size = sizeof(double);
  ASSERT_EQ(left_sentinel, type_size);
}

// --------------
// Allocate Test 3 - throw exceptions
// --------------
TYPED_TEST(TestAllocateInt, test_allocateexcept) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  allocator_type x;
  const size_type s = 90;
  const value_type v = 2;

  ASSERT_THROW(x.allocate(s), bad_alloc);
}

TYPED_TEST(TestAllocateInt, test_allocateexcept2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  allocator_type x;
  const size_type s = 0;
  const value_type v = 2;

  ASSERT_THROW(x.allocate(s), bad_alloc);
}

TYPED_TEST(TestAllocateInt, test_allocateexcept3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  allocator_type x;
  const size_type s = -5;
  const value_type v = 2;

  ASSERT_THROW(x.allocate(s), bad_alloc);
}

TYPED_TEST(TestAllocateInt, test_allocateexcept4) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  allocator_type x;
  const size_type s = 23;
  const value_type v = 2;

  ASSERT_NO_THROW(x.allocate(s));
}

TYPED_TEST(TestAllocateInt, test_allocateexcept5) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  allocator_type x;
  const size_type s = 20;
  const value_type v = 2;
  x.allocate(s);
  ASSERT_THROW(x.allocate(s), bad_alloc);
}

TYPED_TEST(TestAllocateInt, test_allocateexcept6) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const value_type v = 2;
  const pointer p = x.allocate(5);
  const pointer q = x.allocate(5);
  const pointer r = x.allocate(5);

  if (p != nullptr && q != nullptr && r != nullptr) {
    x.construct(r, v);
    x.destroy(r);
    x.deallocate(r, 5);
  }

  ASSERT_NO_THROW(x.allocate(9));
}

TYPED_TEST(TestAllocateInt, test_allocateexcept7) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const value_type v = 2;
  const pointer p = x.allocate(5);
  const pointer q = x.allocate(5);
  const pointer r = x.allocate(5);

  if (p != nullptr && q != nullptr && r != nullptr) {
    x.construct(r, v);
    x.destroy(r);
    x.deallocate(r, 5);
  }

  ASSERT_THROW(x.allocate(10), bad_alloc);
}

// --------------
// Deallocate Test 1
// --------------
TYPED_TEST(TestAllocateDouble, deallocate1) {
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
    const int left_sentinel = abs(*(reinterpret_cast<int *>(p) - 1));
    ASSERT_EQ(92, left_sentinel);
  }
}

// --------------
// Deallocate Test 2
// --------------
TYPED_TEST(TestAllocateDouble, deallocate2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  const pointer r = x.allocate(s);
  const pointer q = x.allocate(s);

  if (p != nullptr && q != nullptr && r != nullptr) {
    x.construct(p, v);
    x.construct(r, v);
    x.construct(q, v);
    ASSERT_EQ(v, *p);
    ASSERT_EQ(v, *r);
    ASSERT_EQ(v, *q);

    x.destroy(p);
    x.deallocate(p, s);
    x.destroy(r);
    x.deallocate(r, s);
    const int left_sentinel = abs(*(reinterpret_cast<int *>(p) - 1));
    ASSERT_EQ(24, left_sentinel);
  }
}

// --------------
// Deallocate Test 3 - Double deallocation
// --------------
TYPED_TEST(TestAllocateDouble, deallocate3) {
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
    x.deallocate(p, s);
    const int left_sentinel = abs(*(reinterpret_cast<int *>(p) - 1));
    ASSERT_EQ(92, left_sentinel);
  }
}

// --------------
// TestAllocator Valid
// --------------
