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

TEST(TestAllocator2, allocate1){
    my_allocator<char, 145> x;
    ASSERT_EQ(x[0], 137);
    ASSERT_EQ(x[141], 137);
}

TEST(TestAllocator2, allocate2){
    my_allocator<double, 64> x;
    ASSERT_EQ(x[0], 56);
    ASSERT_EQ(x[60], 56);
    double* p1 = x.allocate(3);
    ASSERT_EQ(x[0], -24);
    ASSERT_EQ(x[28], -24);
    ASSERT_EQ(x[32], 24);
    ASSERT_EQ(x[60], 24);
    double* p2 = x.allocate(3);
    ASSERT_EQ(x[0], -24);
    ASSERT_EQ(x[28], -24);
    ASSERT_EQ(x[32], -24);
    ASSERT_EQ(x[60], -24);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p2, sizeof(double));
}

TEST(TestAllocator2, allocate3){
    my_allocator<char, 64> x;
    ASSERT_EQ(x[0], 56);
    ASSERT_EQ(x[60], 56);
    char* p1 = x.allocate(3);
    ASSERT_EQ(x[0], -3);
    ASSERT_EQ(x[7], -3);
    ASSERT_EQ(x[11], 45);
    ASSERT_EQ(x[60], 45);
    char* p2 = x.allocate(8);
    ASSERT_EQ(x[0], -3);
    ASSERT_EQ(x[7], -3);
    ASSERT_EQ(x[11], -8);
    ASSERT_EQ(x[23], -8);
    ASSERT_EQ(x[27], 29);
    ASSERT_EQ(x[60], 29);
    x.deallocate(p2, sizeof(char));
    x.deallocate(p1, sizeof(char));
}

TEST(TestAllocator2, deallocate1){
    my_allocator<double, 64> x;
    double* p1 = x.allocate(3);
    double* p2 = x.allocate(3);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p2, sizeof(double));
    ASSERT_EQ(x[0], 56);
    ASSERT_EQ(x[60], 56);
}

TEST(TestAllocator2, deallocate2){
    my_allocator<double, 128> x;
    double* p1 = x.allocate(3);
    double* p2 = x.allocate(3);
    double* p3 = x.allocate(3);
    x.deallocate(p1, sizeof(double));
    x.deallocate(p2, sizeof(double));
    x.deallocate(p3, sizeof(double));
    ASSERT_EQ(x[0], 120);
    ASSERT_EQ(x[124], 120);
}

TEST(TestAllocator2, deallocate3){
    my_allocator<double, 128> x;
    double* p1 = x.allocate(3);
    double* p2 = x.allocate(3);
    double* p3 = x.allocate(3);
    x.deallocate(p2, sizeof(double));
    x.deallocate(p3, sizeof(double));
    x.deallocate(p1, sizeof(double));
    ASSERT_EQ(x[0], 120);
    ASSERT_EQ(x[124], 120);
}

TEST(TestAllocator2, deallocate4){
    my_allocator<char, 128> x;
    char* p1 = x.allocate(3);
    char* p2 = x.allocate(3);
    char* p3 = x.allocate(3);
    char* p4 = x.allocate(3);
    char* p5 = x.allocate(3);
    char* p6 = x.allocate(3);
    x.deallocate(p2, sizeof(char));
    x.deallocate(p4, sizeof(char));
    x.deallocate(p3, sizeof(char));
    x.deallocate(p6, sizeof(char));
    x.deallocate(p1, sizeof(char));
    x.deallocate(p5, sizeof(char));
    ASSERT_EQ(x[0], 120);
    ASSERT_EQ(x[124], 120);
}

TEST(TestAllocator2, exception1){
    try{
        my_allocator<double, 4> x;
        ASSERT_TRUE(false);
    }
    catch (std::bad_alloc& e){
        ASSERT_TRUE(true);
    }
}

TEST(TestAllocator2, exception2){
    try{
        my_allocator<int, 64> x;
        int* p1 = x.allocate(6);
        int* p2 = x.allocate(6);
        int* badPointer = p1-1;
        x.deallocate(p1, sizeof(int));
        x.deallocate(p2, sizeof(int));
        x.deallocate(badPointer, sizeof(int));
        ASSERT_TRUE(false);
    }
    catch (std::invalid_argument& e){
        ASSERT_TRUE(true);
    }
}

TEST(TestAllocator2, exception3){
    try{
        my_allocator<int, 64> x;
        int* p1 = x.allocate(6);
        int* p2 = x.allocate(6);
        int* badPointer = p1-2;
        x.deallocate(p1, sizeof(int));
        x.deallocate(p2, sizeof(int));
        x.deallocate(badPointer, sizeof(int));
        ASSERT_TRUE(false);
    }
    catch (std::invalid_argument& e){
        ASSERT_TRUE(true);
    }
}

TEST(TestAllocator2, exception4){
    try{
        my_allocator<double, 64> x;
        x.allocate(8);
        ASSERT_TRUE(false);
    }
    catch (std::bad_alloc& e){
        ASSERT_TRUE(true);
    }
}

TEST(TestAllocator2, exception5){
    try{
        my_allocator<double, 64> x;
        x.allocate(9);
        ASSERT_TRUE(false);
    }
    catch (std::bad_alloc& e){
        ASSERT_TRUE(true);
    }
}

// TEST(TestAllocator2, exception6){ //causes garbage value warning
//     my_allocator<int, 64> x;
//     int* p1 = x.allocate(6);
//     int* p2 = x.allocate(6);
//     try{
//         int* badPointer = p2-1;
//         x.deallocate(badPointer, sizeof(int));
//         ASSERT_TRUE(false);
//     }
//     catch (std::invalid_argument& e){
//         x.deallocate(p1, sizeof(int));
//         x.deallocate(p2, sizeof(int));
//         ASSERT_TRUE(true);
//     }
// }

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
