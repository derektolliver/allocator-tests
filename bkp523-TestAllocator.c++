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

// [] tests
TEST(TestAllocator2, const_index) {
    const my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

TEST(TestAllocator2, index) {
    my_allocator<int, 100> x;
    ASSERT_EQ(x[0], 92);}

// allocate tests
TEST(TestAllocate, allocate1) {

  my_allocator<int, 100> x;
  x.allocate(1);
  
  ASSERT_EQ(x[0], -4); 
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], 80);
  ASSERT_EQ(x[96], 80);
}

TEST(TestAllocate, allocate2) {

  my_allocator<double, 100> x;
  x.allocate(2);
  
  ASSERT_EQ(x[0], -16); 
  ASSERT_EQ(x[20], -16);
  ASSERT_EQ(x[24], 68);
  ASSERT_EQ(x[96], 68);
}

TEST(TestAllocate, allocate3) {

  my_allocator<double, 50> x;
  x.allocate(1);
  
  ASSERT_EQ(x[0], -8); 
  ASSERT_EQ(x[12], -8);
  ASSERT_EQ(x[16], 26);
  ASSERT_EQ(x[46], 26);
}

// deallocate tests
TEST(TestDeallocate, deallocate1) {

  my_allocator<int, 50> x;
  
  int* p = x.allocate(1);
 
  x.deallocate(p, 1);  

  ASSERT_EQ(x[0], 42);
  ASSERT_EQ(x[46], 42); 
}

TEST(TestDeallocate, deallocate2) {

  my_allocator<int, 40> x;
  
  int* p = x.allocate(2);
  x.deallocate(p, 2);

  ASSERT_EQ(x[0], 32); 
  ASSERT_EQ(x[36], 32);
}

TEST(TestDeallocate, deallocate3) {

  my_allocator<int, 100> x;
  const size_t s = 1; 
  int* p1 = x.allocate(s);
  int* p2 = x.allocate(s);
  int* p3 = x.allocate(s);
  x.deallocate(p1, s);
  x.deallocate(p2, s);
  x.deallocate(p3, s);

  ASSERT_EQ(x[0], 92); 
  ASSERT_EQ(x[96], 92);
}

// valid tests
TEST(TestValid, valid1) {

  my_allocator<int, 200> x;
  x.allocate(1);
  ASSERT_TRUE(x.valid()); 
}
TEST(TestValid, valid2) {

  my_allocator<int, 100> x;
  int *p = x.allocate(3);
  x.deallocate(p, 3);
  ASSERT_TRUE(x.valid());
}
TEST(TestValid, valid3) {

  my_allocator<int, 100> x;
  int* p = x.allocate(1);
  x.deallocate(p,1);
  x.allocate(1);
  ASSERT_TRUE(x.valid()); 
}

// constructor tests
TEST(TestConstructor, constructor1) {

  bool e = false;
  try {
    
    my_allocator<int, 1000> x;  
  } 
  catch(std::bad_alloc& exception) {

      e = true;
  }
  ASSERT_FALSE(e);
}

TEST(TestConstructor, constructor2) {

  bool e = false;
  try {
    
    my_allocator<double, 2> x;  
  } 
  catch(std::bad_alloc& exception) {

      e = true;
  }
  ASSERT_TRUE(e);
}

TEST(TestConstructor, constructor3) {

  bool e = false;
  try {
    
    my_allocator<int, 12> x;  
  } 
  catch(std::bad_alloc& exception) {

      e = true;
  }
  ASSERT_FALSE(e);

}

// coalesce tests
TEST(TestCoalesce, coalesce1) {

  my_allocator<int, 50> x;
  
  int* p = x.allocate(4);
 
  x.deallocate(p, 4);  

  ASSERT_EQ(x[0], 42);
  ASSERT_EQ(x[46], 42); 
}

TEST(TestCoalesce, coalesce2) {

  my_allocator<int, 20> x;
  
  int* p = x.allocate(1);
 
  x.deallocate(p, 1);  

  ASSERT_EQ(x[0], 12);
  ASSERT_EQ(x[16], 12); 
}
TEST(TestCoalesce, coalesce3) {

  my_allocator<int, 1000> x;
  
  int* p = x.allocate(20);
 
  x.deallocate(p, 20);  

  ASSERT_EQ(x[0], 992);
  ASSERT_EQ(x[996], 992); 
}

// int_to_char_ptr tests
TEST(TestIntChar, intchar1) {

  int i = 1;
  int* p = &i;
  char* p2 = my_allocator<int, 100>().int_to_char_ptr(p);
  ASSERT_TRUE((char*)p == p2);
}

TEST(TestIntChar, intchar2) {

  int i = 1;
  int* p = &i;
  char* p2 = my_allocator<int, 200>().int_to_char_ptr(p);
  ASSERT_TRUE ((char*)p == p2);
}

TEST(TestIntChar, intchar3) {

  int i = 1;
  int* p = &i;
  char* p2 = my_allocator<int, 300>().int_to_char_ptr(p);
  ASSERT_TRUE((char*)p == p2);

}

// char_to_int_ptr tests

TEST(TestCharInt, charint1) {

  char i = 1;
  char* p = &i;
  int* p2 = my_allocator<int, 100>().char_to_int_ptr(p);
  ASSERT_TRUE(p == (char*)p2);
}

TEST(TestCharInt, charint2) {

  char i = 1;
  char* p = &i;
  int* p2 = my_allocator<int, 200>().char_to_int_ptr(p);
  ASSERT_TRUE(p == (char*)p2);
}

TEST(TestCharInt, charint3) {

  char i = 1;
  char* p = &i;
  int* p2 = my_allocator<int, 300>().char_to_int_ptr(p);
  ASSERT_TRUE(p == (char*)p2);

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
