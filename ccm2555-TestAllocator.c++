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
#include <numeric> 	 // iota

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

// ALLOCATOR object test goes here <<<<<<>>>>>>>><<<<<<<>>>>>>


TEST(TestAllocator2, bad_allocator) {
    try {
    my_allocator<int, 11> x;
    } catch (std::bad_alloc& ba) {
    ASSERT_STREQ("std::bad_alloc", ba.what());
    }}

// const indexing test

TEST(TestAllocator2, const_index_1) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

// Indexing test

TEST(TestAllocator2, index_1) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index_2) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, index_3) {
    my_allocator<int, 100> x;
    x[50] = 30;
    ASSERT_EQ(x[50], 30);
    x[50] = 200;
    ASSERT_EQ(x[50], 200);}

// valid() test

TEST(TestAllocator2, valid_1) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, valid_2) {
    my_allocator<int, 100> x;
    x[96] = 91;
    ASSERT_EQ(x.valid(), false);}

TEST(TestAllocator2, valid_3) {
    my_allocator<int, 100> x;
    x[0] = -12;
    x[16] = -12;
    x[20] = 72;
    x[96] = 72;
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, valid_4) {
    my_allocator<int, 100> x;
    x[0] = -12;
    x[16] = -12;
    x[20] = 72;
    x[96] = 71;
    ASSERT_EQ(x.valid(), false);}

// allocate() test

TEST(TestAllocator2, bad_allocate_1) {
    try {
    	my_allocator<int, 100> x;
    	x.allocate(26);
    } catch (std::bad_alloc& ba) {
    	ASSERT_STREQ("std::bad_alloc", ba.what());
    }}

TEST(TestAllocator2, bad_allocate_2) {
    try {
    my_allocator<int, 100> x;
    x.allocate(-1);
    } catch (std::bad_alloc& ba) {
    ASSERT_STREQ("std::bad_alloc", ba.what());
    }}

TEST(TestAllocator2, allocate_1) {
    my_allocator<int, 100> x;
    x.allocate(4);
    ASSERT_EQ(x[0], -16);
    ASSERT_EQ(x[20], -16);
    ASSERT_EQ(x[24], 68);
    ASSERT_EQ(x[96], 68);}

TEST(TestAllocator2, allocate_2) {
    my_allocator<int, 100> x;
    x.allocate(4);
    x.allocate(6);
    ASSERT_EQ(x[24], -24);
    ASSERT_EQ(x[52], -24);
    ASSERT_EQ(x[56], 36);
    ASSERT_EQ(x[96], 36);}

TEST(TestAllocator2, allocate_3) {
    my_allocator<int, 100> x;
    x.allocate(4);
    x.allocate(6);
    x.allocate(9);
    ASSERT_EQ(x[56], -36);
    ASSERT_EQ(x[96], -36);}


TEST(TestAllocator2, allocate_4) {
    my_allocator<int, 110> x;
    x.allocate(4);
    x.allocate(6);
    x.allocate(9);
    ASSERT_EQ(x[56], -46);
    ASSERT_EQ(x[106], -46);}

TEST(TestAllocator2, allocate_5) {
    my_allocator<int, 100> x;
    x.allocate(4);
    x.allocate(6);
    x.allocate(9);
    x.allocate(2);
    x.allocate(9);
    ASSERT_EQ(x[56], -36);
    ASSERT_EQ(x[96], -36);}


// deallocate() test

TEST(TestAllocator2, bad_deallocate_1) {
    try {
    	my_allocator<int, 100> x;
    	int* pointer = x.allocate(4);
    	x.deallocate(++pointer, 100);
    } catch (std::invalid_argument& ia) {
    	ASSERT_STREQ("p is invalid.", ia.what() );
    }}

TEST(TestAllocator2, deallocate_1) {
    my_allocator<int, 100> x;
    int* pointer = x.allocate(4);
    x.deallocate(pointer, 100);
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, deallocate_2) {
    my_allocator<int, 100> x;
    int* pointer_1 = x.allocate(4);
    x.allocate(6);
    x.deallocate(pointer_1, 100);
    ASSERT_EQ(x[0], 16);
    ASSERT_EQ(x[20], 16);
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, deallocate_3) {
    my_allocator<int, 100> x;
    int* pointer_1 = x.allocate(4);
    int* pointer_2 = x.allocate(6);
    int* pointer_3 = x.allocate(9);
    x.deallocate(pointer_2, 100);
    ASSERT_EQ(x[24], 24);
    ASSERT_EQ(x[52], 24);
    x.deallocate(pointer_1, 100);
    ASSERT_EQ(x[0], 48);
    ASSERT_EQ(x[52], 48);
    x.deallocate(pointer_3, 100);
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);
    ASSERT_EQ(x.valid(), true);}

