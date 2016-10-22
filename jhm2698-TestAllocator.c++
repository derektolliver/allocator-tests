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

template <typename A> struct TestAllocator1a : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       std::allocator<char>, Allocator<int, 100>,
                       Allocator<double, 100>,
                       Allocator<char, 100>> my_types_1a;

TYPED_TEST_CASE(TestAllocator1a, my_types_1a);

TYPED_TEST(TestAllocator1a, test_1a) {
  // Checking to see if a constructor
  // throws bad_alloc exception
  try {
    Allocator<double, 12> x;
    ASSERT_EQ(true, false);
  } catch (bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator1a, test1b) {
  // Checks to see if allocate throws
  // correct exception if argument is
  // less than n
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 0;
  const value_type v = 2;

  try {
    pointer p = x.allocate(s);
  } catch (bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator1a, test1c) {
  // Checks to see if allocate throws
  // correct exception if argument is
  // greater than n, and can't find
  // free block
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 100;
  const value_type v = 2;

  try {
    pointer p = x.allocate(s);
  } catch (bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator1a, test1d) {
  // Checks to see if deallocate
  // throws exception if passed a
  // nullptr
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;

  try {
    x.deallocate(nullptr, 10);
  } catch (invalid_argument &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator1a, test1e) {
  // Checks to see you can give enough memory for two
  // allocate successive requests
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    x.deallocate(p, s);
    x.deallocate(q, s);
  } catch (invalid_argument &e) {
    ASSERT_EQ(true, true);
  }
}

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
                       Allocator<int, 100>, Allocator<double, 100>> my_types_1;

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

TYPED_TEST(TestAllocator1, test_my_1a) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
    }
    pointer q = x.allocate(s);
    if (q != nullptr) {
      x.construct(q, v);
    }

  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator1, test_my_1b) {
  // Check to see if you can deallocate allocated space
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
    }
    x.deallocate(p, s);

  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator1, test_my_1c) {
  // Coalesing to the left
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
    }
    pointer q = x.allocate(s);
    if (q != nullptr) {
      x.construct(q, v);
    }
    x.deallocate(p, s);
    x.deallocate(q, s);

  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator1, test_my_1d) {
  // Coalesing to the right
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
    }
    pointer q = x.allocate(s);
    if (q != nullptr) {
      x.construct(q, v);
    }
    x.deallocate(q, s);
    x.deallocate(p, s);

  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator1, test_my_1e) {
  // Coalesing to the right AND left
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    if (p != nullptr) {
      x.construct(p, v);
    }
    pointer q = x.allocate(s);
    if (q != nullptr) {
      x.construct(q, v);
    }
    pointer r = x.allocate(s);
    if (r != nullptr) {
      x.construct(r, v);
    }
    x.deallocate(p, s);
    x.deallocate(r, s);
    x.deallocate(q, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
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
  ASSERT_EQ(x[96], 92);

  // Test with const double
  const Allocator<double, 100> y;
  ASSERT_EQ(y[0], 92);
  ASSERT_EQ(y[96], 92);

  // Test with const char
  const Allocator<char, 100> z;
  ASSERT_EQ(z[0], 92);
  ASSERT_EQ(z[96], 92);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);

  // Test with double
  Allocator<double, 100> y;
  ASSERT_EQ(y[0], 92);
  ASSERT_EQ(y[96], 92);

  // Test with char
  Allocator<char, 100> z;
  ASSERT_EQ(z[0], 92);
  ASSERT_EQ(z[96], 92);
}

TEST(TestAllocator2, valid) {
  // Testing valid
  Allocator<int, 16> x;
  x.allocate(1);
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
