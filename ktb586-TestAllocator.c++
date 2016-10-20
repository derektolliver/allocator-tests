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

using namespace std;

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
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, const_index_1) {
    const my_allocator<int, 3999> x;
    ASSERT_EQ(x[0], 3991);
    ASSERT_EQ(x[3995], 3991);
}

TEST(TestAllocator2, const_index_2) {
    const my_allocator<int, 24> x;
    ASSERT_EQ(x[0], 16);
    ASSERT_EQ(x[20], 16);
}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, index_1) {
    my_allocator<int, 2000> x;
    ASSERT_EQ(x[0], 1992);
    ASSERT_EQ(x[1996], 1992);
}

TEST(TestAllocator2, index_2) {
    my_allocator<int, 49> x;
    ASSERT_EQ(x[0], 41);
    ASSERT_EQ(x[45], 41);
}

TEST(TestAllocator2, taken) {
    my_allocator<int, 100> x;
    x.allocate(3);
    ASSERT_EQ(x[0], -12);
    ASSERT_FALSE(x[96] == 12);
}

TEST(TestAllocator2, bad_alloc) {
    try {
        const my_allocator<double, 2> x;
    } catch (bad_alloc e) {
        ASSERT_TRUE(true);
    }
}

TEST(TestAllocator2, valid) {
    // two consecutive free blocks
    my_allocator<int, 100> x;
    x[0] = 4;
    x[8] = 4;
    x[12] = 4;
    x[20] = 4;
    ASSERT_FALSE(x.valid());
}

TEST(TestAllocator2, allocate) {
    // found exactly enough space
    my_allocator<int, 100> x;
    x[0] = 4;
    x[8] = 4;
    x[12] = 80;
    x[96] = 80;
    x.allocate(1);
    ASSERT_EQ(x[12], 80);
    ASSERT_EQ(x[0], -4);
}

TEST(TestAllocator2, deallocate) {
    // found exactly enough space
    my_allocator<int, 100> x;
    x[0] = -4;
    x[8] = -4;
    x[12] = 4;
    x[20] = 4;
    x[24] = 68;
    x[96] = 68;
    int *p = x.allocate(1);
    ASSERT_EQ(x[12], -4);
    x.deallocate(p, 1);
    ASSERT_EQ(x[12], 80);
    ASSERT_EQ(x[12], x[96]);
    
    // coalesce blocks before and after
    p = x.allocate(1);
    x[0] = 4;
    x[8] = 4;
    x.deallocate(p, 1);
    ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, deallocate_2) {
    // pass in nullptr to deallocate, should throw invalid_argurment
    my_allocator<int, 100> x;
    int *n = nullptr;
    int *p = x.allocate(2);
    try {
        x.deallocate(n, 1);
    } catch (invalid_argument e) {
        x.deallocate(p, 2);
        ASSERT_EQ(x[0], 92);
    }
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

TYPED_TEST(TestAllocator3, test_2) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type value_type;
    typedef typename TestFixture::size_type size_type;
    typedef typename TestFixture::pointer pointer;

    allocator_type x;
    const size_type s = 8;
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

// -------------------
// Aaron's stress test
// -------------------
//  
//  template <typename A> struct TestAllocatorStress1 : testing::Test {
//      typedef A allocator_type;
//      typedef typename A::value_type value_type;
//      typedef typename A::size_type size_type;
//      typedef typename A::pointer pointer;
//  };
//  
//  typedef testing::Types<std::allocator<int>, my_allocator<int, 12000>>
//  my_types_stress1;
//  
//  TYPED_TEST_CASE(TestAllocatorStress1, my_types_stress1);
//  
//  // Allocate a bunch of items and deallocate them randomly 
//  TYPED_TEST(TestAllocatorStress1, test_0) {
//      typedef typename TestFixture::allocator_type allocator_type;
//      typedef typename TestFixture::size_type size_type;
//      typedef typename TestFixture::pointer pointer;
//  
//      allocator_type x;
//      const size_type s = 1;
//      pointer ptr[100];
//  
//      // Allocate the items to the pointers
//      for (int i = 0; i < 100; ++i) {
//          ptr[i] = x.allocate(s);
//      }
//  
//      // Deallocate the items in a random order
//      std::vector<int> dealloc_order(100);
//      std::iota(begin(dealloc_order), end(dealloc_order), 0);
//      std::random_shuffle(begin(dealloc_order), end(dealloc_order));
//      for (auto it = begin(dealloc_order); it != end(dealloc_order); ++it) {
//          x.deallocate(ptr[*it], s);
//      }
//  
//      // Allocate one item the size of the entire free store - 8
//      const size_type all = 11992 / sizeof(int);
//      pointer p = x.allocate(all);
//      x.deallocate(p, all);
//  }