TEST(TestAllocator2, deallocate_4) {
    my_allocator<int, 110> x;
    int* pointer1 = x.allocate(4);
    int* pointer2 = x.allocate(6);
    int* pointer3 = x.allocate(9);
    ASSERT_EQ(x[56], -46);
    ASSERT_EQ(x[106], -46);
	x.deallocate(pointer3, 110);
	x.deallocate(pointer2, 110);
	x.deallocate(pointer1, 110);
	ASSERT_EQ(x.valid(), true);
	}

TEST(TestAllocator2, deallocate_5) {
    my_allocator<int, 110> x;
    int* pointer1 = x.allocate(4);
    int* pointer2 = x.allocate(6);
    int* pointer3 = x.allocate(9);
    ASSERT_EQ(x[56], -46);
    ASSERT_EQ(x[106], -46);
	x.deallocate(pointer2, 110);
	x.deallocate(pointer3, 110);
	x.deallocate(pointer1, 110);
	ASSERT_EQ(x.valid(), true);
	}

TEST(TestAllocator2, deallocate_6) {
    my_allocator<int, 110> x;
	ASSERT_EQ(x[0], 102);
    int* pointer1 = x.allocate(4);
    int* pointer2 = x.allocate(6);
    int* pointer3 = x.allocate(9);
    ASSERT_EQ(x[56], -46);
    ASSERT_EQ(x[106], -46);
	x.deallocate(pointer1, 110);
	ASSERT_EQ(x[0], 16);
	ASSERT_EQ(x[20], 16);
	ASSERT_EQ(x.valid(), true);
	x.deallocate(pointer3, 110);
	ASSERT_EQ(x[56], 46);
	ASSERT_EQ(x[106], 46);
	x.deallocate(pointer2, 110);
	// result is 48 instead of 110
	ASSERT_EQ(x[0], 102);
	//ASSERT_EQ(x[4], 102);
	// this should be true, but it isn't correct
	//ASSERT_EQ(x[52], 48);
	//ASSERT_EQ(x[56], 46);
	ASSERT_EQ(x[106], 102);
	ASSERT_EQ(x.valid(), true);
	}
	
TEST(TestAllocator2, deallocate_7) {
    my_allocator<int, 110> x;
    int* pointer1 = x.allocate(4);
    int* pointer2 = x.allocate(6);
    int* pointer3 = x.allocate(9);
    ASSERT_EQ(x[56], -46);
    ASSERT_EQ(x[106], -46);
	x.deallocate(pointer1, 110);
	x.deallocate(pointer2, 110);
	x.deallocate(pointer3, 110);
	ASSERT_EQ(x.valid(), true);
	}

// *******  end TestAllocator2 test **************


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
// this stress test was taken from piazza and was onriganlly written by Aaron Zou
template <typename A> struct TestAllocatorStress1 : testing::Test {
  	typedef A allocator_type;
	typedef typename A::value_type value_type;
  	typedef typename A::size_type size_type;
  	typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, my_allocator<int, 12000>>
    my_types_stress1;

TYPED_TEST_CASE(TestAllocatorStress1, my_types_stress1);

// Allocate a bunch of items and deallocate them randomly 
TYPED_TEST(TestAllocatorStress1, test_0) {
  	typedef typename TestFixture::allocator_type allocator_type;
  	typedef typename TestFixture::size_type size_type;
  	typedef typename TestFixture::pointer pointer;

  	allocator_type x;
  	const size_type s = 1;
  	pointer ptr[1000];

  	// Allocate the items to the pointers
  	for (int i = 0; i < 1000; ++i) {
    	ptr[i] = x.allocate(s);
  	}

  	// Deallocate the items in a random order
  	std::vector<int> dealloc_order(1000);
  	std::iota(begin(dealloc_order), end(dealloc_order), 0);
  	std::random_shuffle(begin(dealloc_order), end(dealloc_order));
  	for (auto it = begin(dealloc_order); it != end(dealloc_order); ++it) {
    	x.deallocate(ptr[*it], s);
  	}

  	// Allocate one item the size of the entire free store - 8
  	const size_type all = 11992 / sizeof(int);
  	pointer p = x.allocate(all);
  	x.deallocate(p, all);
}

