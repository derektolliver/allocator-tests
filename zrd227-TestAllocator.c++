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
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);
    ASSERT_EQ(x[96], 92);}

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
            my_allocator<char,    90>,
            my_allocator<int,     120>>
        my_types_3;

TYPED_TEST_CASE(TestAllocator4, my_types_3);

TYPED_TEST(TestAllocator4, test_1) {
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


TYPED_TEST(TestAllocator4, test_2) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;

          allocator_type x;
    const size_type      s = 10;
    bool exception_thrown = false;
    try {
        while (true) {
            x.allocate(s);}}
    catch (std::bad_alloc& e) {
        exception_thrown = true;}
    assert(exception_thrown);}

TYPED_TEST(TestAllocator4, test_3) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 1;
    const value_type     v = 2;
    
    std::vector<pointer> pointers(10);
    int index = 0;
    for(int i = 0; i<3; i++) {
        for(index = index; index<10; ++index) {
            pointer b = x.allocate(s);
            pointers[index] = b;
        }
        for(index = index; index<=0; --index) {
            x.deallocate(pointers[index], s);
        }
    }
}

TYPED_TEST(TestAllocator4, test_4) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 10;
    bool exception_thrown = false;
    try {
        pointer a = x.allocate(s);
        pointer b = x.allocate(s);
        x.deallocate(a,s);
        x.deallocate(b,s);
        x.deallocate(a,s);}
    catch (...) {
        exception_thrown = true;}
    assert(exception_thrown);}

// --------------
// TestAllocator5
// --------------

TEST(TestAllocator5, test_1) {
    bool exception_thrown = false;

    try {
        my_allocator<int, 11> x;
        assert(false);}
    catch (...) {
        exception_thrown = true;}
    assert(exception_thrown);
    exception_thrown = false;

    try {
        my_allocator<double, 15> x;
        assert(false);}
    catch (...) {
        exception_thrown = true;}
    assert(exception_thrown);
    exception_thrown = false;

    try {
        my_allocator<char, 8> x;
        assert(false);}
    catch (...) {
        exception_thrown = true;}
    assert(exception_thrown);
}

TEST(TestAllocator5, test_2) {
    my_allocator<int, 12> x;
    my_allocator<double, 16> y;
    my_allocator<char, 9> z;
}

TEST(TestAllocator5, valid_1) {
    my_allocator<int, 100> x;
    assert(x.valid());
}

TEST(TestAllocator5, valid_2) {
    my_allocator<int, 100> x;
    x[0] = -x[96];
    assert(!x.valid());
}

TEST(TestAllocator5, valid_3) {
    my_allocator<int, 100> x;
    x[0] = x[46] = x[50] = x[96] = 42;
    assert(!x.valid());
}

// --------------
// TestAllocator6
// --------------

template <typename A>
struct TestAllocator6 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;};

typedef testing::Types<
            my_allocator<int,    1921>,
            my_allocator<double, 3817>>
        my_types_4;

TYPED_TEST_CASE(TestAllocator6, my_types_4);

TYPED_TEST(TestAllocator6, test_1) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 158;
    const value_type     v = 2;
    bool exception_thrown = false;
    try {
        const pointer        left = x.allocate(s);
        const pointer        middle = x.allocate(s);
        const pointer        right = x.allocate(s);
        const pointer large = x.allocate(2*s);
    } catch (std::bad_alloc& e) {
        exception_thrown = true;
    }
    assert(exception_thrown);
}

TYPED_TEST(TestAllocator6, test_2) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 158;
    const value_type     v = 2;

    const pointer        left = x.allocate(s);
    const pointer        middle = x.allocate(s);
    const pointer        right = x.allocate(s);

    x.deallocate(left, s);
    x.deallocate(middle, s);

    const pointer large = x.allocate(2*s);
    x.deallocate(right, s);
    x.deallocate(large, s);
}

TYPED_TEST(TestAllocator6, test_3) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 158;
    const value_type     v = 2;

    const pointer        left = x.allocate(s);
    const pointer        middle = x.allocate(s);
    const pointer        right = x.allocate(s);

    x.deallocate(right, s);
    x.deallocate(middle, s);

    const pointer large = x.allocate(2*s);
    x.deallocate(left, s);
    x.deallocate(large, s);
}

TYPED_TEST(TestAllocator6, test_4) {
    typedef typename TestFixture::allocator_type allocator_type;
    typedef typename TestFixture::value_type     value_type;
    typedef typename TestFixture::size_type      size_type;
    typedef typename TestFixture::pointer        pointer;

          allocator_type x;
    const size_type      s = 158;
    const value_type     v = 2;

    const pointer        left = x.allocate(s);
    const pointer        middle = x.allocate(s);
    const pointer        right = x.allocate(s);

    x.deallocate(left, s);
    x.deallocate(middle, s);
    x.deallocate(right, s);

    const pointer large = x.allocate(3*s);
    x.deallocate(large, s);
}
