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
// Test allocate()
// --------------

template <typename A> struct Test_Allocate : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_3;

TYPED_TEST_CASE(Test_Allocate, my_types_3);

TYPED_TEST(Test_Allocate, test_allocate_zero) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 0;
  const value_type v = 2;
  const pointer p = x.allocate(s);

  // Check that we can't allocate <= 0
  ASSERT_EQ(p, nullptr);
}

TYPED_TEST(Test_Allocate, test_allocate_bad) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1000;
  const value_type v = 2;

  // Expect exception when allocating too much
  EXPECT_THROW({ const pointer p = x.allocate(10000); }, std::bad_alloc);
}

TYPED_TEST(Test_Allocate, test_allocate_write) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer p = x.allocate(s);

  *p = 120;
  *(p + 1) = 150;

  // Check to see if we can write to allocated space
  ASSERT_EQ(*p, 120);
  ASSERT_EQ(*(p + 1), 150);
}

// --------------
// Test deallocate()
// --------------

template <typename A> struct Test_Deallocate : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_3;

TYPED_TEST_CASE(Test_Deallocate, my_types_3);

TYPED_TEST(Test_Deallocate, test_deallocate_null) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;

  allocator_type x;
  const size_type s = 10;

  // Expect exception when deallocating null
  EXPECT_THROW({ x.deallocate(nullptr, s); }, std::invalid_argument);
}

TYPED_TEST(Test_Deallocate, test_sentinel) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer p = x.allocate(s);

  // Check that sentinel flag is negative
  ASSERT_LE(*(int *)((uint64_t)p - 4), 0);

  x.deallocate(p, s);

  // Check that sentinel flag is positive
  ASSERT_GE(*(int *)((uint64_t)p - 4), 0);
}

TYPED_TEST(Test_Deallocate, test_coalesce) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  pointer p = x.allocate(s);
  pointer p2 = x.allocate(s);
  pointer p3 = x.allocate(s);

  x.deallocate(p, s);
  x.deallocate(p2, s);
  x.deallocate(p3, s);

  // Check that deallocate properly coalesced
  // (It should show that there is a free sentinel of 92
  // after all the deallocates)
  ASSERT_EQ(*(int *)((uint64_t)p - 4), 92);
}

TYPED_TEST(Test_Deallocate, test_coalesce2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  pointer p = x.allocate(s);
  pointer p2 = x.allocate(s);
  pointer p3 = x.allocate(s);
  pointer p4 = x.allocate(s);

  x.deallocate(p3, s);
  x.deallocate(p2, s);
  x.deallocate(p4, s);
  x.deallocate(p, s);

  // Check that deallocate properly coalesced
  // (It should show that there is a free sentinel of 92
  // after all the deallocates)
  ASSERT_EQ(*(int *)((uint64_t)p - 4), 92);
}

TYPED_TEST(Test_Deallocate, test_coalesce3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  pointer p = x.allocate(s);
  pointer p2 = x.allocate(s);
  pointer p3 = x.allocate(s);
  pointer p4 = x.allocate(s);

  x.deallocate(p2, s);
  x.deallocate(p4, s);
  x.deallocate(p3, s);
  x.deallocate(p, s);

  pointer p5 = x.allocate(s);
  x.deallocate(p5, s);

  // Check that deallocate properly coalesced
  // (It should show that there is a free sentinel of 92
  // after all the deallocates)
  ASSERT_EQ(*(int *)((uint64_t)p - 4), 92);
}

// --------------
// Test my_allocator constructor()
// --------------

template <typename A> struct Test_Allocator_Constructor : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_4;

TYPED_TEST_CASE(Test_Allocator_Constructor, my_types_4);

TYPED_TEST(Test_Allocator_Constructor, test_sentinel_left) {
  typedef typename TestFixture::allocator_type allocator_type;

  const allocator_type x;

  // Check constructor set the left sentinel properly
  ASSERT_EQ(x[0], 92);
}

TYPED_TEST(Test_Allocator_Constructor, test_sentinel_right) {
  typedef typename TestFixture::allocator_type allocator_type;

  const allocator_type x;

  // Check constructor set the right sentinel properly
  ASSERT_EQ(x[96], 92);
}

TEST(Test_Allocator_Constructor_2, test_bad_alloc) {

  try {
    my_allocator<int, 1> x;

    // Didn't throw bad_alloc (failed test)
    ASSERT_EQ(0, 1);
  } catch (std::bad_alloc const &err) {
    // Caught bad_alloc (passed test)
  }
}

TEST(Test_Is_Valid, test_valid_simple) {

  my_allocator<int, 100> x;
  int *p = x.allocate(10);
  x.deallocate(p, 10);

  ASSERT_EQ(x.valid(), true);
}

TEST(Test_Is_Valid, test_valid_complex) {

  my_allocator<int, 1000> x;
  int *a = x.allocate(10);
  int *b = x.allocate(20);
  int *c = x.allocate(30);
  int *d = x.allocate(40);

  x.deallocate(b, 20);
  x.deallocate(d, 40);
  x.deallocate(c, 30);

  int *e = x.allocate(5);
  int *f = x.allocate(6);

  *a = 1;
  *f = 2;

  x.deallocate(e, 5);

  ASSERT_EQ(x.valid(), true);
}

TEST(Test_Is_Valid, test_valid_fail) {

  my_allocator<int, 20> x;

  // Change last sentinel to invalidate heap
  x[16] = 100;

  ASSERT_EQ(x.valid(), false);
}