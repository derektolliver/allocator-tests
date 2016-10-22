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
// TestAllocator - Doubles
// --------------
TEST(TestAllocatorDoubles, size_48_construction) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  ASSERT_EQ(((ro_viewer)x)[0], 40);
}

TEST(TestAllocatorDoubles, size_48_with_3_doubles) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  x.allocate(3);
  ASSERT_EQ(((ro_viewer)x)[0], -24);
  ASSERT_EQ(((ro_viewer)x)[28], -24);
}

TEST(TestAllocatorDoubles, size_48_with_4th_double) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  x.allocate(3);
  x.allocate(1);
  ASSERT_EQ(((ro_viewer)x)[0], -24);
  ASSERT_EQ(((ro_viewer)x)[28], -24);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);
}

TEST(TestAllocatorDoubles, size_48_with_5th_double_bad) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  x.allocate(3);
  x.allocate(1);
  ASSERT_EQ(((ro_viewer)x)[0], -24);
  ASSERT_EQ(((ro_viewer)x)[28], -24);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);

  try {
    x.allocate(1);
    ASSERT_EQ(false, true);
  } catch (bad_alloc &bad) {
    // yay
  }
}

TEST(TestAllocatorDoubles, size_48_4th_double_dealloc_left) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  double *p1 = x.allocate(3);
  x.allocate(1);
  ASSERT_EQ(((ro_viewer)x)[0], -24);
  ASSERT_EQ(((ro_viewer)x)[28], -24);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);

  x.deallocate(p1, 3);
  ASSERT_EQ(((ro_viewer)x)[0], 24);
  ASSERT_EQ(((ro_viewer)x)[28], 24);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);
}

TEST(TestAllocatorDoubles, size_48_4th_double_dealloc_right) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  x.allocate(3);
  double *p1 = x.allocate(1);
  ASSERT_EQ(((ro_viewer)x)[0], -24);
  ASSERT_EQ(((ro_viewer)x)[28], -24);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);

  x.deallocate(p1, 1);
  ASSERT_EQ(((ro_viewer)x)[0], -24);
  ASSERT_EQ(((ro_viewer)x)[28], -24);
  ASSERT_EQ(((ro_viewer)x)[32], 8);
  ASSERT_EQ(((ro_viewer)x)[44], 8);
}

TEST(TestAllocatorDoubles, size_48_double_dealloc_center_left) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  double *pleft = x.allocate(1);
  double *p1 = x.allocate(1);
  x.allocate(1);
  ASSERT_EQ(((ro_viewer)x)[0], -8);
  ASSERT_EQ(((ro_viewer)x)[12], -8);
  ASSERT_EQ(((ro_viewer)x)[16], -8);
  ASSERT_EQ(((ro_viewer)x)[28], -8);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);

  x.deallocate(pleft, 1);
  x.deallocate(p1, 1);

  ASSERT_EQ(((ro_viewer)x)[0], 24);
  ASSERT_EQ(((ro_viewer)x)[28], 24);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);
}

TEST(TestAllocatorDoubles, size_48_double_dealloc_center_right) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  x.allocate(1);
  double *p1 = x.allocate(1);
  double *pright = x.allocate(1);
  ASSERT_EQ(((ro_viewer)x)[0], -8);
  ASSERT_EQ(((ro_viewer)x)[12], -8);
  ASSERT_EQ(((ro_viewer)x)[16], -8);
  ASSERT_EQ(((ro_viewer)x)[28], -8);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);

  x.deallocate(pright, 1);
  x.deallocate(p1, 1);

  ASSERT_EQ(((ro_viewer)x)[0], -8);
  ASSERT_EQ(((ro_viewer)x)[12], -8);
  ASSERT_EQ(((ro_viewer)x)[16], 24);
  ASSERT_EQ(((ro_viewer)x)[44], 24);
}

TEST(TestAllocatorDoubles, size_48_double_dealloc_center_merge) {
  using rw_viewer = my_allocator<double, 48>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  double *pleft = x.allocate(1);
  double *p1 = x.allocate(1);
  double *pright = x.allocate(1);
  ASSERT_EQ(((ro_viewer)x)[0], -8);
  ASSERT_EQ(((ro_viewer)x)[12], -8);
  ASSERT_EQ(((ro_viewer)x)[16], -8);
  ASSERT_EQ(((ro_viewer)x)[28], -8);
  ASSERT_EQ(((ro_viewer)x)[32], -8);
  ASSERT_EQ(((ro_viewer)x)[44], -8);

  x.deallocate(pright, 1);
  x.deallocate(pleft, 1);
  x.deallocate(p1, 1);

  ASSERT_EQ(((ro_viewer)x)[0], 40);
  ASSERT_EQ(((ro_viewer)x)[44], 40);
}

TEST(TestAllocatorDoubles, size_40_construction) {
  using rw_viewer = my_allocator<double, 40>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  ASSERT_EQ(((ro_viewer)x)[0], 32);
  ASSERT_EQ(((ro_viewer)x)[36], 32);
}

TEST(TestAllocatorDoubles, size_32_3_doubles) {
  using rw_viewer = my_allocator<double, 40>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  x.allocate(3);
  ASSERT_EQ(((ro_viewer)x)[0], -32);
  ASSERT_EQ(((ro_viewer)x)[36], -32);
}

TEST(TestAllocatorDoubles, size_32_4_doubles_bad) {
  using rw_viewer = my_allocator<double, 40>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;

  x.allocate(3);
  ASSERT_EQ(((ro_viewer)x)[0], -32);
  ASSERT_EQ(((ro_viewer)x)[36], -32);

  try {
    x.allocate(1);
    ASSERT_EQ(true, false);
  } catch (bad_alloc &bc) {
    // yay
  }
}

TEST(TestAllocatorDoubles, size_24_construction) {
  using rw_viewer = my_allocator<double, 24>;
  using ro_viewer = const rw_viewer;
  rw_viewer x;
  ASSERT_EQ(((ro_viewer)x)[0], 16);
  ASSERT_EQ(((ro_viewer)x)[20], 16);
}

TEST(TestAllocatorDoubles, size_24_3_doubles_bad) {
  using rw_viewer = my_allocator<double, 24>;
  rw_viewer x;

  try {
    x.allocate(3);
    ASSERT_EQ(true, false);
  } catch (bad_alloc &bc) {
    // yay
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
                       my_allocator<int, 100>,
                       my_allocator<double, 100>> my_types_1;

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
