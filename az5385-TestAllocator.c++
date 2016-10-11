// ------------------------------------
// TestAllocator.c++
// Copyright (C) 2016
// Aaron Zou
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator
#include <vector>    // containers

#include "gtest/gtest.h"

#include "Allocator.h"

// --------------
// TestAllocator0
// --------------
/**
 * Test fixture class used to test pathological conditions of this assignment.\n
 * Prepare yourself.
 */
template <typename A> struct TestAllocator0 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<int, 100>, Allocator<double, 100>>
    my_types_0;

TYPED_TEST_CASE(TestAllocator0, my_types_0);

TYPED_TEST(TestAllocator0, test_0) {
  // Check bad_alloc exception thrown in constructor
  try {
    Allocator<double, 8> x;
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator0, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  // Check bad_alloc exception thrown in allocate()
  allocator_type x;
  const size_type s = 100;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator0, test_2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;

  // Check invalid_argument exception thrown in free()
  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  try {
    x.deallocate(nullptr, 10);
    ASSERT_EQ(true, false);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(true, true);
  }
}

TYPED_TEST(TestAllocator0, test_3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  // Ensure that memory is being split (can two requests be satisfied?)
  allocator_type x;
  const size_type s = 4;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator0, test_4) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  // Check coalescing to the right (easy to force)
  allocator_type x;
  const size_type s = 4;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    x.deallocate(q, s); // force a right-coalesce
    x.deallocate(p, s); // force a right-coalesce
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator0, test_5) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  // Check coalescing to the left (easy to force)
  allocator_type x;
  const size_type s = 4;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    x.deallocate(p, s);
    x.deallocate(q, s); // force a left-coalesce
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

TYPED_TEST(TestAllocator0, test_6) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  // Check coalescing to the left and right
  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  try {
    pointer p = x.allocate(s);
    pointer q = x.allocate(s);
    pointer r = x.allocate(s);
    x.deallocate(r, s);
    x.deallocate(p, s);
    x.deallocate(q, s); // force a left-coalesce and right-coalesce
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, false);
  }
}

// --------------------
// TestAllocatorStress0
// --------------------

template <typename A> struct TestAllocatorStress0 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, Allocator<int, 10000>>
    my_types_stress0;

TYPED_TEST_CASE(TestAllocatorStress0, my_types_stress0);

// Check allocate()/deallocate() in tandem (ring-around-the-rosie)
TYPED_TEST(TestAllocatorStress0, test_0) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 800;
  const value_type v = 2;
  pointer p = nullptr;
  pointer q = nullptr;
  pointer r = nullptr;

  // Always have two allocated at any given point in time
  for (int i = 0; i <= 50000; i++) {
    if (i % 3 == 0) {
      p = x.allocate(s);
      if (q != nullptr) {
        x.deallocate(q, s);
      }
    } else if (i % 3 == 1) {
      q = x.allocate(s);
      if (r != nullptr) {
        x.deallocate(r, s);
      }
    } else {
      r = x.allocate(s);
      if (p != nullptr) {
        x.deallocate(p, s);
      }
    }
  }

  // Deallocate the remaining two
  x.deallocate(q, s);
  x.deallocate(r, s);
}


// --------------------
// TestAllocatorStress1
// --------------------

template <typename A> struct TestAllocatorStress1 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, Allocator<int, 12000>>
    my_types_stress1;

TYPED_TEST_CASE(TestAllocatorStress1, my_types_stress1);

// Allocate a bunch of items and deallocate them randomly 
TYPED_TEST(TestAllocatorStress1, test_0) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  pointer ptr[1000];

  // Allocate the items in a random order
  std::vector<int> alloc_order(1000);
  std::iota(begin(alloc_order), end(alloc_order), 0);
  std::random_shuffle(begin(alloc_order), end(alloc_order));
  for (auto it = begin(alloc_order); it != end(alloc_order); ++it) {
    ptr[*it] = x.allocate(s);
  }

  // Deallocate the items in a random order
  std::vector<int> dealloc_order(1000);
  std::iota(begin(dealloc_order), end(dealloc_order), 0);
  std::random_shuffle(begin(dealloc_order), end(dealloc_order));
  for (auto it = begin(alloc_order); it != end(alloc_order); ++it) {
    x.deallocate(ptr[*it], s);
  }
}

// --------------
// TestAllocator1
// --------------

template <typename A> struct TestAllocator1 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       Allocator<int, 100>, Allocator<double, 100>>
    my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_0) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  const value_type v = 2;
  pointer p = x.allocate(s);
  x.deallocate(p, s);
  pointer q = x.allocate(s);
  x.deallocate(q, s);
}

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
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

// --------------
// TestAllocator3
// --------------

template <typename A> struct TestAllocator3 : testing::Test {
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
