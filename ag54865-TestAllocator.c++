// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <cmath>
#include <memory> // allocator
#include <string>

#include "gtest/gtest.h"

#include "Allocator.h"

using namespace std;
// --------------
// TestAllocator1 [Original Tests by Professor Downing]
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

// <--- Test default constructor --->
TEST(TestAllocator2, const_index) {
  const my_allocator<int, 50> x;
  ASSERT_EQ(x[0], 42);
}

TEST(TestAllocator2, test_1) {
  const my_allocator<double, 46> x;
  ASSERT_EQ(x[0], 38);
}

TEST(TestAllocator2, test_2) {
  const my_allocator<char, 99> x;
  ASSERT_EQ(x[0], 91);
}

TEST(TestAllocator2, test_3) {
  const my_allocator<int, 33> x;
  ASSERT_EQ(x[0], 25);
}

// --------------
// TestAllocator3 [Original Tests by Professor Downing]
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

  ASSERT_EQ(1, 1);
}

// --------------
// TestAllocator4 [Rigorously Tests all of deallocate()'s coalescing cases]
// --------------

template <typename A> struct TestAllocator4 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>> my_types_3;

TYPED_TEST_CASE(TestAllocator4, my_types_3);

TYPED_TEST(TestAllocator4, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  // typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  string expectedResult = "| 92 | --- | 92 | \n";
  allocator_type x;
  const size_type s = 10;

  // Allocate
  const pointer a = x.allocate(2);
  const pointer b = x.allocate(2);
  const pointer c = x.allocate(2);
  const pointer d = x.allocate(2);
  const pointer e = x.allocate(2);
  // const pointer f = x.allocate(2);

  // Deallocate
  x.deallocate(c, s);
  x.deallocate(e, s);
  x.deallocate(d, s);
  x.deallocate(a, s);
  x.deallocate(b, s);
  ASSERT_EQ(x.printHeap(), expectedResult);
}

// --------------
// TestAllocator5 [Rigorously Tests all of deallocate()'s coalescing cases]
// -------------- 

template <typename A> struct TestAllocator5 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>> my_types_4;

TYPED_TEST_CASE(TestAllocator5, my_types_4);

TYPED_TEST(TestAllocator5, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  // typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  string expectedResult = "| 92 | --- | 92 | \n";
  allocator_type x;
  const size_type s = 10;

  // Allocate
  const pointer a = x.allocate(2);
  const pointer b = x.allocate(2);
  const pointer c = x.allocate(2);
  const pointer d = x.allocate(2);
  const pointer e = x.allocate(2);

  // Deallocate
  x.deallocate(a, s);
  x.deallocate(b, s);
  x.deallocate(e, s);
  x.deallocate(d, s);
  x.deallocate(c, s);

  ASSERT_EQ(x.printHeap(), expectedResult);
}

// <--- Test printHeap() --->
TEST(TestAllocator6, test_6) {
  my_allocator<int, 100> x;
  string expectedResult = "| 92 | --- | 92 | \n";

  ASSERT_EQ(x.printHeap(), expectedResult);
}

TEST(TestAllocator7, test_7) {
  my_allocator<double, 16> x;
  string expectedResult = "| 8 | --- | 8 | \n";

  ASSERT_EQ(x.printHeap(), expectedResult);
}

TEST(TestAllocator8, test_8) {
  my_allocator<double, 1000> x;
  string expectedResult = "| 992 | --- | 992 | \n";

  ASSERT_EQ(x.printHeap(), expectedResult);
}

// <--- Test valid() --->
TEST(TestAllocator9, test_9) {
  my_allocator<double, 1000> x;
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator10, test_10) {
  my_allocator<double, 1000> x;
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator11, test_11) {
  my_allocator<double, 1000> x;
  double *a = x.allocate(23);
  x.deallocate(a, sizeof(a));
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator12, test_12) {
  my_allocator<double, 1000> x;
  double *a = x.allocate(23);
  x.deallocate(a, sizeof(a));
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator13, test_13) {
  my_allocator<double, 1000> x;
  double *a = x.allocate(23);
  x.deallocate(a, sizeof(a));
  ASSERT_TRUE(x.valid());
}

// <--- Test allocate() --->
TEST(TestAllocator14, test_14) {
  my_allocator<double, 1000> x;
  x.allocate(10);
  string expectedResult = "| -80 | --- | -80 | 904 | --- | 904 | \n";
  ASSERT_EQ(x.printHeap(), expectedResult);
}

TEST(TestAllocator15, test_15) {
  my_allocator<int, 50> x;
  x.allocate(2);
  string expectedResult = "| -8 | --- | -8 | 26 | --- | 26 | \n";
  ASSERT_EQ(x.printHeap(), expectedResult);
}
TEST(TestAllocator16, test_16) {
  my_allocator<int, 250> x;
  x.allocate(4);
  string expectedResult = "| -16 | --- | -16 | 218 | --- | 218 | \n";
  ASSERT_EQ(x.printHeap(), expectedResult);
}

// <--- Test deallocate() --->
TEST(TestAllocator17, test_17) {
  my_allocator<double, 1000> x;
  double *a = x.allocate(10);
  x.deallocate(a, sizeof(a));
  string expectedResult = "| 992 | --- | 992 | \n";
  ASSERT_EQ(x.printHeap(), expectedResult);
}

TEST(TestAllocator18, test_18) {
  my_allocator<int, 50> x;
  int *a = x.allocate(2);
  x.allocate(1);
  x.deallocate(a, sizeof(a));
  string expectedResult = "| 8 | --- | 8 | -4 | --- | -4 | 14 | --- | 14 | \n";
  ASSERT_EQ(x.printHeap(), expectedResult);
}

TEST(TestAllocator19, test_19) {
  my_allocator<int, 250> x;
  x.allocate(4);
  int *b = x.allocate(10);
  x.deallocate(b, sizeof(b));
  string expectedResult = "| -16 | --- | -16 | 218 | --- | 218 | \n";
  ASSERT_EQ(x.printHeap(), expectedResult);
}
