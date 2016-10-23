// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

template <typename A> struct TestAllAllocators : testing::Test {
  typedef A allocator_type;
  typedef typename A::value_type value_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
};

typedef testing::Types<std::allocator<int>, std::allocator<double>,
                       my_allocator<int, 100>, my_allocator<double, 100>>
    allocatorTypes;
TYPED_TEST_CASE(TestAllAllocators, allocatorTypes);

TYPED_TEST(TestAllAllocators, constuct_1) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type size = 1;
  const value_type value = 2;
  const pointer p = x.allocate(size);
  if (p) {
    x.construct(p, value);
    EXPECT_EQ(value, *p);
    x.destroy(p);
    x.deallocate(p, size);
  }
}

TYPED_TEST(TestAllAllocators, construct_many) {
  typedef typename TestFixture::allocator_type allocator_type;
  typedef typename TestFixture::value_type value_type;
  typedef typename TestFixture::size_type size_type;
  typedef typename TestFixture::pointer pointer;
  allocator_type x;
  const size_type size = 10;
  const value_type value = 2;
  const pointer begin = x.allocate(size);
  if (begin) {
    pointer end = begin + size;
    pointer p = begin;
    while (p != end) {
      x.construct(p, value);
      ++p;
    }
    EXPECT_EQ(size, std::count(begin, end, value));
    while (begin != end) {
      --end;
      x.destroy(end);
    }
    x.deallocate(begin, size);
  }
}

// --------------
// TestAllocator
// --------------

template <typename T> struct TestTypes : testing::Test {
  typedef T type;
  static constexpr std::size_t size = sizeof(T);
};

struct vec3 {
  float ar[3];
};
typedef testing::Types<char, int, double, vec3> dataTypes;
TYPED_TEST_CASE(TestTypes, dataTypes);

TYPED_TEST(TestTypes, allocator_size) {
  using type = typename TestFixture::type;
#define test(size, expectError)                                                \
  {                                                                            \
    bool errorCaught = false;                                                  \
    try {                                                                      \
      my_allocator<type, size> x;                                              \
    } catch (const std::bad_alloc &e) {                                        \
      errorCaught = true;                                                      \
    }                                                                          \
    EXPECT_EQ(expectError, errorCaught);                                       \
  }
  test(0, true);
  test(TestFixture::size, true);
  test(TestFixture::size + 7, true);
  test(TestFixture::size + 8, false);
  test(TestFixture::size + 9, false);
#undef test
}

TYPED_TEST(TestTypes, alloc_1) {
  using type = typename TestFixture::type;
  my_allocator<type, TestFixture::size + 8> x;

  type *const p = x.allocate(1);
  EXPECT_TRUE(p);
  x.deallocate(p, 1);
}

TYPED_TEST(TestTypes, alloc_2) {
  using type = typename TestFixture::type;
  my_allocator<type, (TestFixture::size + 8) * 2> x;

  type *const p1 = x.allocate(1);
  type *const p2 = x.allocate(1);
  EXPECT_TRUE(p1);
  EXPECT_TRUE(p2);
}

TYPED_TEST(TestTypes, alloc_2_contiguous) {
  using type = typename TestFixture::type;
  my_allocator<type, TestFixture::size * 2 + 8> x;

  type *const p = x.allocate(2);
  EXPECT_TRUE(p);
}

TYPED_TEST(TestTypes, alloc_dealloc_alloc) {
  using type = typename TestFixture::type;
  my_allocator<type, TestFixture::size * 2 + 8> x;

  type *const p = x.allocate(2);
  x.deallocate(p, 1);
  EXPECT_TRUE(p);
}

TYPED_TEST(TestTypes, alloc_fail) {
  using type = typename TestFixture::type;
  my_allocator<type, TestFixture::size + 8> x;

  bool caught = false;
  try {
    type *const p = x.allocate(2);
  } catch (std::bad_alloc &x) {
    caught = true;
  }
  EXPECT_TRUE(caught);
}

TYPED_TEST(TestTypes, dealloc_coalesce) {
  using type = typename TestFixture::type;
  my_allocator<type, (TestFixture::size + 8) * 5> x;

  type *const p[5] = {x.allocate(1), x.allocate(1), x.allocate(1),
                      x.allocate(1), x.allocate(1)};
  for (const auto &it : p) {
    EXPECT_TRUE(it);
    x.deallocate(it, 1);
  }
  type *const p2 = x.allocate(5);
  EXPECT_TRUE(p2);
  x.deallocate(p2, 1);
}

TYPED_TEST(TestTypes, dealloc_coalesce_unordered) {
  using type = typename TestFixture::type;
  my_allocator<type, (TestFixture::size + 8) * 5> x;

  type *const p[5] = {x.allocate(1), x.allocate(1), x.allocate(1),
                      x.allocate(1), x.allocate(1)};
  for (const auto &it : {2, 4, 1, 0, 3}) {
    EXPECT_TRUE(p[it]);
    x.deallocate(p[it], 1);
  }
  type *const p2 = x.allocate(5);
  EXPECT_TRUE(p2);
  x.deallocate(p2, 5);
}

TYPED_TEST(TestTypes, dealloc_null) {
  using type = typename TestFixture::type;
  my_allocator<type, (TestFixture::size + 8) * 5> x;

  bool errorCaught = false;
  try {
    x.deallocate(nullptr, 1);
  } catch (const std::invalid_argument &e) {
    errorCaught = true;
  }
  EXPECT_TRUE(errorCaught);
}

TYPED_TEST(TestTypes, dealloc_freed) {
  using type = typename TestFixture::type;
  my_allocator<type, (TestFixture::size + 8) * 5> x;
  type *const p = x.allocate(1);
  x.deallocate(p, 1);

  bool errorCaught = false;
  try {
    x.deallocate(p, 1);
  } catch (const std::invalid_argument &e) {
    errorCaught = true;
  }
  EXPECT_TRUE(errorCaught);
}

// --------------
// Test special cases
// --------------

TEST(TestAllocatorIndex, const_index) {
  const my_allocator<int, 100> x;
  EXPECT_EQ(x[0], 92);
}

TEST(TestAllocatorIndex, index) {
  my_allocator<int, 100> x;
  EXPECT_EQ(x[0], 92);
}
