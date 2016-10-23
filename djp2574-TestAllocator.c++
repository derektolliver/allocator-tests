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


TEST(TestAllocator2, ALLOACTE_TEST_1) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);

    x.allocate(1);
    ASSERT_EQ(x[0], -4);
    ASSERT_EQ(x[8], -4);
    ASSERT_EQ(x[12], 80);
    ASSERT_EQ(x[96], 80);

    x.allocate(2);
    ASSERT_EQ(x[0], -4);
    ASSERT_EQ(x[8], -4);
    ASSERT_EQ(x[12], -8);
    ASSERT_EQ(x[24], -8);
    ASSERT_EQ(x[28], 64);
    ASSERT_EQ(x[96], 64);

    x.allocate(14);
    ASSERT_EQ(x[0], -4);
    ASSERT_EQ(x[8], -4);
    ASSERT_EQ(x[12], -8);
    ASSERT_EQ(x[24], -8);
    ASSERT_EQ(x[28], -64);
    ASSERT_EQ(x[96], -64);
}

TEST(TestAllocator2, ALLOACTE_TEST_2) {
    my_allocator<double, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);

    x.allocate(10);
    ASSERT_EQ(x[0], -92);
    ASSERT_EQ(x[96], -92);

    //////

    my_allocator<double, 100> y;
    ASSERT_EQ(y[0], 92);
    ASSERT_EQ(y[96], 92);

    y.allocate(9);
    ASSERT_EQ(y[0], -92);
    ASSERT_EQ(y[96], -92);

    //////

    my_allocator<double, 100> z;
    ASSERT_EQ(z[0], 92);
    ASSERT_EQ(z[96], 92);

    z.allocate(8);
    ASSERT_EQ(z[0], -64);
    ASSERT_EQ(z[68], -64);
    ASSERT_EQ(z[72], 20);
    ASSERT_EQ(z[96], 20);
}

TEST(TestAllocator2, ALLOACTE_TEST_3) {
	my_allocator<int, 100> x;
	bool error = false;
	try {
		x.allocate(23);
	} catch(std::bad_alloc& e){
		error = true;
	}
	ASSERT_FALSE(error);

    error = false;
    try {
        x.allocate(1);
    } catch(std::bad_alloc& e){
        error = true;
    }
    ASSERT_TRUE(error);


    my_allocator<int, 100> y;
    error = false;
    try {
        y.allocate(24);
    } catch(std::bad_alloc& e){
        error = true;
    }
    ASSERT_TRUE(error);
}

TEST(TestAllocator2, ALLOCATOR_TEST_1) {
    my_allocator<int, 20> x;
    ASSERT_EQ(x[0], 12);
    ASSERT_EQ(x[16], 12);
}

TEST(TestAllocator2, ALLOCATOR_TEST_2) {
    my_allocator<double, 40> x;
    ASSERT_EQ(x[0], 32);
    ASSERT_EQ(x[36], 32);
}

TEST(TestAllocator2, ALLOCATOR_TEST_3) {
	bool error = false;
	try {
        my_allocator<int, 8> x;
	} catch(std::bad_alloc& e){
		error = true;
	}
	ASSERT_TRUE(error);
}

TEST(TestAllocator2, DEALLOCATE_TEST_1) {
    my_allocator<int, 100> x;
    x.allocate(1);
    int* p = x.allocate(2);
    x.deallocate(p, 1);
    ASSERT_EQ(x[0], -4);
    ASSERT_EQ(x[8], -4);
    ASSERT_EQ(x[12], 80);
    ASSERT_EQ(x[96], 80);
}

TEST(TestAllocator2, DEALLOCATE_TEST_2) {
    my_allocator<int, 100> x;
    int* a = x.allocate(1);
    int* b = x.allocate(2);
    int* c = x.allocate(1);


    x.deallocate(c, 1);
    x.deallocate(a, 1);
    x.deallocate(b, 1);

    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, DEALLOCATE_TEST_3) {
    my_allocator<int, 100> x;
    int* a = x.allocate(1);

    bool error = false;
    try {
        x.deallocate(a+1000, 1);
    } catch(std::invalid_argument& e){
        error = true;
    }
    ASSERT_TRUE(error);

    error = false;
    try {
        x.deallocate(a-1000, 1);
    } catch(std::invalid_argument& e){
        error = true;
    }
    ASSERT_TRUE(error);
}

TEST(TestAllocator2, VALID_TEST_1) {
    my_allocator<int, 100> x;
    x[0] = -4;
    x[8] = -4;
    x[12] = -8;
    x[24] = -8;
    x[28] = 64;
    x[96] = 64;
    ASSERT_TRUE(x.valid());
}

TEST(TestAllocator2, VALID_TEST_2) {
    my_allocator<int, 100> x;
    x[0] = -4;
    x[8] = -4;
    x[12] = 8;
    x[24] = 8;
    x[28] = 64;
    x[96] = 64;
    ASSERT_FALSE(x.valid());
}

TEST(TestAllocator2, VALID_TEST_3) {
    my_allocator<int, 100> x;
    x[0] = -4;
    x[8] = -4;
    x[12] = -8;
    x[24] = 8;
    x[28] = 64;
    x[96] = 64;
    ASSERT_FALSE(x.valid());
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
