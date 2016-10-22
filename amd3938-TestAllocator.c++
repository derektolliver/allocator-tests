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
        x.deallocate(p, s);
    }}

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

/*Alyjan's and Chris's tests*/

TEST(TestAllocator4, index) {
    my_allocator<int, 1000> x;
    ASSERT_EQ(x[0], 992);}

TEST(TestAllocator5, index) {
    try{
        my_allocator<int, 1> x;
    }
    // the constructor should throw a bad_alloc_exception
    catch(bad_alloc e){
        ASSERT_TRUE(true);
    }}

TEST(TestAllocator6, index) {
    my_allocator<double, 1000> x;
    ASSERT_EQ(x[0], 992);}

TEST(TestAllocator7, valid) {
    my_allocator<int, 100> x;
    x[0] = 20;
    ASSERT_FALSE(x.valid());}

TEST(TestAllocator8, valid) {
    my_allocator<int, 100> x;
    x[0] = 20;
    x[96] = 20;
    ASSERT_FALSE(x.valid());}

TEST(TestAllocator9, valid) {
    my_allocator<double,100> x;
    x[96] = -92;
    ASSERT_FALSE(x.valid());}

template <typename A>
struct TestAllocator11 : testing::Test {
    // --------
    // typedefs
    // --------
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    100>>
        our_types;

TYPED_TEST_CASE(TestAllocator11, our_types);

TYPED_TEST(TestAllocator11, allocate1) {
    typedef typename TestFixture::pointer        pointer;
    my_allocator<int, 100> x;
    try{
        pointer p = x.allocate(100);
    }
    // we should get an exception when we try to allocate more than n*sizeof(T)+(2*sizeof(int))
    catch(bad_alloc e){
        ASSERT_TRUE(true);
    }
}

TYPED_TEST(TestAllocator11, allocate2) {
    typedef typename TestFixture::pointer        pointer;
    my_allocator<int, 100> x;
    try{
        pointer p = x.allocate(-100);
    }
    // we should get an exception when we try to allocate negative stuff
    catch(bad_alloc e){
        ASSERT_TRUE(true);
    }
}

TYPED_TEST(TestAllocator11, allocate3) {
    typedef typename TestFixture::pointer        pointer;
    my_allocator<int, 100> x;
    try{
        pointer p = x.allocate(5);
        x.construct(p, 2);
        ASSERT_EQ(p[0], 2);
    }
    // we should never get an exception in this case
    catch(bad_alloc e){
        ASSERT_FALSE(true);
    }
}

template <typename A>
struct TestAllocator12 : testing::Test {
    // --------
    // typedefs
    // --------
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    100>>
        our_types_2;

TYPED_TEST_CASE(TestAllocator12, our_types_2);

TYPED_TEST(TestAllocator12, deallocate1) {
    typedef typename TestFixture::pointer        pointer;
    my_allocator<int, 100> x;
    try{
        pointer p = x.allocate(5);
        x.deallocate(p-10, 100);
    }
    // we should get an exception when we try to deallocate with a pointer that doesn't point to stuff we allocated
    catch(invalid_argument e){
        ASSERT_TRUE(true);
    }
}

TYPED_TEST(TestAllocator12, deallocate2) {
    typedef typename TestFixture::pointer        pointer;
    my_allocator<int, 100> x;
    try{
        pointer p = x.allocate(5);
        x.deallocate(0, 5);
    }
    // we should get an exception when we try to deallocate with a pointer that doesn't point to stuff we allocated
    catch(invalid_argument e){
        ASSERT_TRUE(true);
    }
}

TYPED_TEST(TestAllocator12, deallocate3) {
    typedef typename TestFixture::pointer        pointer;
    my_allocator<int, 100> x;
    try{
        pointer p = x.allocate(1);
        x.construct(p,10);
        x.deallocate(p, 1);
        ASSERT_EQ(p[0],10);
    }
    // we should never get an exception when we try to deallocate correctly
    catch(invalid_argument e){
        ASSERT_TRUE(false);
    }
}