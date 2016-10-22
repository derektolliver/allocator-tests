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
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument
#include <numeric>   // itoa

#include "Allocator.h"

// -----------
// constructor
// -----------

// create an allocator with insufficent size
TEST(construct_test, construct_1) {
  try {
    // size < sizeof(double) + 2*sizeof(int)
    my_allocator<double, sizeof(double) + sizeof(int)> x;
  } catch (std::bad_alloc) {
    // bad_alloc should be thrown
    return;
  }
  ASSERT_TRUE(0);
}

// create an allocator with exactly the sufficent size
TEST(construct_test, construct_2) {
  try {
    my_allocator<double, sizeof(double) + 2 * sizeof(int)> x;
  } catch (std::bad_alloc) {
    // bad_alloc should not be thrown
    ASSERT_TRUE(0);
    return;
  }
}

// --------
// allocate
// --------

template <typename A> struct allocate_test : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<
    my_allocator<char, sizeof(char) * 100 + sizeof(int) * 200>,
    my_allocator<int, sizeof(int) * 100 + sizeof(int) * 200>,
    my_allocator<double, sizeof(double) * 100 + sizeof(int) * 200>>
    allocate_types;

TYPED_TEST_CASE(allocate_test, allocate_types);

// allocate to fill up the entire block with smallest units possible
TYPED_TEST(allocate_test, allocate_1) {
  typedef typename TestFixture::allocator_type allocator_type;

  allocator_type x;
  for (int i = 0; i < 100; i++)
    x.allocate(1);
}

// allocating blocks of varying sizes still fits within minimum size
TYPED_TEST(allocate_test, allocate_2) {
  typedef typename TestFixture::allocator_type allocator_type;

  allocator_type x;
  for (int i = 0; i < 13; i++)
    x.allocate(i);
}

// if there isn't room for another free block, allocate allocates the entire
// block
TEST(allocate_test, allocate_3) {
  my_allocator<double, sizeof(double) * 4 + sizeof(int) * 4 + 7> x;
  x.allocate(2);
  x.allocate(2);
  const my_allocator<double, sizeof(double) * 4 + sizeof(int) * 4 + 7> &y = x;
  ASSERT_NE(y[0], y[sizeof(double) * 2 + sizeof(int) * 2]);
}

// throw bad_alloc if you try to allocate more than N-2*sizeof(int)
TEST(allocate_test, allocate_4) {
  my_allocator<int, sizeof(int) * 4> x;
  try {
    // there is only room for 2 ints
    x.allocate(4);
  } catch (std::bad_alloc) {
    // bad_alloc should be thrown
    return;
  }
  ASSERT_TRUE(0);
}

// produce a bad_alloc if you don't return the pointer given by the free
TEST(allocate_test, allocate_5) {
  my_allocator<double, sizeof(double) * 5 + sizeof(int) * 4> x;
  x.allocate(2);
  try {
    // there is only room for 3 doubles
    x.allocate(4);
  } catch (std::bad_alloc) {
    // bad_alloc should be thrown
    return;
  }
  ASSERT_TRUE(0);
}

// ----------
// deallocate
// ----------

template <typename A> struct deallocate_test : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<char, 10000>, my_allocator<int, 10000>,
                       my_allocator<double, 10000>> deallocate_types;

TYPED_TEST_CASE(deallocate_test, deallocate_types);

// unite adjacent free blocks, left freed first
TYPED_TEST(deallocate_test, deallocate_1) {

  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;

  pointer left = x.allocate(100);
  pointer right = x.allocate(1);
  x.deallocate(left, 100);
  x.deallocate(right, 1);
  const allocator_type &y = x;
  ASSERT_EQ(y[0], 10000 - 2 * sizeof(int));
}

// unite adjacent free blocks, right freed first
TYPED_TEST(deallocate_test, deallocate_2) {

  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const allocator_type &y = x;
  pointer left = x.allocate(100);
  pointer right = x.allocate(1);
  x.deallocate(right, 1);
  x.deallocate(left, 100);
  ASSERT_EQ(y[0], 10000 - 2 * sizeof(int));
}

