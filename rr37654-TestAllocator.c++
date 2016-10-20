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
                       Allocator<int, 100>, Allocator<double, 100>>
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
  const Allocator<int, 100> x;
  ASSERT_EQ(x[0], 23);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
  ASSERT_EQ(x[0], 23);
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

// ---------------------
// TestAllocatorAllocate
// ---------------------

TEST(TestAllocatorAllocate, test1) {
  // Tests basic allocations that fill the allocator to the max
  Allocator<int, 38> y;

  int *a;
  int *b;
  int *c;

  a = y.allocate(1);
  c = y.allocate(1);
  b = y.allocate(1);

  assert(y.valid());

  y.deallocate(a, 1);
  y.deallocate(b, 1);
  y.deallocate(c, 1);
}

TEST(TestAllocatorAllocate, test2) {
  // Tests that types smaller than int work, filling allocator to the max, also
  // checking allocate return is correct spot within the heap
  Allocator<short, 24> y;

  short *a;
  short *b;

  a = y.allocate(2);
  b = y.allocate(1);

  assert(y.valid());

  *a = 1;
  *b = 14;
  *(a + 1) = 1;

  assert(reinterpret_cast<int *>(a) == &(y[4]));
  assert(reinterpret_cast<int *>(a + 1) == &(y[6]));
  assert(reinterpret_cast<int *>(b) == &(y[16]));

  y.deallocate(b, 1);
  y.deallocate(a, 2);

  assert(y.valid());
}

TEST(TestAllocatorAllocate, test3) {
  // this test checks that allocating blocks bigger than the allocator will fail
  // with the correct error code
  Allocator<int, 24> y;

  int *a = y.allocate(4);

  assert(y.valid());

  y.deallocate(a, 4);

  int fail = 0;
  try {
    a = y.allocate(5);
  } catch (std::bad_alloc &e) {
    fail = 1;
  }

  assert(fail == 1);
  assert(y.valid());

  a = y.allocate(4);

  assert(y.valid());

  y.deallocate(a, 4);
}

TEST(TestAllocatorAllocate, test4) {
  // this checks that i there is no space left in allocator, it will fail
  // properly
  Allocator<int, 24> y;

  int *a = y.allocate(4);

  assert(y.valid());

  int fail = 0;
  try {
    a = y.allocate(1);
  } catch (std::bad_alloc &e) {
    fail = 1;
  }

  assert(fail == 1);
  assert(y.valid());

  y.deallocate(a, 4);

  a = y.allocate(1);

  assert(y.valid());

  y.deallocate(a, 1);
}

// ------------------
// TestAllocatorValid
// ------------------

TEST(TestAllocatorValid, test1) {
  // checks if basic constructor is valid and that corrupting sentinels will
  // correctly invalidate the allocator
  Allocator<int, 12> y;

  assert(y.valid());

  y[0] = -y[0];

  assert(!y.valid());
}

TEST(TestAllocatorValid, test2) {
  // checks that manual correct modification of the sentinels behaves correctly
  Allocator<int, 12> y;

  assert(y.valid());

  y[0] = -y[0];
  y[8] = -y[8];
  y[4] = 3429;

  assert(y.valid());
}

TEST(TestAllocatorValid, test3) {
  // checks that each method keeps the allocator valid
  Allocator<int, 12> y;

  assert(y.valid());

  int *t = y.allocate(1);

  assert(y.valid());

  y.deallocate(t, 1);

  assert(y.valid());
}

TEST(TestAllocatorValid, test4) {
  // checks validity is maintained across multiple allocates and
  // deallocate/coalesces
  Allocator<int, 25> y;

  assert(y.valid());

  int *t = y.allocate(1);

  assert(y.valid());

  int *u = y.allocate(1);

  assert(y.valid());

  y.deallocate(t, 1);

  assert(y.valid());

  y.deallocate(u, 1);

  assert(y.valid());
}

// ------------------------
// TestAllocatorConstructor
// ------------------------

TEST(TestAllocatorConstructor, test1) {
  // checks that error is thrown correctly if allocator constructor input space
  // amount is not sufficient
  int fail = 0;

  try {
    Allocator<int, 11> y;
  } catch (std::bad_alloc &e) {
    fail = 1;
  }

  assert(fail == 1);

  Allocator<int, 12> y;

  assert(y.valid());
}

TEST(TestAllocatorConstructor, test2) {
  // check that initial sentinels are correct
  Allocator<int, 100> y;

  assert(y[0] == 23);

  Allocator<double, 200> z;

  assert(z[0] == 48);
}

// -----------------------
// TestAllocatorDeallocate
// -----------------------

TEST(TestAllocatorDeallocate, test1) {
  // checks basic deallocation modifies beginning and end sentinels correctly
  Allocator<int, 20> y;

  int *p = y.allocate(1);

  assert(y[0] == -3);
  assert(y[16] == -3);

  y.deallocate(p, 1);

  assert(y[0] == 3);
  assert(y[16] == 3);
}

TEST(TestAllocatorDeallocate, test2) {
  // checks coalescing by allocating successively and deallocating in the same
  // order
  Allocator<int, 100> y;

  int *p = y.allocate(1);
  int *r = y.allocate(1);
  int *s = y.allocate(1);

  y.deallocate(p, 1);
  assert(y[0] == 1);

  y.deallocate(r, 1);
  assert(y[0] == 4);

  y.deallocate(s, 1);
  assert(y[0] == 23);
}

TEST(TestAllocatorDeallocate, test3) {
  // checks coalescing by allocating successively and deallocating in reverse
  // order
  Allocator<int, 100> y;

  int *p = y.allocate(1);
  int *r = y.allocate(1);
  int *s = y.allocate(1);

  y.deallocate(s, 1);
  assert(y[24] == 17);

  y.deallocate(r, 1);
  assert(y[12] == 20);

  y.deallocate(p, 1);
  assert(y[0] == 23);
}

TEST(TestAllocatorDeallocate, test4) {
  // checks coalescing handles case of merging free blocks from left and right
  // side by deallocating middle block last
  Allocator<int, 100> y;

  int *p = y.allocate(1);
  int *r = y.allocate(1);
  int *s = y.allocate(1);

  y.deallocate(s, 1);
  assert(y[24] == 17);

  y.deallocate(p, 1);
  assert(y[0] == 1);

  y.deallocate(r, 1);
  assert(y[0] == 23);
}

TEST(TestAllocatorDeallocate, test5) {
  // check the invalid argument cases
  Allocator<int, 100> y;

  // nullptr input
  int fail = 0;
  try {
    y.deallocate(0, 1);
  } catch (std::invalid_argument &e) {
    fail = 1;
  }

  assert(fail == 1);

  fail = 0;

  // deallocate invalidly (in this case, double deallocate)
  int *p = y.allocate(1);
  y.deallocate(p, 1);

  try {
    y.deallocate(p, 1);
  } catch (std::invalid_argument &e) {
    fail = 1;
  }

  assert(fail == 1);

  // deallocate out of bounds
  p = y.allocate(1);

  try {
    y.deallocate(p + 28, 1);
  } catch (std::invalid_argument &e) {
    fail = 1;
  }

  assert(fail == 1);

  y.deallocate(p, 1);
}
