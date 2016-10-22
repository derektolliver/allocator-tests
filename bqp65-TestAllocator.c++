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

template <typename A> struct TestAllocator1 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_1;

TYPED_TEST_CASE(TestAllocator1, my_types_1);

TYPED_TEST(TestAllocator1, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator1, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 2;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}

// --------------
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
  const my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

// --------------
// TestAllocator3
// --------------

template <typename A> struct TestAllocator3 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<my_allocator<int, 100>, my_allocator<double, 100>>
    my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 10;
  const value_type v = 30;
  const pointer p = x.allocate(s);
  if (p != nullptr) {
    x.construct(p, v);
    ASSERT_EQ(v, *p);
    x.destroy(p);
    x.deallocate(p, s);
  }
}

TYPED_TEST(TestAllocator3, test_10) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 9;
  const value_type v = 13;
  const pointer b = x.allocate(s);
  if (b != nullptr) {
    pointer e = b + s;
    pointer p = b;
    try {
      while (p != e) {
        x.construct(p, v);
        ++p;
      }
    } catch (...) {
      while (b != p) {
        --p;
        x.destroy(p);
      }
      x.deallocate(b, s);
      throw;
    }
    ASSERT_EQ(s, std::count(b, e, v));
    while (b != e) {
      --e;
      x.destroy(e);
    }
    x.deallocate(b, s);
  }
}

// --------------
// Constructor Tests
// --------------

TEST(TestAllocator5, bad_one) {
  try {
    my_allocator<int, 1> x;
    // should never reach
    ASSERT_EQ(1, 0);
  } catch (...) {
    ASSERT_EQ(0, 0);
  }
}

TEST(TestAllocator6, bad_two) {
  try {
    my_allocator<int, 4> x;
    // should never reach
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(0, 0);
  }
}

TEST(TestAllocator7, bad_three) {
  try {
    my_allocator<double, 1> x;
    // should never reach
    ASSERT_EQ(1, 0);
  } catch (...) {
    ASSERT_EQ(0, 0);
  }
}

TEST(TestAllocator8, bad_four) {
  try {
    my_allocator<double, 8> x;
    // should never reach
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(0, 0);
  }
}

TEST(TestAllocator9, single_ele_one) {
  my_allocator<int, 12> x;
  ASSERT_EQ(x[0], 4);
}

TEST(TestAllocator10, single_ele_two) {
  my_allocator<double, 16> x;
  ASSERT_EQ(x[0], 8);
}

// --------------
// valid() tests
// --------------

TEST(TestAllocator11, valid_one) {
  // two consecutive free blocks
  my_allocator<int, 100> x;
  x[0] = 30;
  x[34] = 30;
  x[38] = 20;
  x[62] = 20;
  x[66] = -26;
  x[96] = -26;
  ASSERT_FALSE(x.valid());
}

TEST(TestAllocator12, valid_two) {
  my_allocator<double, 100> x;
  // sentinel is 0
  x[0] = 30;
  x[34] = 30;
  x[38] = 0;
  x[42] = 0;
  x[46] = -46;
  x[96] = 46;
  ASSERT_FALSE(x.valid());
}

TEST(TestAllocator13, valid_three) {
  // sentinels don't match
  my_allocator<int, 100> x;
  x[0] = 50;
  x[54] = 30;
  x[58] = -34;
  x[96] = -34;
  ASSERT_FALSE(x.valid());
}

TEST(TestAllocator14, valid_four) {
  my_allocator<int, 100> x;
  x[0] = 10;
  x[14] = 10;
  x[18] = -10;
  x[32] = -10;
  x[36] = 10;
  x[50] = 10;
  x[54] = -10;
  x[68] = -10;
  x[72] = 20;
  x[96] = 20;
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator15, valid_five) {
  my_allocator<double, 100> x;
  x[0] = -16;
  x[20] = -16;
  x[24] = -24;
  x[52] = -24;
  x[56] = 36;
  x[96] = 36;
  ASSERT_TRUE(x.valid());
}

TEST(TestAllocator16, valid_six) {
  my_allocator<int, 100> x;
  x[0] = -36;
  x[40] = -36;
  x[44] = 20;
  x[68] = 20;
  x[72] = -20;
  x[96] = -20;
  ASSERT_TRUE(x.valid());
}

// --------------
// allocate() tests
// --------------

TEST(TestAllocator17, allocate_1) {
  // allocate 0 memory
  my_allocator<int, 100> x;
  int *test = x.allocate(0);
  ASSERT_EQ(test, nullptr);
}

TEST(TestAllocator18, allocate_2) {
  // allocate space more than size of allocator
  my_allocator<double, 100> x;
  try {
    x.allocate(13);
    // should never reach this assert
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator19, allocate_3) {
  // no free blocks left.
  my_allocator<int, 100> x;
  x.allocate(21);
  try {
    x.allocate(1);
    // should never reach this assert
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator20, allocate_4) {
  my_allocator<int, 100> x;
  x.allocate(4);
  x.allocate(17);
  try {
    x.allocate(1);
    // should never reach this assert
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator21, allocate_5) {
  my_allocator<double, 100> x;
  x.allocate(5);
  x.allocate(5);
  try {
    x.allocate(1);
    // should never reach this assert
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator22, allocate_6) {
  my_allocator<int, 100> x;
  x.allocate(10);
  x.allocate(10);
  try {
    x.allocate(1);
    // should never reach this assert
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(1, 1);
  }
}

// --------------
// deallocate() tests
// --------------

TEST(TestAllocator23, deallocate_1) {
  // deallocate nullptr
  my_allocator<int, 100> x;
  try {
    x.deallocate(nullptr, (std::size_t)1);
    // should never reach this assert
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator24, deallocate_2) {
  // deallocate free memory
  my_allocator<double, 100> x;
  try {
    x.deallocate(reinterpret_cast<double *>(&x[4]), (std::size_t)1);
    // should never reach this assert
    ASSERT_EQ(0, 1);
  } catch (...) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator25, deallocate_3) {
  // left merge
  my_allocator<int, 100> x;
  x[0] = 32;
  x[36] = 32;
  x[40] = -52;
  x[96] = -52;
  x.deallocate(reinterpret_cast<int *>(&x[44]), (std::size_t)1);
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator26, deallocate_4) {
  // right merge
  my_allocator<double, 100> x;
  x[0] = -64;
  x[68] = -64;
  x[72] = 20;
  x[96] = 20;
  x.deallocate(reinterpret_cast<double *>(&x[4]), (std::size_t)1);
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator27, deallocate_5) {
  // left and right merge
  my_allocator<int, 100> x;
  x[0] = 20;
  x[24] = 20;
  x[28] = -20;
  x[52] = -20;
  x[56] = 36;
  x[96] = 36;
  x.deallocate(reinterpret_cast<int *>(&x[32]), (std::size_t)1);
  ASSERT_EQ(x[0], 92);
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator28, deallocate_6) {
  // no merge
  my_allocator<double, 100> x;
  x[0] = -16;
  x[20] = -16;
  x[24] = -48;
  x[76] = -48;
  x[80] = -12;
  x[96] = -12;
  x.deallocate(reinterpret_cast<double *>(&x[28]), (std::size_t)1);
  ASSERT_EQ(x[24], 48);
  ASSERT_EQ(x[76], 48);
}
