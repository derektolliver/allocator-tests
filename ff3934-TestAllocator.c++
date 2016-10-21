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

// Testing my_allocator

TEST(TestAllocator2, const_index) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index2) {
    my_allocator<int, 121> x;
    ASSERT_EQ(x[0], 113);}

TEST(TestAllocator2, exception) {
    try{
        my_allocator<int, 10> x;}
    catch(bad_alloc e){
		  ASSERT_TRUE(true);}}

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

// -------------------------
// Testing allocate function
// -------------------------

TYPED_TEST(TestAllocator3, allocate) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;
	 		 allocator_type x;
	 const size_type 		s = 1000;
    try{
        const pointer p = x.allocate(s);}
    catch(bad_alloc e){
        ASSERT_TRUE(true);}}

TYPED_TEST(TestAllocator3, allocate2) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;
	 		 allocator_type x;
	 const size_type 		s = 0;
    try{
        const pointer p = x.allocate(s);}
    catch(bad_alloc e){
        ASSERT_TRUE(true);}}

TYPED_TEST(TestAllocator3, allocate3) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;
			 allocator_type x;
    const size_type      s = 4;
	 const size_type      s2 = 4;
	 const size_type 		 s3 = 1;
    const value_type     v = 2;
    const pointer p = x.allocate(s);
    const pointer p2 = x.allocate(s2);
    const pointer p3 = x.allocate(s3);
	 if (p == nullptr)
	 	ASSERT_TRUE(false);
	 if (p2 == nullptr)
	 	ASSERT_TRUE(false);
	 if (p3 == nullptr)
	 	ASSERT_TRUE(false);}

// -------------------------
// Testing deallocate method
// -------------------------

TYPED_TEST(TestAllocator3, deallocate) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;
			 allocator_type x;
    const size_type      s = 5;
    try{
        pointer p = x.allocate(s);
        x.deallocate(p - 5, s);}
    catch(invalid_argument e){
        ASSERT_TRUE(true);}}

TYPED_TEST(TestAllocator3, deallocate2) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;
			 allocator_type x;
    const size_type      s = 5;
    try{
        pointer p = x.allocate(s);
        x.deallocate(0, s);}
    catch(invalid_argument e){
        ASSERT_TRUE(true);}}

TYPED_TEST(TestAllocator3, deallocate3) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;
			 allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    try{
        const pointer p = x.allocate(s);
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.deallocate(p, s);}
    catch(invalid_argument e){
        ASSERT_TRUE(false);}}

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

// --------------
// TestAllocator4
// --------------

// Testing valid

TEST(TestAllocator4, valid) {
    my_allocator<int, 100> x;
    x[0] = 12;
	 x[16] = -12;
    ASSERT_FALSE(x.valid());}

TEST(TestAllocator4, valid2) {
    my_allocator<int, 100> x;
    x[0] = 20;
    x[24] = 20;
	 x[28] = 64;
    ASSERT_FALSE(x.valid());}

TEST(TestAllocator4, valid3) {
    my_allocator<double,100> x;
    x[96] = -12;
    ASSERT_FALSE(x.valid());}
