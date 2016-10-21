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
    const my_allocator<int, 400> x;
    ASSERT_EQ(x[0], 98);
    ASSERT_EQ(x[396], 98);
    }

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 23);
    ASSERT_EQ(x[96], 23);
}

TEST(TestAllocator2, index1) {
    my_allocator<int, 100> x;
    int* p = x.allocate(12);
    ASSERT_EQ(x[0],  -12);
    ASSERT_EQ(x[52], -12);
    ASSERT_EQ(x[56], 9);
    ASSERT_EQ(x[96], 9);
}
TEST(TestAllocator2, index2) {
    my_allocator<double, 80> x;
    double* p = x.allocate(1);
    ASSERT_EQ(x[0],  -1);
    ASSERT_EQ(x[12], -1);
    ASSERT_EQ(x[16], 7);
    ASSERT_EQ(x[76], 7);
}
TEST(TestAllocator2, index3) {
    my_allocator<double, 80> x;
    double* p = x.allocate(8);
    ASSERT_EQ(x[0],  -9);
    ASSERT_EQ(x[76], -9);

    x.deallocate(p, sizeof(int));
    ASSERT_EQ(x[0], 9);
    ASSERT_EQ(x[76], 9);
}

//Bottom Deallocate
//Allocate
TEST(TestAllocator2, index4) {
    my_allocator<double, 80> x;
    double *p1 = x.allocate(1);
    ASSERT_EQ(x[0], -1);
    ASSERT_EQ(x[12], -1);
    double *p2 = x.allocate(7);
    ASSERT_EQ(x[0], -1);
    ASSERT_EQ(x[12], -1);
    ASSERT_EQ(x[16], -7); //first sentinel second block
    ASSERT_EQ(x[76], -7);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p2, sizeof(double));
    ASSERT_EQ(x[0], 9);
    ASSERT_EQ(x[76], 9);
}

//Top Deallocate
//Allocate
TEST(TestAllocator2, index5) {
    my_allocator<double, 80> x;
    double *p1 = x.allocate(1);
    ASSERT_EQ(x[0], -1);
    ASSERT_EQ(x[12], -1);
    double *p2 = x.allocate(7);
    ASSERT_EQ(x[0], -1);
    ASSERT_EQ(x[12], -1);
    ASSERT_EQ(x[16], -7); //first sentinel second block
    ASSERT_EQ(x[76], -7);
    x.deallocate(p2, sizeof(double));
    x.deallocate(p1, sizeof(double));
    ASSERT_EQ(x[0], 9);
    ASSERT_EQ(x[76], 9);
}

//Combination bottom/deallocate
//allocate
TEST(TestAllocator2, index6) {
    my_allocator<double, 72> x;
    double* p1 = x.allocate(1);
    double* p2 = x.allocate(2);
    double* p3 = x.allocate(3);
    x.deallocate(p2, sizeof(double));
    x.deallocate(p1, sizeof(double));
    x.deallocate(p3, sizeof(double));
    ASSERT_EQ(x[0], 8);
    ASSERT_EQ(x[68], 8);
 }

TEST(TestAllocator2, index7) {
    my_allocator<int, 100> x;
    int* p1 = x.allocate(4);
    int* p2 = x.allocate(4);
    int* p3 = x.allocate(11);
    x.deallocate(p1, sizeof(int));
    x.deallocate(p2, sizeof(int));
    x.deallocate(p3, sizeof(int));
    ASSERT_EQ(x[0], 23);
    ASSERT_EQ(x[96], 23);
}

TEST(TestAllocator2, index8) {
    my_allocator<int, 100> x;
    int* p1 = x.allocate(4);
    int* p2 = x.allocate(4);
    int* p3 = x.allocate(11);
    x.deallocate(p1, sizeof(int));
    x.deallocate(p3, sizeof(int));
    x.deallocate(p2, sizeof(int));
    ASSERT_EQ(x[0], 23);
    ASSERT_EQ(x[96], 23);
}

// TEST(TestAllocator2, index9) {
//     my_allocator<int, 100> x;
//     int* p1 = x.allocate(4);
//     int* p2 = x.allocate(4);
//     int* p3 = x.allocate(11);
//     x.deallocate(p3, sizeof(int));
//     x.deallocate(p2, sizeof(int));
//     x.deallocate(p1, sizeof(int));
//     ASSERT_EQ(x[0], 23);
//     ASSERT_EQ(x[96], 23);
// }


// // --------------
// // TestAllocator3
// // --------------

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
        
        
