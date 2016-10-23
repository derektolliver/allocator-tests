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
#include <vector> // for tests

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

TEST(TestAllocator2, allocate_1) {
    my_allocator<int, 100> x;
    x.allocate(2);
    const my_allocator<int,100> y = x;
    ASSERT_EQ(-8,y[0]);}

TEST(TestAllocator2, allocate_2) {
    my_allocator<double, 100> x;
    x.allocate(10);
    const my_allocator<double,100> y = x;
    ASSERT_EQ(-92,y[0]);}

TEST(TestAllocator2,allocate_3) {
    my_allocator<int, 100> x;
    x.allocate(20);
    x.allocate(1);
    const my_allocator<int,100> y = x;
    ASSERT_EQ(-80, y[0]);
    ASSERT_EQ(-4, y[88]);}

TEST(TestAllocator2,deallocate_1) {
    my_allocator<int, 100> x;
    x.allocate(20);
    auto ptr = x.allocate(1);
    x.deallocate(ptr,1);
    const my_allocator<int,100> y = x;
    ASSERT_EQ(-80, y[0]);
    ASSERT_EQ(4, y[88]);}

TEST(TestAllocator2,deallocate_2) {
    my_allocator<int, 100> x;
    auto ptr = x.allocate(20);
    auto ptr2 = x.allocate(1);
    x.deallocate(ptr,20);
    x.deallocate(ptr2,1);
    const my_allocator<int,100> y = x;
    ASSERT_EQ(92,y[0]);}

TEST(TestAllocator2,deallocate_3) {
    my_allocator<int, 100> x;
    auto ptr = x.allocate(2);
    auto ptr2 = x.allocate(1);
    auto ptr3 = x.allocate(16);
    x.deallocate(ptr, 2);
    x.deallocate(ptr3, 16);
    x.deallocate(ptr2, 1);
    const my_allocator<int,100> y = x;
    ASSERT_EQ(92,y[0]);}

class A {
public:
    int i = 1;
    int j = 2;};

TEST(TestAllocator2, construct_1) {
    my_allocator<A,1000> x;
    auto ptr = x.allocate(10);
    A a;
    x.construct(ptr,a);
    ASSERT_EQ(1, (*ptr).i);
    ASSERT_EQ(2, (*ptr).j);
    x.destroy(ptr);
    x.deallocate(ptr, sizeof(A));}

TEST(TestAllocator2, construct_2) {
    my_allocator<A,1000> x;
    auto ptr = x.allocate(10);
    auto temp_ptr = ptr;
    auto e = ptr + 10;
    A a;

    while (temp_ptr != e) {
        x.construct(temp_ptr,a);
        ASSERT_EQ(1, (*temp_ptr).i);
        ASSERT_EQ(2, (*temp_ptr).j);
        ++temp_ptr;}
    temp_ptr = ptr;

    while (temp_ptr != e) {
        x.destroy(temp_ptr);
        ++temp_ptr;}
    x.deallocate(ptr, 10);}
TEST(TestAllocator2, construct_3) {
    // just barley making enough room
    my_allocator<A,sizeof(A) * 1000 + 8 * 1000> x;
    auto ptr = x.allocate(1000);
    auto temp_ptr = ptr;
    auto e = ptr + 10;
    A a;

    while (temp_ptr != e) {
        x.construct(temp_ptr,a);
        ASSERT_EQ(1, (*temp_ptr).i);
        ASSERT_EQ(2, (*temp_ptr).j);
        ++temp_ptr;}
    temp_ptr = ptr;

    while (temp_ptr != e) {
        x.destroy(temp_ptr);
        ++temp_ptr;}
    x.deallocate(ptr, 10);}
class B {
    char a[1000];};

TEST(TestAllocator2, destroy_1) {
    my_allocator<B,sizeof(B) + 8> x;
    B b;
    auto ptr = x.allocate(1);
    x.construct(ptr,b);
    x.destroy(ptr);}
class C {
    char a[100000];};

TEST(TestAllocator2, destroy_2) {
    my_allocator<C,sizeof(C) + 8> x;
    C c;
    auto ptr = x.allocate(1);
    x.construct(ptr,c);
    x.destroy(ptr);}

TEST(TestAllocator2, destroy_3) {
    my_allocator<std::vector<int>, 100000> x;
    std::vector <int> v(1000,0);
    auto ptr = x.allocate(1);
    x.construct(ptr, v);
    x.destroy(ptr);}

TEST(TestAllocator2, brackets_op_1) {
    my_allocator<A, 100> x;
    A a;
    auto ptr = x.allocate(1);
    x.construct(ptr,a);
    const my_allocator<A, 100> y = x;
    ASSERT_EQ(1, *((int*)&y[4]));}

TEST(TestAllocator2, brackets_op_2) {
    my_allocator<A, 100> x;
    A a;
    auto ptr = x.allocate(1);
    x.construct(ptr,a);
    const my_allocator<A, 100> y = x;
    ASSERT_EQ(2, *((int*)&y[8]));}

TEST(TestAllocator2, brackets_op_3) {
    my_allocator<A, 100> x;
    A a;
    auto ptr = x.allocate(1);
    x.construct(ptr,a);
    const my_allocator<A, 100> y = x;
    ASSERT_EQ(-1 * sizeof(A), *((int*)&y[0]));}



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
