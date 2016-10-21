// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Fernando Mendoza
// ------------------------------------

// shout out to the multiple people in the test repo
// from which I got the ideas for some of this structure.

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

typedef testing::Types<Allocator<char, 100>, Allocator<int, 100>,
                       Allocator<double, 100>>
    my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);
TYPED_TEST(TestAllocator1, valid_arguments_test) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);

  // Are we checking for null pointer?
  // credit to Aaron for showing how to check with try/catch
  try {
    x.deallocate(nullptr, 10);
    ASSERT_EQ(true, false);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator1, allocate_exception_test) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10000;
  pointer p;

  try {
    p = x.allocate(s);
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

// check constructor arguments
template <typename A> struct TestAllocatorNT : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<char, 27>> my_types_NT;
TYPED_TEST_CASE(TestAllocatorNT, my_types_NT);
TYPED_TEST(TestAllocatorNT, bad_constructor_test) {
  // Are we constructing correctly?
  try {
    Allocator<long, 2> bad_alloc_check;
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

template <typename A> struct TestAllocator2 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<char, 1000>, Allocator<int, 1000>,
                       Allocator<double, 1000>>
    my_types_2;

TYPED_TEST_CASE(TestAllocator2, my_types_2);
TYPED_TEST(TestAllocator2, valid_arguments_test) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 120;
  const pointer p = x.allocate(s);

  // Are we checking for null pointer?
  x.construct(p, v);
  value_type res = *p;
  x.destroy(p);
  x.deallocate(p, s);
  ASSERT_EQ(res, v);
}

TYPED_TEST(TestAllocator2, multiple_allocate_test0) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 0;
  pointer p;
  p = x.allocate(s);
  x.deallocate(p, s);
  p = x.allocate(s * 2);
  x.deallocate(p, s);
  p = x.allocate(10);
  x.deallocate(p, s);
  p = x.allocate(s);

  x.construct(p, v);
  ASSERT_EQ(*p, v);
}

TYPED_TEST(TestAllocator2, multiple_allocate_test1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 500;
  const value_type v = 0;
  pointer p, q, r;
  try {
    p = x.allocate(3);
    q = x.allocate(4);
    r = x.allocate(5);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

template <typename A> struct TestAllocator3 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<char, 200>, Allocator<int, 200>,
                       Allocator<double, 200>>
    my_types_3;
TYPED_TEST_CASE(TestAllocator3, my_types_3);

TYPED_TEST(TestAllocator3, left_coalesce_test) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  pointer p, q;
  const size_type s = 2;

  p = x.allocate(s);
  q = x.allocate(s);
  x.deallocate(p, s);
  x.deallocate(q, s);
  p = x.allocate(s);
}

TYPED_TEST(TestAllocator3, right_coalesce_test) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  pointer p, q;
  const size_type s = 2;

  p = x.allocate(s);
  q = x.allocate(s);
  x.deallocate(q, s);
  x.deallocate(p, s);
  p = x.allocate(s);
}

TYPED_TEST(TestAllocator3, dual_coalesce_test) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  pointer p, q, r;
  const size_type s = 2;

  p = x.allocate(s);
  q = x.allocate(s);
  r = x.allocate(s);
  x.deallocate(r, s);
  x.deallocate(p, s);
  r = x.allocate(s);
  x.deallocate(q, s);
  q = x.allocate(s);
  p = x.allocate(s);
}

// credit to Glenn Downing
TYPED_TEST(TestAllocator3, downing_test_10) {
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
