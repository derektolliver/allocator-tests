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

// --------------
// TestAllocator4, 5, & 6
// --------------
// Tests valid() for mismatched or incorrect sentinels

// Test zero length block
TEST(TestAllocator4, zero) {
  my_allocator<int, 100> x;
  x[0] = 0;
  ASSERT_FALSE(x.valid());
}

// Test mismatched sentinels
TEST(TestAllocator5, mismatch) {
  my_allocator<int, 100> x;
  // Falsify a free block
  x[0] = 4;
  x[8] = 5;
  ASSERT_FALSE(x.valid());
}

// Test free block sentinels value too large
TEST(TestAllocator6, free) {
  my_allocator<int, 100> x;
  x[0] = 1000;
  x[92] = 1000;
  ASSERT_FALSE(x.valid());
}

// ----------------------
// TestAllocator7, 8, & 9
// ----------------------
// Test allocator construction which should throw an exception
// on an invalid size
TEST(TestAllocator7, constructonebyte) {
  try {
    my_allocator<int, 1> x;
  } catch (...) {
    ASSERT_TRUE(1);
    return;
  }
  ASSERT_TRUE(0);
}

TEST(TestAllocator8, constructint) {
  my_allocator<int, 20> x;
  ASSERT_EQ(x[0], 12);
}

TEST(TestAllocator9, constructdouble) {
  my_allocator<double, 20> x;
  ASSERT_EQ(x[0], 12);
}

// --------------------------
// TestAllocator 10, 11, & 12
// --------------------------
// Test that allocate correctly throws exceptions

template <typename A> struct TestAllocator10 : testing::Test {
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

TYPED_TEST_CASE(TestAllocator10, my_types_3);

TYPED_TEST(TestAllocator10, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = -1;
  const value_type v = 2;
  try {
    const pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
      x.destroy(p);
      x.deallocate(p, s);
    }
  } catch (...) {
    ASSERT_TRUE(1);
    return;
  }
  ASSERT_TRUE(0);
}

template <typename A> struct TestAllocator11 : testing::Test {
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

TYPED_TEST_CASE(TestAllocator11, my_types_4);

TYPED_TEST(TestAllocator11, test_11) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10000;
  const value_type v = 2;
  try {
    const pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
      x.destroy(p);
      x.deallocate(p, s);
    }
  } catch (...) {
    ASSERT_TRUE(1);
    return;
  }
  ASSERT_TRUE(0);
}

template <typename A> struct TestAllocator12 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_5;

TYPED_TEST_CASE(TestAllocator12, my_types_5);

TYPED_TEST(TestAllocator12, test_12) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 0;
  const value_type v = 2;
  try {
    const pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
      x.destroy(p);
      x.deallocate(p, s);
    }
  } catch (...) {
    ASSERT_TRUE(1);
    return;
  }
  ASSERT_TRUE(0);
}

// --------------------------
// TestAllocator 13, 14, & 15
// --------------------------
// Test that deallocate correctly throws exceptions

template <typename A> struct TestAllocator13 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_6;

TYPED_TEST_CASE(TestAllocator13, my_types_6);

TYPED_TEST(TestAllocator13, test_13) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    const pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
      x.destroy(p);
      x.deallocate(0, s);
    }
  } catch (...) {
    ASSERT_TRUE(1);
    return;
  }
  ASSERT_TRUE(0);
}

template <typename A> struct TestAllocator14 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_7;

TYPED_TEST_CASE(TestAllocator14, my_types_7);

TYPED_TEST(TestAllocator14, test_14) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    const pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
      x.destroy(p);
      x.deallocate((pointer)1000, s);
    }
  } catch (...) {
    ASSERT_TRUE(1);
    return;
  }
  ASSERT_TRUE(0);
}
