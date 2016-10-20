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
#include <cassert>
#include <vector>
#include <numeric>

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
            Allocator<int,    100>,
            Allocator<double, 100>>
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
    const Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96],92);}

TEST(TestAllocator2, index) {
    Allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96],92);}

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
            Allocator<int,    200>,
            Allocator<double, 200>>
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

//My Unit Test
template <typename A>
struct MyTestAllocator_1 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            Allocator<int,    1200>
            //Allocator<double, 1200>
            >
        my_types_3;

TYPED_TEST_CASE(MyTestAllocator_1, my_types_3);

TYPED_TEST(MyTestAllocator_1, test_1)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type s = 1;
    const pointer ptr = x.allocate(s);
    x.deallocate(ptr,s);
}

TYPED_TEST(MyTestAllocator_1, test_2)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type all = 1192/sizeof(int);
    pointer p = x.allocate(all);
    x.deallocate(p,all);
}

TYPED_TEST(MyTestAllocator_1, test_3)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type s = 1;
    pointer p[100];
    for(int i = 0;i<100;++i)
    {
        p[i] = x.allocate(s);
    }
    for(int i = 99;i>=0;--i)
    {
        x.deallocate(p[i],s);
    }
    const size_type all = 1192/sizeof(int);
    pointer _p = x.allocate(all);
    x.deallocate(_p,all);

}

TYPED_TEST(MyTestAllocator_1, test_4)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type s = 1;
    pointer p[100];
    for(int i = 0;i<100;++i)
    {
        p[i] = x.allocate(s);
    }
    for(int i = 0;i<100;++i)
    {
        x.deallocate(p[i],s);
    }
    const size_type all = 1192/sizeof(int);
    pointer _p = x.allocate(all);
    x.deallocate(_p,all);
}

template <typename A>
struct MyTestAllocator_2 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            Allocator<double,    1600>
            >
        my_types_4;

TYPED_TEST_CASE(MyTestAllocator_2, my_types_4);

TYPED_TEST(MyTestAllocator_2, test_1)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type s = 1;
    const pointer ptr = x.allocate(s);
    x.deallocate(ptr,s);
}

TYPED_TEST(MyTestAllocator_2, test_2)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type all = 1592/sizeof(double);
    pointer p = x.allocate(all);
    x.deallocate(p,all);
}

TYPED_TEST(MyTestAllocator_2, test_3)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type s = 1;
    pointer p[100];
    for(int i = 0;i<100;++i)
    {
        p[i] = x.allocate(s);
    }
    for(int i = 99;i>=0;--i)
    {
        x.deallocate(p[i],s);
    }
    const size_type all = 1592/sizeof(double);
    pointer _p = x.allocate(all);
    x.deallocate(_p,all);

}

TYPED_TEST(MyTestAllocator_2, test_4)
{
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type x;
    const size_type s = 1;
    pointer p[100];
    for(int i = 0;i<100;++i)
    {
        p[i] = x.allocate(s);
    }
    for(int i = 0;i<100;++i)
    {
        x.deallocate(p[i],s);
    }
    const size_type all = 1192/sizeof(double);
    pointer _p = x.allocate(all);
    x.deallocate(_p,all);
}






































