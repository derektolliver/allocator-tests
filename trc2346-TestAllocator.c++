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

// ----------------
// Test constructor
// ----------------

TEST(TestAllocator2, const_index) {
  const my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, double_index) {
  const my_allocator<double, 200> x;
  ASSERT_EQ(x[0], 192);
  ASSERT_EQ(x[196], 192);
}

TEST(TestAllocator2, bad_allocation_1) {
  ASSERT_THROW((my_allocator<int, 2>()), std::bad_alloc);
}

/* Test whether having one less than the correct number of bytes throws error.*/
TEST(TestAllocator2, bad_allocation_2) {
  ASSERT_THROW((my_allocator<int, 11>()), std::bad_alloc);
}

/* Check that having exactly correct number of bytes works. */
TEST(TestAllocator2, exact_allocation) {
  my_allocator<int, 12> x;
  ASSERT_EQ(x[0], 4);
  ASSERT_EQ(x[8], 4);
}

// ---------------------
// Test Valid() function
// ---------------------

// Test invalid ending sentinel value
TEST(TestValid, test_1) {
  my_allocator<int, 100> x;
  x[96] = 10;
  ASSERT_FALSE(x.valid());
}

// Test sentinel size value that goes beyond the array.
TEST(TestValid, test_2) {
  my_allocator<int, 100> x;
  x[0] = 100;
  ASSERT_FALSE(x.valid());
}

// Test sentinel size value that would case the last byte
// of the end sentinel to go beyond the array.
TEST(TestValid, test_3) {
  my_allocator<int, 100> x;
  x[0] = 93;
  ASSERT_FALSE(x.valid());
}

// Test whether valid detects adjacent (but otherwise valid) freeblocks.
TEST(TestValid, test_4) {
  my_allocator<int, 28> x;
  x[0] = 8;  // 4 byte sentinel
             // 8 byte free space.
  x[12] = 8; // 4 byte sentinel
  x[16] = 4; // 4 byte sentinel
             // 4 byte free space.
  x[24] = 4; // 4 byte sentinel
             // 28 total bytes (size of allocator)
  ASSERT_FALSE(x.valid());
}

// ---------------
// Test allocate()
// ---------------

// Test that the first sentinel is a used block after first allocation.
TEST(TestAllocate, test_1) {
  my_allocator<int, 100> x;
  int *p = x.allocate(5);
  ASSERT_EQ(x[0], -20);
  x.deallocate(p, 5);
}

// Test that the first block of exactly the right size is chosen.
TEST(TestAllocate, test_2) {
  my_allocator<int, 100> x;
  int *p1 = x.allocate(2);
  int *p2 = x.allocate(3);
  int *p3 = x.allocate(2);
  x.deallocate(p2, 3);
  int *p4 = x.allocate(3);
  ASSERT_EQ(p2, p4);
  x.deallocate(p1, 2);
  x.deallocate(p3, 2);
  x.deallocate(p4, 3);
}

// Test that error is thrown if allocation is beyond size of memory..
TEST(TestAllocate, test_3) {
  my_allocator<int, 100> x;
  ASSERT_THROW(x.allocate(100), std::bad_alloc);
}

// Test that error is thrown when allocation is within size but there is not
// enough space.
TEST(TestAllocate, test_4) {
  my_allocator<int, 100> x;
  int *p1 = x.allocate(20);
  ASSERT_THROW(x.allocate(20), std::bad_alloc);
  x.deallocate(p1, 20);
}

// Test that the first block of any size that fits is used, when block >>
// request
TEST(TestAllocate, test_5) {
  my_allocator<int, 100> x;
  int *p1 = x.allocate(2);
  int *p2 = x.allocate(10);
  int *p3 = x.allocate(2);
  x.deallocate(p2, 3);
  int *p4 = x.allocate(3);
  ASSERT_EQ(p2, p4);
  x.deallocate(p1, 2);
  x.deallocate(p3, 2);
  x.deallocate(p4, 3);
}

// Test that the first block of any size that fits is used, when block is only
// slightly greater than request
TEST(TestAllocate, test_6) {
  my_allocator<int, 100> x;
  int *p1 = x.allocate(2);
  int *p2 = x.allocate(4);
  int *p3 = x.allocate(2);
  x.deallocate(p2, 3);
  int *p4 = x.allocate(3);
  ASSERT_EQ(p2, p4);
  x.deallocate(p1, 2);
  x.deallocate(p3, 2);
  x.deallocate(p4, 3);
}

// -----------------
// Test deallocate()
// -----------------

// Check that error is thrown when nullptr is passed.
TEST(TestDeallocate, test_1) {
  my_allocator<int, 100> x;
  ASSERT_THROW(x.deallocate(nullptr, 1), std::invalid_argument);
}

// Check that error is thrown when passed size is too big
TEST(TestDeallocate, test_2) {
  my_allocator<int, 100> x;
  int *p = x.allocate(2);
  ASSERT_THROW(x.deallocate(p, 5), std::invalid_argument);
  x.deallocate(p, 2);
}

// Check that deallocation occurs if size is less than actual block size.
TEST(TestDeallocate, test_3) {
  my_allocator<int, 100> x;
  int *p = x.allocate(10);
  ASSERT_NO_THROW(x.deallocate(p, 5));
}

// Check that error is thrown if beginning of array is passed in
TEST(TestDeallocate, test_4) {
  my_allocator<int, 100> x;
  int *p = x.allocate(2);
  --p;
  ASSERT_THROW(x.deallocate(p, 2), std::invalid_argument);
  ++p;
  x.deallocate(p, 2);
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
