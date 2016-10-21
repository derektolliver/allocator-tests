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
    //cout << "all destroyed."<<endl;
    x.deallocate(b, s);
    //cout << "Called deallocate" << endl;
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

/* User-defined tests */

template <typename A> struct TestAllocate : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>>
    my_types_3;

TYPED_TEST_CASE(TestAllocate, my_types_3);

/* invalid argumet, throws bad_alloc */
TYPED_TEST(TestAllocate, test1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 0;
  ASSERT_THROW(const pointer p = x.allocate(s), std::bad_alloc);
}

/* Checking if a valid block is left after allocation */
TYPED_TEST(TestAllocate, test2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const pointer p = x.allocate(s);
  ASSERT_TRUE(p != nullptr);
  if (p != nullptr) {
    ASSERT_EQ(*((char *)p + 8), 80);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocate, test3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 22;
  const pointer p = x.allocate(s);
  ASSERT_TRUE(p != nullptr);
  if (p != nullptr) {
    ASSERT_EQ(*((int*)((char *)p - 4)), -92);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

// deallocate
template <typename A> struct TestDeallocate : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

TYPED_TEST_CASE(TestDeallocate, my_types_3);

/* Deallocating invalid pointer */
TYPED_TEST(TestDeallocate, test1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.destroy(p);
    ASSERT_THROW(x.deallocate(p-1, s), std::invalid_argument);
  }
}

/* Deallocation creates consecutive free blocks */
TYPED_TEST(TestDeallocate, test2) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const pointer p1 = x.allocate(s);
  const pointer p2 = x.allocate(s);
  const pointer p3 = x.allocate(s);
  if (p1 != nullptr && p2 != nullptr && p3 != nullptr) {
    x.destroy(p1);
    x.deallocate(p1, s);
    x.destroy(p3);
    x.deallocate(p3, s);
    ASSERT_TRUE(*((int *)p2 + 2)!=0);
    x.destroy(p1);
    x.deallocate(p1, s);
  }
}
TYPED_TEST(TestDeallocate, test3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const pointer p1 = x.allocate(s);
  const pointer p2 = x.allocate(s);
  const pointer p3 = x.allocate(s);
  if (p1 != nullptr && p2 != nullptr && p3 != nullptr) {
    x.destroy(p2);
    x.deallocate(p2, s);
    x.destroy(p3);
    x.deallocate(p3, s);
    ASSERT_EQ(*((char *)p1 + 8), 80);
    x.destroy(p1);
    x.deallocate(p1, s);
  }
}

TYPED_TEST(TestDeallocate, test4) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.destroy(p);
    x.deallocate(p, s);
    ASSERT_THROW(x.deallocate(p + 10000, s), std::invalid_argument);
  }
}

TYPED_TEST(TestDeallocate, test6) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.destroy(p);
    ASSERT_THROW(x.deallocate(p, s + 20000), std::invalid_argument);
  }
}

// valid
TEST(TestValid, test1) {
  my_allocator<int, 100> x;
  ASSERT_TRUE(x.valid());
}

TEST(TestValid, test2) {
  my_allocator<int, 100> x;
  int* p = (int*)(x.allocate(1));
  x.deallocate(p, 1);
  ASSERT_TRUE(x.valid());
}

TEST(TestValid, test3) {
  my_allocator<int, 100> x;
  *((int*)x.a) = -4;
  *((int*)x.a + 2) = 4;
  ASSERT_FALSE(x.valid());
}

// my_allocator
template <typename A> struct TestConstructor : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>> my_types_4;

TYPED_TEST_CASE(TestConstructor, my_types_4);

TYPED_TEST(TestConstructor, test1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  my_allocator<int, 100>();

  allocator_type x;
  const size_type s = 0;
  const pointer p = x.allocate(s);

  if (p != nullptr) {
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestConstructor, test2) {
  ASSERT_THROW((my_allocator<int, 1>()), std::bad_alloc);
}
