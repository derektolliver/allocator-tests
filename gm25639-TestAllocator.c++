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
            std::allocator<char>,
            Allocator<int,    100>,
            Allocator<double, 100>,
            Allocator<char, 100>>
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

// Fix these for later

TEST(TestAllocator2, const_index) {
    const Allocator<char, 9> x;
    ASSERT_EQ(x[0], 1);
    ASSERT_EQ(x[5], 1);}

TEST(TestAllocator2, index) {
    Allocator<char, 9> x;
    ASSERT_EQ(x[0], 1);
    ASSERT_EQ(x[5], 1);}

TEST(TestAllocator2, const_index_1) {
    const Allocator<int, 12> x;
    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[8], 4);}

TEST(TestAllocator2, index_1) {
    Allocator<int, 12> x;
    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[8], 4);}

TEST(TestAllocator2, const_index_2) {
    const Allocator<double, 16> x;
    ASSERT_EQ(x[0], 8);
    ASSERT_EQ(x[12], 8);}

TEST(TestAllocator2, index_2) {
    Allocator<double, 16> x;
    ASSERT_EQ(x[0], 8);
    ASSERT_EQ(x[12], 8);}

TEST(TestAllocator2, constructor_exception_test) {
    try {
        Allocator<double, 15> x;
        ASSERT_TRUE(false);
    } catch(...) {
        ASSERT_TRUE(true);
    }
    
}

TEST(TestAllocator2, constructor_exception_test_1) {
    try {
        Allocator<int, 11> x;
        ASSERT_TRUE(false);
    } catch(...) {
        ASSERT_TRUE(true);
    }
    
}

TEST(TestAllocator2, constructor_exception_test_2) {
    try {
        Allocator<char, 8> x;
        ASSERT_TRUE(false);
    } catch(...) {
        ASSERT_TRUE(true);
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

// typedef testing::Types<
//             Allocator<int,    100>,
//             Allocator<double, 100>,
//             Allocator<char,    100>>
typedef testing::Types<
            Allocator<char,    1000>,
            Allocator<int,     1000>,
            Allocator<double,  2000>>
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
    const pointer        q = x.allocate(s);
    if (p != nullptr) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}
    if (q != nullptr) {
        x.construct(q, v);
        ASSERT_EQ(v, *q);
        x.destroy(q);
        x.deallocate(q, s);}}

TYPED_TEST(TestAllocator3, test_10) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;
    const value_type     v = 2;
    pointer        b = x.allocate(s);
    pointer        c = x.allocate(s);
    pointer        d = x.allocate(s);

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
            printf("THROW FAIL\n");
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}

    if (c != nullptr) {
        pointer e = c + s;
        pointer p = c;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (c != p) {
                --p;
                x.destroy(p);}
            x.deallocate(c, s);
            throw;}
        ASSERT_EQ(s, std::count(c, e, v));
        while (c != e) {
            --e;
            x.destroy(e);}
        x.deallocate(c, s);}

    if (d != nullptr) {
        pointer e = d + s;
        pointer p = d;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (d != p) {
                --p;
                x.destroy(p);}
            x.deallocate(d, s);
            throw;}
        ASSERT_EQ(s, std::count(d, e, v));
        while (d != e) {
            --e;
            x.destroy(e);}
        x.deallocate(d, s);}

    b = x.allocate(s);
    c = x.allocate(s);
    d = x.allocate(s);

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
            printf("THROW FAIL\n");
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}

    if (d != nullptr) {
        pointer e = d + s;
        pointer p = d;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (d != p) {
                --p;
                x.destroy(p);}
            x.deallocate(d, s);
            throw;}
        ASSERT_EQ(s, std::count(d, e, v));
        while (d != e) {
            --e;
            x.destroy(e);}
        x.deallocate(d, s);}

    if (c != nullptr) {
        pointer e = c + s;
        pointer p = c;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (c != p) {
                --p;
                x.destroy(p);}
            x.deallocate(c, s);
            throw;}
        ASSERT_EQ(s, std::count(c, e, v));
        while (c != e) {
            --e;
            x.destroy(e);}
        x.deallocate(c, s);}

    b = x.allocate(s);
    c = x.allocate(s);
    d = x.allocate(s);


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
            printf("THROW FAIL\n");
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}

    if (d != nullptr) {
        pointer e = d + s;
        pointer p = d;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (d != p) {
                --p;
                x.destroy(p);}
            x.deallocate(d, s);
            throw;}
        ASSERT_EQ(s, std::count(d, e, v));
        while (d != e) {
            --e;
            x.destroy(e);}
        x.deallocate(d, s);}

    if (c != nullptr) {
        pointer e = c + s;
        pointer p = c;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (c != p) {
                --p;
                x.destroy(p);}
            x.deallocate(c, s);
            throw;}
        ASSERT_EQ(s, std::count(c, e, v));
        while (c != e) {
            --e;
            x.destroy(e);}
        x.deallocate(c, s);}}

