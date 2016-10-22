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
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

// my valid tests
TEST(TestAllocator4, valid1) {
    my_allocator<int, 100> x;
    x[0] = 10;
    ASSERT_FALSE(x.valid());
}

TEST(TestAllocator4, valid2) {
    my_allocator<double, 100> x;
    x[0] = 3;
    x[96] = 3;
    ASSERT_FALSE(x.valid());
}

TEST(TestAllocator4, valid3) {
    my_allocator<int, 100> x;
    x[0] = -8;
    x[16] = -8;
    x[20] = 68;
    x[96] = 68;
    ASSERT_FALSE(x.valid());
}

// my constructor tests
TEST(TestAllocator2, constructor1) {
    const my_allocator<double, 100> x;
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, constructor2) {
    const my_allocator<long, 100> x;
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, constructor3) {
    try {
        my_allocator<int, 1> x;
    } catch (std::bad_alloc e)  {
        ASSERT_TRUE(true);}
}

// my allocator tests
TEST(TestAllocator2, alloc1) {
    try {
        my_allocator<int, 8> x;
        x.allocate(1);
        ASSERT_TRUE(false);
    } catch (std::bad_alloc e) {
        ASSERT_TRUE(true);}
}

TEST(TestAllocator2, alloc2) {
    try {
        my_allocator<double, 17> x;
        x.allocate(1);
        ASSERT_TRUE(true);
    } catch (std::bad_alloc e) {
        ASSERT_TRUE(false);
    }
}

TEST(TestAllocator2, alloc3) {
    try {
        my_allocator<double, 44> x;
        x.allocate(2);
        ASSERT_TRUE(true);
    } catch (std::bad_alloc e) {
        ASSERT_TRUE(false);
    }
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

// my deallocator tests
TYPED_TEST(TestAllocator3, dealloc1) {
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
        // new test
        try{
            x.deallocate(b - 1, s);
            ASSERT_TRUE(false);
        } catch (...) {}
        x.deallocate(b, s);
        ASSERT_TRUE(true);
    }
}

TYPED_TEST(TestAllocator3, dealloc2) {
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
        // new test
        try{
            x.deallocate(b + 2, s);
            ASSERT_TRUE(false);
        } catch (...) {}
        x.deallocate(b, s);
        ASSERT_TRUE(true);
    }
}

TYPED_TEST(TestAllocator3, dealloc3) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type     value_type;
  typedef typename TestFixture::size_type      size_type;
  typedef typename TestFixture::pointer        pointer;

  // allocate two blocks, test coalesced
  allocator_type x;
  const size_type s = 3;
  const value_type v = 2;
  const pointer b1 = x.allocate(s);
  const pointer b2 = x.allocate(s);

if (b1 != nullptr) {
    pointer e = b1 + s;
    pointer p = b1;
    try {
        while (p != e) {
            x.construct(p, v);
            ++p;}}
    catch (...) {
        while (b1 != p) {
            --p;
            x.destroy(p);}
        x.deallocate(b1, s);
        throw;}
    ASSERT_EQ(s, std::count(b1, e, v));
    while (b1 != e) {
        --e;
        x.destroy(e);}}

if (b2 != nullptr) {
    pointer e = b2 + s;
    pointer p = b2;
    try {
        while (p != e) {
            x.construct(p, v);
            ++p;}}
    catch (...) {
        while (b2 != p) {
            --p;
            x.destroy(p);}
        x.deallocate(b2, s);
        throw;}
    ASSERT_EQ(s, std::count(b2, e, v));
    while (b2 != e) {
        --e;
        x.destroy(e);}

  x.deallocate(b1, s);
  x.deallocate(b2, s);

}
}
