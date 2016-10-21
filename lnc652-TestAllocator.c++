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

///////////////////////////////
//                           //
//     *** My Tests ***      //
//                           //
///////////////////////////////

/* allocator constructor tests */
TEST(TestAllocator4, ctest_1) {
  try {
    const my_allocator<int, 88> x;
    ASSERT_EQ(x[0], 80);
    ASSERT_EQ(x[84], 80);
  } catch (std::bad_alloc &ba) {
    FAIL();
  }
}

TEST(TestAllocator4, ctest_2) {
  try {
    const my_allocator<double, 16> x;
    ASSERT_EQ(x[0], 8);
    ASSERT_EQ(x[12], 8);
  } catch (std::bad_alloc &ba) {
    FAIL();
  }
}

TEST(TestAllocator4, ctest_3) {
  try {
    const my_allocator<int, 5> x;
    FAIL();
  } catch (std::bad_alloc &ba) {
    SUCCEED();
  }
}

/* allocate() tests */
TYPED_TEST(TestAllocator3, atest_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 8;
  try {
    const pointer p = x.allocate(s);
    ASSERT_EQ(p[0], 0);
  } catch (std::bad_alloc &ba) {
    FAIL();
  }
}

TYPED_TEST(TestAllocator3, atest_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 500;
  try {
    const pointer p = x.allocate(s);
    FAIL();
  } catch (std::bad_alloc &ba) {
    SUCCEED();
  }
}

TYPED_TEST(TestAllocator3, atest_3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  typedef typename TestFixture::value_type value_type;
  allocator_type x;
  const size_type s = 92 / sizeof(value_type);
  try {
    const pointer p = x.allocate(s);
    ASSERT_EQ(p[0], 0);
  } catch (std::bad_alloc &ba) {
    FAIL();
  }
}

/* deallocate() tests */
TYPED_TEST(TestAllocator3, dtest_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  typedef typename TestFixture::value_type value_type;
  allocator_type x;
  const size_type s = 92 / sizeof(value_type);
  const pointer p = x.allocate(s);
  try {
    x.deallocate(p, s);
    SUCCEED();
  } catch (std::invalid_argument &ia) {
    FAIL();
  }
}

TYPED_TEST(TestAllocator3, dtest_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 1;
  const pointer p1 = x.allocate(s);
  const pointer p2 = x.allocate(s);
  const pointer p3 = x.allocate(s);

  try {
    x.deallocate(p2, s);
    x.deallocate(p3, s);
    x.deallocate(p1, s);
    SUCCEED();

  } catch (std::invalid_argument &ia) {
    FAIL();
  }
}

TYPED_TEST(TestAllocator3, dtest_3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  typedef typename TestFixture::value_type value_type;
  allocator_type x;
  const size_type s = 4;
  const pointer p1 = x.allocate(s);

  try {
    x.deallocate(p1, s);
    x.deallocate(p1, s);
    FAIL();

  } catch (std::invalid_argument &ia) {
    SUCCEED();
  }
}

/* valid() tests */

TEST(TestAllocator5, vtest_1) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator5, vtest_2) {
  my_allocator<int, 100> x;
  x[0] = 2;
  ASSERT_TRUE(!x.valid());
}

TEST(TestAllocator5, vtest_3) {
  const size_t s = 1;
  my_allocator<int, 100> x;
  x.allocate(s);
  x[0] *= -1;
  x[2 * sizeof(int)] *= -1;
  ASSERT_TRUE(!x.valid());
}