TYPED_TEST(TestAllocator3, test_50) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 50;
    const value_type     v = 2;
    pointer        b = x.allocate(s);
    pointer        c = x.allocate(s);
    pointer        d = x.allocate(s);

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
            printf("THROW FAIL\n");
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}

    if (c != nullptr) {
        pointer e = c + s;
        pointer p = c;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (c != p) {
                --p;
                x.destroy(p);}
            x.deallocate(c, s);
            throw;}
        ASSERT_EQ(s, std::count(c, e, v));
        while (c != e) {
            --e;
            x.destroy(e);}
        x.deallocate(c, s);}

    if (d != nullptr) {
        pointer e = d + s;
        pointer p = d;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (d != p) {
                --p;
                x.destroy(p);}
            x.deallocate(d, s);
            throw;}
        ASSERT_EQ(s, std::count(d, e, v));
        while (d != e) {
            --e;
            x.destroy(e);}
        x.deallocate(d, s);}

    b = x.allocate(s);
    c = x.allocate(s);
    d = x.allocate(s);

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
            printf("THROW FAIL\n");
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}

    if (d != nullptr) {
        pointer e = d + s;
        pointer p = d;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (d != p) {
                --p;
                x.destroy(p);}
            x.deallocate(d, s);
            throw;}
        ASSERT_EQ(s, std::count(d, e, v));
        while (d != e) {
            --e;
            x.destroy(e);}
        x.deallocate(d, s);}

    if (c != nullptr) {
        pointer e = c + s;
        pointer p = c;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (c != p) {
                --p;
                x.destroy(p);}
            x.deallocate(c, s);
            throw;}
        ASSERT_EQ(s, std::count(c, e, v));
        while (c != e) {
            --e;
            x.destroy(e);}
        x.deallocate(c, s);}

    b = x.allocate(s);
    c = x.allocate(s);
    d = x.allocate(s);


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
            printf("THROW FAIL\n");
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}

    if (d != nullptr) {
        pointer e = d + s;
        pointer p = d;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (d != p) {
                --p;
                x.destroy(p);}
            x.deallocate(d, s);
            throw;}
        ASSERT_EQ(s, std::count(d, e, v));
        while (d != e) {
            --e;
            x.destroy(e);}
        x.deallocate(d, s);}

    if (c != nullptr) {
        pointer e = c + s;
        pointer p = c;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (c != p) {
                --p;
                x.destroy(p);}
            x.deallocate(c, s);
            throw;}
        ASSERT_EQ(s, std::count(c, e, v));
        while (c != e) {
            --e;
            x.destroy(e);}
        x.deallocate(c, s);}}


TYPED_TEST(TestAllocator3, allocate_exception_test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 0;
    try {
        const pointer        p = x.allocate(s);
        ASSERT_TRUE(false);
    } catch(...) {
        ASSERT_TRUE(true);
    }}

TYPED_TEST(TestAllocator3, deallocate_exception_test_1) {
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
        try {
            x.deallocate(NULL, s);
            ASSERT_TRUE(false);
        } catch(...) {
            ASSERT_TRUE(true);
        }}}