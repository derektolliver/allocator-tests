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

TYPED_TEST(TestAllocator1, test_2) {
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

TYPED_TEST(TestAllocator3, test_2) {
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

// -----------------------
// ADDED TESTS :)  TestAllocator4
// -----------------------

template <typename A>
struct TestAllocator4 : testing::Test {
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
        my_types_3;

TYPED_TEST_CASE(TestAllocator4, my_types_3);

TYPED_TEST(TestAllocator4, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type  bad_size = 30;
    const size_type  ok_size = 10;
    const value_type v = 4;
    bool  caught_exception = true;
    pointer a = nullptr;

    try {
        a = x.allocate(bad_size);
    } 
    catch (const std::bad_alloc& e){
        caught_exception = true;
	    a = x.allocate(ok_size);
    }

    if (a!=nullptr){
        pointer e = a + ok_size;
    	pointer p = a;
    	try{
    	    while (p!=e){
    	        x.construct(p, v);
    		++p;}}
    	catch (...){
    	    while(a!=p){
    	        --p;
    		x.destroy(p);}
    		x.deallocate(a, ok_size);
    		throw;}
        ASSERT_EQ(ok_size,std::count(a, e, v));
    	while(a!=e){
    	    --e;
    	    x.destroy(e);}
    	x.deallocate(a, ok_size);}
  
    ASSERT_EQ(true, caught_exception);}

TYPED_TEST(TestAllocator4, test_2) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 3;
    const pointer        b = x.allocate(s);
    const pointer        c = x.allocate(s);
    const pointer        d = x.allocate(s);

    bool caught = false;
    try {
        const pointer e = x.allocate(100);
    } catch (const std::bad_alloc& e) {
        caught = true;
    }

    if (b!=nullptr)
        x.deallocate(b, s);
    if (c!=nullptr)
        x.deallocate(c, s);
    if (d!=nullptr)
        x.deallocate(d,s);

    ASSERT_EQ(caught, true);
 }

TYPED_TEST(TestAllocator4, test_3) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 3;

    bool caught = false;
    try {
        const pointer e = x.allocate(0);
    } catch (const std::bad_alloc& e) {
        caught = true;
    }

    ASSERT_EQ(caught, true);
 }

/* Tests for constructors. */ 
TEST(TestAllocator5, index_1){
    bool caught = false;
    try{
        my_allocator<double, 13> x;
    } catch(const std::bad_alloc& e) {
        caught = true;
    }
    ASSERT_EQ(true, caught);
}

TEST(TestAllocator5, index_2){
    my_allocator<char, 99> x;
    ASSERT_EQ(x[0], 91);
    ASSERT_EQ(x[95], 91);
}

TEST(TestAllocator5, index_3){
    my_allocator<long long, 1024> x;
    ASSERT_EQ(x[0], 1016);
    ASSERT_EQ(x[1020], 1016);
}

// -----------------------
// ADDED TESTS :)  TestAllocator6
// -----------------------

template <typename A>
struct TestAllocator6 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<char,   1024>,
            my_allocator<double, 1024>>
        my_types_4;

TYPED_TEST_CASE(TestAllocator6, my_types_4);

TYPED_TEST(TestAllocator6, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type  s = 124;
    const pointer a = x.allocate(s);

    if (a!=nullptr)
        x.deallocate(a, s);

    const pointer b = x.allocate(s);
    if (b!=nullptr)
        x.deallocate(b, s);
    
    ASSERT_EQ(a, b);
}

TYPED_TEST(TestAllocator6, test_2) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type  s = 10;
    const pointer a = x.allocate(s);
    const pointer b = x.allocate(s);
    const pointer c = x.allocate(s);

    if (c!=nullptr)
        x.deallocate(c, s);

    const pointer d = x.allocate(s);
    ASSERT_EQ(c, d);

    if (d!=nullptr)
        x.deallocate(d, s);

    if (a!=nullptr)
        x.deallocate(a, s);

    if (b!=nullptr)
        x.deallocate(b, s);
}

TYPED_TEST(TestAllocator6, test_3) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type  s = 10;

    const pointer a = x.allocate(s);
    const pointer b = x.allocate(s);

    bool caught = false;
    try{
        const pointer c = x.allocate(s*100);
    } catch (const std::bad_alloc& e){
        caught = true;
    }

    if(a!=nullptr)
        x.deallocate(a, s);

    const pointer d = x.allocate(s);
    if(d!=nullptr)
        x.deallocate(d, s);
    ASSERT_EQ(caught, true);
    ASSERT_EQ(a, d);
}

TYPED_TEST(TestAllocator6, test_4) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type  s = 10;
    const pointer a = x.allocate(s);
    bool caught = false;

    try{
        x.deallocate(a-1, s);
    } catch (const std::invalid_argument& e){
        caught = true;
    }

    if(a!=nullptr)
        x.deallocate(a, s);

    ASSERT_EQ(caught, true);
}

TYPED_TEST(TestAllocator6, test_5) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type  s = 10;
    const pointer a = x.allocate(s);
    bool caught = false;

    try{
        x.deallocate(a+s*1000, s);
    } catch (const std::invalid_argument& e){
        caught = true;
    }

    if( caught && a!=nullptr)
        x.deallocate(a, s);

    ASSERT_EQ(caught, true);
}

// --------------
// TestAllocator7
// --------------

TEST(TestAllocator7, valid1) {
    my_allocator<int, 100> x;
    x[0] = 0;
    ASSERT_EQ(x.valid(), false);}

TEST(TestAllocator7, valid2) {
    my_allocator<int, 100> x;
    x[96] = 0;
    ASSERT_EQ(x.valid(), false);}

TEST(TestAllocator7, valid3) {
    my_allocator<int, 100> x;
    x[0] = -16;
    x[20] = -16;
    x[24] = 68;
    x[96] = 68;
    ASSERT_EQ(x.valid(), true);}
    