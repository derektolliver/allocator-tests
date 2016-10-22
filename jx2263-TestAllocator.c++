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

////////////////////////// Jamie and Robbie's Tests ////////////////////////////

// CONSTRUCTOR

/// Check bad_alloc exception in constructor
TEST(TestAllocator4, Constructor1) {
  try {
    my_allocator<int, 8> x;
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

/// Simple allocate that ensures correct intiial sentinels
TEST(TestAllocator4, Constructor2) {
  const my_allocator<int, 80> x;
  ASSERT_EQ(x[0], 72);
  ASSERT_EQ(x[76], 72);
}

/// Simple allocate that ensures correct intiial sentinels
TEST(TestAllocator4, Constructor3) {
  const my_allocator<double, 16> x;
  ASSERT_EQ(x[0], 8);
  ASSERT_EQ(x[12], 8);
}

// ALLOCATE

/// Test multiple allocates
TEST(TestAllocator5, Allocate1) {
  my_allocator<int, 200> x;
  x.allocate(32);
  x.allocate(32);
  x.allocate(32);
  x.allocate(32);
  x.allocate(32);
  ASSERT_TRUE(x.valid());
}

/// Test bad allocate where we allocate too much (no room for sentinels)
TEST(TestAllocator6, Allocate2) {
  try {
    my_allocator<int, 100> x;
    x.allocate(100);
    ASSERT_TRUE(false);
  } catch (std::bad_alloc &e) {
    ASSERT_TRUE(true); // pass
  }
}

/// Test bad allocate (x.allocate(0))
TEST(TestAllocator7, Allocate3) {
  try {
    my_allocator<double, 72> x;
    x.allocate(2);
    x.allocate(1);
    x.allocate(0);
    ASSERT_TRUE(false);
  } catch (std::bad_alloc &e) {
    ASSERT_TRUE(true); // pass
  }
}

/// If there's no room to allocate, allocate() returns nullptr
TEST(TestAllocator8, Allocate4) {
  my_allocator<int, 200> x;
  x.allocate(32);
  x.allocate(32);
  x.allocate(32);
  x.allocate(32);
  x.allocate(32);
  ASSERT_TRUE(x.allocate(32) == nullptr);
  ASSERT_TRUE(x.valid());
}

// DEALLOCATE

/// Test simple deallocation -- no coalescing
TEST(TestAllocator9, Deallocate1) {
  my_allocator<int, 200> x;
  int *a = x.allocate(20);
  x.deallocate(a, 20);

  ASSERT_TRUE(x[0] == x[196]);
  ASSERT_TRUE(x[0] == 192);
}

/// Test multiple coalescing -- front, back, combination
TEST(TestAllocator10, Deallocate2) {
  my_allocator<int, 19200> x;
  int *a = x.allocate(100);
  int *b = x.allocate(100);
  int *c = x.allocate(100);
  int *d = x.allocate(190);
  int *e = x.allocate(100);
  int *f = x.allocate(10);

  x.deallocate(b, 100);
  x.deallocate(c, 100);
  x.deallocate(a, 100);
  x.deallocate(d, 200);
  x.deallocate(e, 110);
  x.deallocate(f, 10);

  int *g = x.allocate(460);

  x.deallocate(g, 460);

  ASSERT_TRUE(x.allocate(3000) != nullptr);
  ASSERT_TRUE(x.valid());
}

/// Invalid deallocation test
TEST(TestAllocator11, Deallocate3) {
  my_allocator<int, 100> x;
  int *a = nullptr;
  try {
    a = x.allocate(0);
  } catch (std::bad_alloc &) {
    try {
      x.deallocate(a, 0);
      ASSERT_TRUE(false);
    } catch (std::invalid_argument &) {
      ASSERT_TRUE(true);
    }
  }
}

// VALID

/// Simple behavior test
TEST(TestAllocator12, valid1) {

  my_allocator<int, 100> x;
  int *p = x.allocate(10);
  x.deallocate(p, 10);

  ASSERT_EQ(x.valid(), true);
}

/// Simple behavior test
TEST(TestAllocator13, valid2) {

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

/// Purposely mismatching sentinels
TEST(TestAllocator14, valid3) {
  my_allocator<int, 20> x;
  x[16] = 100;
  ASSERT_EQ(x.valid(), false);
}