// unite a free block with block both to left and right
TYPED_TEST(deallocate_test, deallocate_3) {

  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const allocator_type &y = x;
  pointer left = x.allocate(100);
  pointer mid = x.allocate(70);
  pointer right = x.allocate(1);
  x.deallocate(right, 1);
  x.deallocate(left, 100);

  // check that blocks are still seperated
  ASSERT_EQ(y[0], sizeof(value_type) * 100);
  x.deallocate(mid, 70);

  // all 3 blocks can join now
  ASSERT_EQ(y[0], 10000 - 2 * sizeof(int));
}

// deallocating frees memory for more allocations
TYPED_TEST(deallocate_test, deallocate_4) {

  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  pointer p = x.allocate(5);
  // allocate until you run out of memory
  try {
    x.allocate(5);
  } catch (std::bad_alloc) {
  }
  // deallocate one block
  x.deallocate(p, 5);

  // attempt to allocate again
  try {
    x.allocate(5);
  } catch (std::bad_alloc) {
    ASSERT_TRUE(0);
  }
}

// deallocating a free block results in an invalid_arguement exception
TYPED_TEST(deallocate_test, deallocate_5) {

  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  pointer p = x.allocate(5);
  x.deallocate(p, 5);
  try {
    x.deallocate(p, 5);
  } catch (std::invalid_argument) {
    return;
  }
  ASSERT_TRUE(0);
}

// -----
// valid
// -----

// valid should fail if there are adjacent free blocks
TEST(valid_test, valid_1) {
  my_allocator<int, 100> x;
  x.set_header(0, 8, false);
  x.set_header(x.next(0), 8, false);
  ASSERT_FALSE(x.valid());
}

// valid should fail if headers are not equivalent
TEST(valid_test, valid_2) {
  my_allocator<int, 100> x;
  x.set_header(0, 8, false);
  x[0] = -8;
  ASSERT_FALSE(x.valid());
}

// valid should fail if the allocated blocks exceed N
TEST(valid_test, valid_3) {
  my_allocator<int, 100> x;
  x.set_header(0, 8, true);
  x.set_header(x.next(0), 100, false);
  ASSERT_FALSE(x.valid());
}

// ----
// next
// ----

// next should give the index of the block after the given block
TEST(next_test, next_1) {
  my_allocator<double, 1000> x;
  x.allocate(10);
  int next_header = x.next(0);
  ASSERT_EQ(next_header, 10 * sizeof(double) + 2 * sizeof(int));
}

// next should return N if there is no free block following the given one
TEST(next_test, next_2) {
  my_allocator<double, 1000> x;
  int next_header = x.next(0);
  ASSERT_EQ(next_header, 1000);
}

// next should ultimately return N if used to loop through all allocated blocks
TEST(next_test, next_3) {
  my_allocator<double, 1000> x;
  x.allocate(5);
  x.allocate(8);
  x.allocate(15);
  x.allocate(1);
  int i;
  for (i = 0; i < 1000; i = x.next(i))
    ;
  ASSERT_EQ(i, 1000);
}

// ----
// prev
// ----

// prev should return the index of the previous block
TEST(prev_test, prev_1) {
  my_allocator<double, 1000> x;
  x.allocate(1);
  x.allocate(10);
  x.allocate(7);
  int prev_header = x.prev(sizeof(double) * 11 + 4 * sizeof(int));
  ASSERT_EQ(prev_header, sizeof(double) + 2 * sizeof(int));
}

// prev should return 0 if there is not previous block
TEST(prev_test, prev_2) {
  my_allocator<double, 1000> x;
  int prev_header = x.prev(0);
  ASSERT_EQ(prev_header, 0);
}

// ----------
// set_header
// ----------

// begining and ending headers should be equal after a call to set_header
TEST(set_header_test, set_header_1) {
  my_allocator<double, 100> x;
  x.set_header(0, 20, true);
  ASSERT_EQ(x[0], x[20 + sizeof(int)]);
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
  ASSERT_EQ(x[0], 100 - 2 * sizeof(int));
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 100 - 2 * sizeof(int));
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
