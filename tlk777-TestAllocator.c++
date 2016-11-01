// ------------------------------------
// projects/allocator/Testmy_allocator1.c++
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
// Testmy_allocator1
// --------------

template <typename A> 
struct Testmy_allocator1 : testing::Test {
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
            my_allocator<double, 100> >
        my_types_1;

TYPED_TEST_CASE(Testmy_allocator1, my_types_1);

TYPED_TEST(Testmy_allocator1, test_1) {
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

TYPED_TEST(Testmy_allocator1, test_10) {
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
// Testmy_allocator2
// --------------

// each test individually tests allocate and deallocate
TEST(Testmy_allocator2, const_index) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    }

//This test accounts for a lot of things. Mainly the point of it is to test that deallocate can coallesce a previous block thats 
//free, the current block being freed, and the block after that's being freed. It also checks that allocate is working correctly
TEST(Testmy_allocator2, both) {
    my_allocator<int, 100> x;
    int* p = x.allocate(14);
    x.construct(p, 12);
    ASSERT_EQ(x[0], 0-(14*4));
    ASSERT_EQ(x[4], 12); 
    ASSERT_EQ(x[96], 92-14*4-8);
    int* b = x.allocate(2);
    x.construct(b, 3);
    ASSERT_EQ(x[60], 0-56);
    ASSERT_EQ(x[64], 0-8);
    ASSERT_EQ(x[68], 3);
    ASSERT_EQ(x[76], 0-8);
    x.destroy(p);
    x.deallocate(p, 14);
    ASSERT_EQ(x[0], 14*4);
    ASSERT_EQ(x[60], 14*4);
    x.destroy(b);
    x.deallocate(b, 2);
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
}

TEST(Testmy_allocator2, awkward){
    my_allocator<double, 100> x; 
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
    double* p = x.allocate(5);
    double* b = x.allocate(4);
    x.deallocate(p, 5);
    //these asserts aren't necessary, they are more to help me keep track of numbers.
    ASSERT_EQ(x[0], 40);
    ASSERT_EQ(x[44], 40);
    ASSERT_EQ(x[48], -32);
    ASSERT_EQ(x[84], -32);
    ASSERT_EQ(x[88], 4);
    ASSERT_EQ(x[96], 4);
    x.deallocate(b, 4);
    ASSERT_EQ(x[96], 40 + 4 + 32 +8 + 4 + 4);
    ASSERT_EQ(x[0], 92); 
} 

// --------------
// Testmy_allocator3
// --------------

template <typename A>
struct Testmy_allocator3 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    98>,
            my_allocator<double, 98> >
        my_types_2;

TYPED_TEST_CASE(Testmy_allocator3, my_types_2);

TYPED_TEST(Testmy_allocator3, test_1) {
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

TYPED_TEST(Testmy_allocator3, test_10) {
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


//using this to check for consistency between more types. Also a larger allocation size.
template <typename A>
struct Testmy_allocator4 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    1000>,
            my_allocator<double, 1000>,  
            my_allocator<short,  1000>,
            my_allocator<long,   1000> >
        my_types;

TYPED_TEST_CASE(Testmy_allocator4, my_types);

TYPED_TEST(Testmy_allocator4, test_100) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

        allocator_type  x;
    const size_type       s = 100;
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

