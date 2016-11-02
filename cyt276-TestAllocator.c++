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
#include <numeric>   //iota
#include <vector>

#include "gtest/gtest.h"

#include "Allocator.h"

// ---------
// OurTests
//----------

template <typename A> struct TestAllocator0 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_0;

TYPED_TEST_CASE(TestAllocator0, my_types_0);

// Allocator invalid
TYPED_TEST(TestAllocator0, test_0) {
  try {
    my_allocator<double, 6> x;
    ASSERT_EQ(1, 0);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}

// Allocate invalid
TYPED_TEST(TestAllocator0, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 100;
  try {
    pointer p = x.allocate(s);
    ASSERT_EQ(1, 0);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}

// Deallocate null pointer
TYPED_TEST(TestAllocator0, test_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  allocator_type x;
  try {
    x.deallocate(nullptr, 13);
    ASSERT_EQ(1, 0);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

// Deallocate invalid pointer
TYPED_TEST(TestAllocator0, test_3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::pointer pointer;
  typedef typename TestFixture::size_type size_type;
  allocator_type x;
  const size_type s = 100;
  try {
    pointer p = x.allocate(s);
    x.deallocate(p + 1, s);
    ASSERT_EQ(1, 0);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}

// Deallocate invalid pointer 2
TYPED_TEST(TestAllocator0, test_4) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::pointer pointer;
  typedef typename TestFixture::size_type size_type;
  allocator_type x;
  const size_type s = 100;
  try {
    pointer p = x.allocate(s);
    x.deallocate(p - 1, s);
    ASSERT_EQ(1, 0);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}

// Multiple allocations
TYPED_TEST(TestAllocator0, test_5) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 4;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

// Left Coallescing
TYPED_TEST(TestAllocator0, test_6) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 4;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    x.deallocate(p, s);
    x.deallocate(q, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

// Right Coallescing
TYPED_TEST(TestAllocator0, test_7) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 4;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    x.deallocate(q, s);
    x.deallocate(p, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

// Coallesce variety 1
TYPED_TEST(TestAllocator0, test_8) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    pointer r = x.allocate(s);
    x.deallocate(r, s);
    x.deallocate(p, s);
    x.deallocate(q, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

// Coallesce variety 2
TYPED_TEST(TestAllocator0, test_9) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    pointer r = x.allocate(s);
    x.deallocate(q, s);
    x.deallocate(r, s);
    x.deallocate(p, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

// Coallesce variety 3
TYPED_TEST(TestAllocator0, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type s = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    pointer r = x.allocate(s);
    x.deallocate(p, s);
    x.deallocate(r, s);
    x.deallocate(q, s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

//-------------
// Stress Test
//-------------
template <typename A> struct StressTestAllocator0 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, my_allocator<int, 960>>
    my_types_stress;

TYPED_TEST_CASE(StressTestAllocator0, my_types_stress);

// Allocate a bunch of values consecutively
TYPED_TEST(StressTestAllocator0, test_0) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  pointer ptr[80];

  for (int i = 0; i < 80; ++i) {
    ptr[i] = x.allocate(s);
  }
}

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
  ASSERT_EQ(x[0], -92);
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], -92);
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
