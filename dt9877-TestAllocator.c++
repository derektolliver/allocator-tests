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
// Stress Test by Aaron Zou
// --------------

template <typename A> struct TestAllocatorStress1 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, my_allocator<int, 12000>>
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

  // Allocate the items to the pointers
  for (int i = 0; i < 1000; ++i) {
    ptr[i] = x.allocate(s);
  }

  // Deallocate the items in a random order
  std::vector<int> dealloc_order(1000);
  std::iota(begin(dealloc_order), end(dealloc_order), 0);
  std::random_shuffle(begin(dealloc_order), end(dealloc_order));
  for (auto it = begin(dealloc_order); it != end(dealloc_order); ++it) {
    x.deallocate(ptr[*it], s);
  }

  // Allocate one item the size of the entire free store - 8
  const size_type all = 11992 / sizeof(int);
  pointer p = x.allocate(all);
  x.deallocate(p, all);
}

// --------------
// MyTests
// --------------

template <typename A> struct TestAllocator0 : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_0;

TYPED_TEST_CASE(TestAllocator0, my_types_0);

TYPED_TEST(TestAllocator0, test_0) {
  // Check bad_alloc exception thrown in constructor
  try {
    my_allocator<double, 8> x;
    ASSERT_EQ(true, false);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(true, true);
  }
}

TEST(TestAllocator0, test_1) {
  // Allocator returns more memory than what was asked for 
    my_allocator<double, 100> x;
    double *p = x.allocate(4);
    p = x.allocate(4);
    p = x.allocate(1);
    ASSERT_EQ(x[0], -32);
    ASSERT_EQ(x[36], -32);
    ASSERT_EQ(x[40], -32);
    ASSERT_EQ(x[76], -32);
    ASSERT_EQ(x[80], -12);
    ASSERT_EQ(x[96], -12);
}

TEST(TestAllocator0, test_2) {
  // Allocator returns a block with exactly what was asked for
    my_allocator<double, 96> x;
    double *p = x.allocate(4);
    p = x.allocate(4);
    p = x.allocate(1);
    ASSERT_EQ(x[0], -32);
    ASSERT_EQ(x[36], -32);
    ASSERT_EQ(x[40], -32);
    ASSERT_EQ(x[76], -32);
    ASSERT_EQ(x[80], -8);
    ASSERT_EQ(x[92], -8);
}

TYPED_TEST(TestAllocator0, test_3) {
  // Asking for more than what the allocator has
    my_allocator<double, 96> x;
    double *p = x.allocate(4);
    p = x.allocate(4);
    p = x.allocate(5);
    ASSERT_EQ(p, nullptr);      
}

TYPED_TEST(TestAllocator0, test_4) {
  // Deallocating correctly, shouldn't throw any exeption
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p = x.allocate(4);
    p = x.allocate(4);
    try {
      x.deallocate(p, sizeof(double));
      ASSERT_EQ(true, true);
    }
    catch (std::bad_alloc &e){
      ASSERT_EQ(true, false);
    }
}

TYPED_TEST(TestAllocator0, test_5) {
  // Deallocating incorrect poitner, should throw invalid_argument exception
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p = x.allocate(4);
    p = x.allocate(4);
    try {
      x.deallocate(p + 4, sizeof(double));
      ASSERT_EQ(true, false);
    }
    catch (std::invalid_argument &e){
      ASSERT_EQ(true, true);
    }
}

TYPED_TEST(TestAllocator0, test_6) {
  // Coalescing with previous block
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p1 = x.allocate(4);
    pointer p2 = x.allocate(4);
    pointer p3 = x.allocate(1);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p2, sizeof(double));
    //make sure it still has enough space to allocate
    pointer p4 = x.allocate(8);
    ASSERT_NE(p4, nullptr);
}

TYPED_TEST(TestAllocator0, test_7) {
  // Coalescing with next block
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p1 = x.allocate(4);
    pointer p2 = x.allocate(4);
    pointer p3 = x.allocate(1);
    x.deallocate(p2, sizeof(double));
    x.deallocate(p1, sizeof(double));
    //make sure it still has enough space to allocate
    pointer p4 = x.allocate(8);
    ASSERT_NE(p4, nullptr);
}

TYPED_TEST(TestAllocator0, test_8) {
  // Coalescing with block next and previous block
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

    allocator_type x;
    pointer p1 = x.allocate(4);
    pointer p2 = x.allocate(4);
    pointer p3 = x.allocate(1);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p3, sizeof(double));
    x.deallocate(p2, sizeof(double));
    //make sure it still has enough space to allocate
    pointer p4 = x.allocate(11);
    ASSERT_NE(p4, nullptr);
}

TYPED_TEST(TestAllocator0, test_9) {
  // Allocate one block and make sure you can write to it
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  
  pointer p1 = x.allocate(s);

  *p1 = 5;  
}

TYPED_TEST(TestAllocator0, test_10) {
  // Allocate two blocks and make sure you can write to them
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 2;
  
  pointer p1 = x.allocate(s);
  pointer p2 = x.allocate(s);

  *p1 = 5;
  *p2 = 10;  
}

TYPED_TEST(TestAllocator0, test_11) {
  // allocate a block the size of the whole space and write to it

  my_allocator<int, 100> x;
  int * p = x.allocate(23);
  *p = 5;  
}


// --------------
// TestAllocator1
// --------------

template <typename A>
struct TestAllocator1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator1, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type  x;
    const size_type       s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 100 - (2 * sizeof(int)));
    ASSERT_EQ(x[96], 100 - (2 * sizeof(int)));
}


TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 100 - (2 * sizeof(int)));
    ASSERT_EQ(x[96], 100 - (2 * sizeof(int)));
}

// --------------
// TestAllocator3
// --------------

template <typename A>
struct TestAllocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    const pointer        p = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;
    const value_type     v = 2;
    const pointer        b = x.allocate(s);
    if (b != nullptr) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}


