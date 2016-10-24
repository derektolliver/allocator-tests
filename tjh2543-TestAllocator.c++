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
#include <exception> // exception
#include "gtest/gtest.h"
#include "cstddef"
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
    typedef typename A::pointer    pointer;
};

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

// -------------
// TestMy_Allocator
// --------------

TEST(TestMy_Allocator, bad_alloc1) {    // tests to see if allocate catches negative specified number of elements
    my_allocator<int, 100> x;
    ASSERT_THROW(x.allocate(-1), std::bad_alloc);
}

TEST(TestMy_Allocator, bad_alloc2) {    // tests to see if allocate catches number of element whose size is too large for heap 
    my_allocator<double, 20> x;
    ASSERT_THROW(x.allocate(2), std::bad_alloc);
}

TEST(TestMy_Allocator, bad_alloc3) {    // tests to see if allocate throws exception if too full too allocate new element
    my_allocator<int, 20> x;
    int* a = x.allocate(3);
    ASSERT_THROW(x.allocate(1), std::bad_alloc);
    x.deallocate(a, 3);
}

TEST(TestMy_Allocator, testInsert) {    // tests to be sure that allocate properly stores new values 
    my_allocator<int, 40> x;
    int* a = x.allocate(3);
    int* b = x.allocate(3);
    *a = 4;
    *b = 25;

    ASSERT_TRUE(*(a + 5) == 25);
    ASSERT_TRUE(*(b - 5) == 4);
    x.deallocate(a, 4);
    x.deallocate(b, 25);

}

// -------------
// TestCoalesce
// -------------

TEST(TestCoalesce, straddle_allocated_block) {
    my_allocator<double, 300> x;
    double* a = x.allocate(5);
    double* b = x.allocate(3);
    double* c = x.allocate(5);
    double* d = x.allocate(1);

    ASSERT_TRUE(*(((int*)a)-1) == -40 && *(((int*)b)-1) == -24 && *(((int*)c)-1) == -40 && *(((int*)d)-1) == -8);
    x.deallocate(a, 5);
    x.deallocate(c, 5);
    ASSERT_TRUE(*(((int*)a)-1) == 40 && *(((int*)b)-1) == -24 && *(((int*)c)-1) == 40 && *(((int*)d)-1) == -8);
    x.deallocate(b, 3);
    x.deallocate(d, 1);
}

TEST(TestCoalesce, coalesces_all_blocks) {
    my_allocator<double, 200> x;
    double* a = x.allocate(5);
    double* b = x.allocate(3);
    double* c = x.allocate(5);

    ASSERT_TRUE(*(((int*)a)-1) == -40 && *(((int*)b)-1) == -24 && *(((int*)c)-1) == -40);
    x.deallocate(a, 5);
    ASSERT_TRUE(*(((int*)a)-1) == 40);
    x.deallocate(b, 3);
    ASSERT_TRUE(*(((int*)a)-1) == 72);
    x.deallocate(c, 5);
    ASSERT_TRUE(*(((int*)a)-1) == 192);
}

    TEST(TestCoalesce, coalesces_middle_blocks_only) {
        my_allocator<int, 300> x;
        int* a = x.allocate(15);
        int* b = x.allocate(5);
        int* c = x.allocate(3);
        int* d = x.allocate(4);
        int* e = x.allocate(4);
        ASSERT_TRUE(*(a-1) == -60 && *(e-1) == -16);
        x.deallocate(b, 5);
        ASSERT_TRUE(*(b-1) == 20);
        x.deallocate(c, 3);
        ASSERT_TRUE(*(b-1) == 40);
        x.deallocate(d, 4);
        ASSERT_TRUE(*(b-1) == 64);

        ASSERT_TRUE(*(a-1) == -60 && *(e-1) == -16);
        x.deallocate(a, 15);
        x.deallocate(e, 4);
    }
//---------------
//TestDeallocate
//---------------

TEST(TestDeallocate, invalid_pointer) {
    my_allocator<int, 100> x;
    int* a = x.allocate(20);
    ASSERT_THROW(x.deallocate((a+1), 20), std::invalid_argument);
    x.deallocate(a, 20);
}

TEST(TestDeallocate, dealloc_succeeds1) {
    my_allocator<int, 100> x;
    int* a = x.allocate(10);
    int* b = x.allocate(5);
    int* c = x.allocate(3);
    ASSERT_TRUE((*(b-1)) == -20);
    x.deallocate(b, 5);
    ASSERT_TRUE((*(b-1)) == 20);
    x.deallocate(a, 10);
    x.deallocate(c, 3);
}

TEST(TestDeallocate, dealloc_succeeds2) {
    my_allocator<double, 200> x;
    double* a = x.allocate(4);
    double* b = x.allocate(5);
    double* c = x.allocate(3);
    double* d = x.allocate(1);
    ASSERT_TRUE(*(((int*)a) - 1) == -32);
    x.deallocate(a, 4); 
    ASSERT_TRUE(*(((int*)a)-1) == 32);
    ASSERT_TRUE(*(((int*)c)-1) == -24);
    x.deallocate(c, 3);
    ASSERT_TRUE(*(((int*)c)-1) == 24);
    x.deallocate(b, 5);
    x.deallocate(d, 1);
}