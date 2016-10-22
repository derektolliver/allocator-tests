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

/*
 * 
 * our tests begin here:
 *
 */

// --------------
// TestAllocator4
// --------------

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
            my_allocator<int,    40>,
            my_allocator<double, 20>>
        my_types_4;

TYPED_TEST_CASE(TestAllocator4, my_types_4);

TYPED_TEST(TestAllocator4, test_1) {
    //test if constructor checks size correctly
    try {
        my_allocator<int, 8> foo;
        ASSERT_EQ(true, false);}
    catch (bad_alloc &e) {
        ASSERT_EQ(true, true);}}

TYPED_TEST(TestAllocator4, test_2){
    //test constructor
    try {
        my_allocator<double, 20> foo;
        ASSERT_EQ(true, true);}
    catch (...) {
        ASSERT_EQ(true, false);}}

TYPED_TEST(TestAllocator4, test_3) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;
    
          allocator_type y;
    const size_type      s = 100;
    const value_type     v = 1;

    // check if allocate() correctly checks size
    try{
        pointer a = y.allocate(s);
        ASSERT_EQ(true, false);
    } catch (bad_alloc &e){
        ASSERT_EQ(true, true);}}

TYPED_TEST(TestAllocator4, test_4) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    allocator_type y;
    const size_type      s = 1;
    const value_type     v = 1;
    
    //Test allocate
    try{
        pointer b = y.allocate(s);
        ASSERT_EQ(true, true);
    } catch (...){
        ASSERT_EQ(true, false);}}

TYPED_TEST(TestAllocator4, test_5) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;

    // Test if deallocate catches null pointer
    allocator_type x;
    const size_type s = 10;
    const value_type v = 2;
    
    try {
      x.deallocate(nullptr, 10);
      ASSERT_EQ(true, false);
    } catch (std::invalid_argument &e) {
      ASSERT_EQ(true, true);
    }}

TYPED_TEST(TestAllocator4, test_6) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    // Test if deallocate catches out-of-range invalid pointer
    allocator_type x;
    const size_type s = 1;
    const value_type v = 2;
    pointer p = x.allocate(s);
    pointer b = p - 50;
    x.deallocate(p, s);
    try {
      x.deallocate(b, s);
      ASSERT_EQ(true, false);
    } catch (std::invalid_argument &e){
      ASSERT_EQ(true, true);}}

TYPED_TEST(TestAllocator4, test_7) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

    // Test if deallocate catches in-range invalid pointer
    allocator_type x;
    const size_type s = 1;
    const value_type v = 2;
    pointer p = x.allocate(s);
    pointer b = p - 1;
    x.deallocate(p, s);
    try {
      x.deallocate(b, s);
      ASSERT_EQ(true, false);
    } catch (std::invalid_argument &e){
      ASSERT_EQ(true, true);}}

// --------------
// TestAllocator5
// --------------

TEST(TestAllocator5, index1) {
    //test allocate
    my_allocator<double, 100> y;
    ASSERT_EQ(y[0], 92);
    ASSERT_EQ(y[96], 92);
    y.allocate(2);
    ASSERT_EQ(y[0], -16);
    ASSERT_EQ(y[20], -16);
    ASSERT_EQ(y[24], 68);
    ASSERT_EQ(y[96], 68);}

TEST(TestAllocator5, index2) {
    //test deallocate
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    int* p = x.allocate(10);
    x.deallocate(p, 10);
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator5, index3) {
    //test allocate when remaining is less than block size 
    my_allocator<double, 100> y;
    ASSERT_EQ(y[0], 92);
    ASSERT_EQ(y[96], 92);
    y.allocate(10);
    ASSERT_EQ(y[0], -92);
    ASSERT_EQ(y[96], -92);}

TEST(TestAllocator5, index4) {
    //test left coalescing in deallocate 
    my_allocator<double, 600> y;
    double* lhs = y.allocate(10);
    double* ml = y.allocate(15);
    double* m = y.allocate(10);
    double* mr = y.allocate(15);
    double* rhs = y.allocate(10);
    (void) lhs;
    (void) rhs;
    (void) mr;
    y.deallocate(ml, 15);
    ASSERT_EQ(y[88], 120);
    ASSERT_EQ(y[212], 120);
    y.deallocate(m, 10);
    ASSERT_EQ(y[88], 208);
    ASSERT_EQ(y[300], 208);}

TEST(TestAllocator5, index5) {
    //test right coalescing in deallocate 
    my_allocator<double, 600> y;
    double* lhs = y.allocate(10);
    double* ml = y.allocate(15);
    double* m = y.allocate(10);
    double* mr = y.allocate(15);
    double* rhs = y.allocate(10);
    (void) lhs;
    (void) rhs;
    (void) ml;
    y.deallocate(mr, 15);
    ASSERT_EQ(y[304], 120);
    ASSERT_EQ(y[428], 120);
    y.deallocate(m, 10);
    ASSERT_EQ(y[216], 208);
    ASSERT_EQ(y[428], 208);}

TEST(TestAllocator5, index6) {
    //test right and left coalescing in deallocate 
    my_allocator<double, 600> y;
    double* lhs = y.allocate(10);
    double* ml = y.allocate(15);
    double* m = y.allocate(10);
    double* mr = y.allocate(15);
    double* rhs = y.allocate(10);
    (void) lhs;
    (void) rhs;
    y.deallocate(ml, 15);
    y.deallocate(mr, 15);
    y.deallocate(m, 10);
    ASSERT_EQ(y[88], 336);
    ASSERT_EQ(y[428], 336);} 

TEST(TestAllocator5, index7) {
    //allocate and deallocate entire block 
    my_allocator<int, 100> y;
    int* p = y.allocate(23);
    ASSERT_EQ(y[0], -92);
    ASSERT_EQ(y[96], -92);
    y.deallocate(p, 23);
    ASSERT_EQ(y[0], 92);
    ASSERT_EQ(y[96], 92);}

// --------------
// TestAllocator6
// --------------

TEST(TestAllocator6, valid1) {
    //test valid out of bounds
    my_allocator<int, 100> y;
    y[0] = -12;
    try {
        y.valid();
        ASSERT_EQ(true, false);}
    catch (...){
        ASSERT_EQ(true, true);}}

TEST(TestAllocator6, valid2) {
    //test valid opposite signs
    my_allocator<int, 100> y;
    y[0] = -92;
    try {
        y.valid();
        ASSERT_EQ(true, false);}
    catch (...){
        ASSERT_EQ(true, true);}}

TEST(TestAllocator6, valid3) {
    //test valid sentinels don't match
    my_allocator<int, 100> y;
    y[0] = 56;
    try {
        y.valid();
        ASSERT_EQ(true, false);}
    catch (...){
        ASSERT_EQ(true, true);}}

TEST(TestAllocator6, valid4) {
    //test valid two free blocks in a row
    my_allocator<int, 100> y;
    y[0] = 12;
    y[16] = 12;
    y[20] = 72;
    y[96] = 72;
    try {
        assert(y.valid());
        ASSERT_EQ(true, false);}
    catch (...){
        ASSERT_EQ(true, true);}}

TEST(TestAllocator6, valid5) {
    //test valid- this is valid
    my_allocator<int, 100> y;
    y[0] = 12;
    y[16] = 12;
    y[20] = -4;
    y[28] = -4;
    y[32] = 60;
    y[96] = 60;
    try {
        y.valid();
        ASSERT_EQ(true, true);}
    catch (...){
        ASSERT_EQ(true, false);}}
