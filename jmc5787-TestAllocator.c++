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
        x.deallocate(b, s);}
    }

    TYPED_TEST(TestAllocator1, test_11) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s1 = 1;
    const value_type     v1 = 2;
    const pointer        p1 = x.allocate(s1);
    if (p1 != nullptr) {
        x.construct(p1, v1);
        ASSERT_EQ(v1, *p1);
        x.destroy(p1);
        x.deallocate(p1, s1);}

    const size_type      s2 = 4;
    const value_type     v2 = 6;
    const pointer        p2 = x.allocate(s2);
    if (p2 != nullptr) {
        x.construct(p2, v2);
        ASSERT_EQ(v2, *p2);
        x.destroy(p2);
        x.deallocate(p2, s2);}}

TYPED_TEST(TestAllocator1, test_100) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;
    const value_type     v = 25;
    const pointer        p1 = x.allocate(s);
    if (p1 != nullptr) {
        x.construct(p1, v);
        ASSERT_EQ(v, *p1);}

    const pointer        p2 = x.allocate(s);
    if (p2 != nullptr) {
        x.construct(p2, v);

        ASSERT_EQ(v, *p2);}

    if (p1 != nullptr) {
        x.destroy(p1);
        x.deallocate(p1, s);}

    if (p2 != nullptr) {
        x.destroy(p2);
        x.deallocate(p2, s);}}

    TYPED_TEST(TestAllocator1, test_101) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 49;
    const value_type     v = 56;
    const pointer        p1 = x.allocate(s);
    if (p1 != nullptr) {
        x.construct(p1, v);
        ASSERT_EQ(v, *p1);}

    const pointer        p2 = x.allocate(s);
    if (p2 != nullptr) {
        x.construct(p2, v);

        ASSERT_EQ(v, *p2);}

    if (p1 != nullptr) {
        x.destroy(p1);
        x.deallocate(p1, s);}

    if (p2 != nullptr) {
        x.destroy(p2);
        x.deallocate(p2, s);}}    


// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
    // std::cout << "hi\n";
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 12> x;
    ASSERT_EQ(x[0], 4);}

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

TYPED_TEST(TestAllocator3, test_11) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s1 = 13;
    const value_type     v1 = 27;
    const pointer        p1 = x.allocate(s1);
    if (p1 != nullptr) {
        x.construct(p1, v1);
        ASSERT_EQ(v1, *p1);
        x.destroy(p1);
        x.deallocate(p1, s1);}

    const size_type      s2 = 38;
    const value_type     v2 = 38;
    const pointer        p2 = x.allocate(s2);
    if (p2 != nullptr) {
        x.construct(p2, v2);
        ASSERT_EQ(v2, *p2);
        x.destroy(p2);
        x.deallocate(p2, s2);}}

TYPED_TEST(TestAllocator3, test_100) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s1 = 67;
    const value_type     v1 = 82;
    const pointer        b1 = x.allocate(s1);
    if (b1 != nullptr) {
        pointer e1 = b1 + s1;
        pointer p1 = b1;
        try {
            while (p1 != e1) {
                x.construct(p1, v1);
                ++p1;}}
        catch (...) {
            while (b1 != p1) {
                --p1;
                x.destroy(p1);}
            x.deallocate(b1, s1);
            throw;}
        ASSERT_EQ(s1, std::count(b1, e1, v1));
        while (b1 != e1) {
            --e1;
            x.destroy(e1);}
        x.deallocate(b1, s1);}

    const size_type      s2 = 10;
    const value_type     v2 = 2;
    const pointer        b2 = x.allocate(s2);
    if (b2 != nullptr) {
        pointer e2 = b2 + s2;
        pointer p2 = b2;
        try {
            while (p2 != e2) {
                x.construct(p2, v2);
                ++p2;}}
        catch (...) {
            while (b2 != p2) {
                --p2;
                x.destroy(p2);}
            x.deallocate(b2, s2);
            throw;}
        ASSERT_EQ(s2, std::count(b2, e2, v2));
        while (b2 != e2) {
            --e2;
            x.destroy(e2);}
        x.deallocate(b2, s2);}}

TYPED_TEST(TestAllocator3, test_101) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s1 = 7;
    const value_type     v1 = 3;
    const pointer        p1 = x.allocate(s1);
    if (p1 != nullptr) {
        x.construct(p1, v1);
        ASSERT_EQ(v1, *p1);
        x.destroy(p1);
        x.deallocate(p1, s1);}

    const size_type      s2 = 62;
    const value_type     v2 = 21;
    const pointer        p2 = x.allocate(s2);
    if (p2 != nullptr) {
        x.construct(p2, v2);
        ASSERT_EQ(v2, *p2);
        x.destroy(p2);
        x.deallocate(p2, s2);}}