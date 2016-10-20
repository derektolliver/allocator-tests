// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <cassert>   // ASSERT
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// --------
// My Test
// --------

template <typename A> struct StudentTestAllocator0 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<char, 100>, Allocator<int, 100>,
                       Allocator<double, 100>, Allocator<short, 100>>
    student_types_0;

TYPED_TEST_CASE(StudentTestAllocator0, student_types_0);

TYPED_TEST(StudentTestAllocator0, test_1) {
  // test bad_alloc exception by passing over-size s
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 101;
  const value_type v = 20;
  pointer p;
  try {
    p = x.allocate(s);
    ASSERT_TRUE(false);
  } catch (const std::bad_alloc &e) {
    ASSERT_TRUE(true);
  }
}

TYPED_TEST(StudentTestAllocator0, test_2) {
  // test invalid_argument exception passing nullptr
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 101;
  const value_type v = 20;
  pointer p = nullptr;
  try {
    x.deallocate(p, v);
    ASSERT_TRUE(false);
  } catch (const std::invalid_argument &e) {
    ASSERT_TRUE(true);
  }
}

TYPED_TEST(StudentTestAllocator0, test_3) {
  // test invalid_argument exception combining test_1&2
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 101;
  const value_type v = 20;
  pointer p = nullptr;
  try {
    p = x.allocate(s);
    ASSERT_TRUE(false);
  } catch (const std::bad_alloc &e) {
    ASSERT_TRUE(true);
    try {
      x.deallocate(p, s);
      ASSERT_TRUE(false);
    } catch (const std::invalid_argument &e) {
      ASSERT_TRUE(true);
    }
  }
}

TYPED_TEST(StudentTestAllocator0, test_4) {
  // test right coalesce
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p1 = x.allocate(s);
    pointer p2 = x.allocate(s);
    x.deallocate(p2, s);
    x.deallocate(p1, s);
  } catch (const std::bad_alloc &e) {
    ASSERT_TRUE(false);
  } catch (const std::invalid_argument &e) {
    ASSERT_TRUE(false);
  }
}

TYPED_TEST(StudentTestAllocator0, test_5) {
  // test left coalesce
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p1 = x.allocate(s);
    pointer p2 = x.allocate(s);
    x.deallocate(p1, s);
    x.deallocate(p2, s);
  } catch (const std::bad_alloc &e) {
    ASSERT_TRUE(false);
  } catch (const std::invalid_argument &e) {
    ASSERT_TRUE(false);
  }
}

TYPED_TEST(StudentTestAllocator0, test_6) {
  // test right coalesce, then left coalesce
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  try {
    pointer p1 = x.allocate(s);
    pointer p2 = x.allocate(s);
    pointer p3 = x.allocate(s);
    x.deallocate(p2, s);
    x.deallocate(p1, s);
    x.deallocate(p3, s);
  } catch (const std::bad_alloc &e) {
    ASSERT_TRUE(false);
  } catch (const std::invalid_argument &e) {
    ASSERT_TRUE(false);
  }
}

TYPED_TEST(StudentTestAllocator0, test_7) {
  // test simple construct and deconstruct
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  pointer p = x.allocate(s);
  x.construct(p, v);
  ASSERT_EQ(v, *p);
  x.destroy(p);
  x.deallocate(p, s);
}

template <typename A> struct StudentTestAllocator1 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<char, 900>, Allocator<int, 1200>,
                       Allocator<double, 1600>, Allocator<short, 1000>>
    student_types_1;

TYPED_TEST_CASE(StudentTestAllocator1, student_types_1);

TYPED_TEST(StudentTestAllocator1, test_1) {
  // test bad_alloc exception by creating more after FULL
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  pointer ptr[101];
  try {
    // call 100 times
    for (int i = 0; i < 100; ++i) {
      ptr[i] = x.allocate(s);
    }
    // call after size getting FULL
    ptr[100] = x.allocate(s);
    ASSERT_TRUE(false);
  } catch (const std::bad_alloc &e) {
    ASSERT_TRUE(true);
    for (int i = 0; i < 100; ++i) {
      x.deallocate(ptr[i], s);
    }
  }
}

TYPED_TEST(StudentTestAllocator1, test_2) {
  // call allocate 100 tiems and deallocate all of them
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  pointer ptr[100];
  try {
    // call 100 times
    for (int i = 0; i < 100; ++i) {
      ptr[i] = x.allocate(s);
    }
    // call after size getting FULL
    ASSERT_TRUE(true);
    for (int i = 0; i < 100; ++i) {
      x.deallocate(ptr[i], s);
    }
  } catch (const std::bad_alloc &e) {
    ASSERT_TRUE(false);
  } catch (const std::invalid_argument &e) {
    ASSERT_TRUE(false);
  }
}

TYPED_TEST(StudentTestAllocator1, test_3) {
  // test simple construct and deconstruct
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 5;

  pointer ptr[100];
  // create 100 poiners of size s with value v
  for (int i = 0; i < 100; ++i) {
    ptr[i] = x.allocate(s);
    x.construct(ptr[i], v);
  }
  // check the value and destory them all
  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(v, *ptr[i]);
    x.destroy(ptr[i]);
    x.deallocate(ptr[i], s);
  }
}

// Same strucutre as StudentTestAllocator1 test_3 with larger container
template <typename A> struct StudentStressTestAllocator1 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<Allocator<char, 9000>, Allocator<int, 12000>,
                       Allocator<double, 16000>, Allocator<short, 10000>>
    student_types_2;

TYPED_TEST_CASE(StudentStressTestAllocator1, student_types_2);

TYPED_TEST(StudentStressTestAllocator1, test_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 5;

  pointer ptr[1000];
  // create 100 poiners of size s with value v
  for (int i = 0; i < 1000; ++i) {
    ptr[i] = x.allocate(s);
    x.construct(ptr[i], v);
  }
  // check the value and destory them all
  for (int i = 0; i < 1000; ++i) {
    ASSERT_EQ(v, *ptr[i]);
    x.destroy(ptr[i]);
    x.deallocate(ptr[i], s);
  }
}

// Professor Downing's Test + MyTests
TEST(TestAllocator2, const_index) {
  const Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);

  // test bigger size
  const Allocator<int, 1000> y;
  ASSERT_EQ(y[0], 992);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);

  // test bigger size
  Allocator<int, 1000> y;
  ASSERT_EQ(y[0], 992);
}

// Professor Downing's Test
// --------------
// TestAllocator0
// --------------

template <typename A> struct TestAllocator0 : testing::Test {
  // --------
  // typedefs
  // --------

  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       Allocator<int, 100>, Allocator<double, 100>>
    my_types_0;

TYPED_TEST_CASE(TestAllocator0, my_types_0);

TYPED_TEST(TestAllocator0, test_1) {
  // basic allocation test
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;

  allocator_type x;
  const size_type s = 1;
  const value_type v = 2;
  pointer p = x.allocate(s);
  x.deallocate(p, s);
}

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
                       Allocator<int, 100>, Allocator<double, 100>>
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

typedef testing::Types<Allocator<int, 100>, Allocator<double, 100>> my_types_2;

TYPED_TEST_CASE(TestAllocator3, my_types_2);

TYPED_TEST(TestAllocator3, test_1) {
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

TYPED_TEST(TestAllocator3, test_10) {
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