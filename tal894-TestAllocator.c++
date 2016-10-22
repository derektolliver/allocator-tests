// ------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// ------------------------------------

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
// TestAllocator2
// --------------

TEST(TestAllocator2, const_index) {
  const Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, const_index_end_sentinal) {
  const Allocator<int, 100> x;
  ASSERT_EQ(x[96], 92);
}

TEST(TestAllocator2, index) {
  Allocator<int, 100> x;
  ASSERT_EQ(x[0], 92);
}

TEST(TestAllocator2, index_end_sentinal) {
  Allocator<int, 100> x;
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

// --------------
// TestAllocator4
// --------------

TEST(TestAllocator4, allocate_zero) {
  Allocator<int, 100> x;
  auto s = 0;
  auto b = x.allocate(s);
  ASSERT_EQ(b, nullptr);
}

TEST(TestAllocator4, allocate_all) {
  Allocator<int, 100> x;
  auto s = 23;
  try {
    auto b = x.allocate(s);
    ASSERT_EQ(1, 1);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(0, 1);
  }
}

TEST(TestAllocator4, allocate_over) {
  Allocator<int, 100> x;
  auto s = 100;
  try {
    auto b = x.allocate(s);
    ASSERT_EQ(0, 1);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, constructor_good) {
  try {
    const Allocator<int, 12> x;
    ASSERT_EQ(x[0], 4);
    ASSERT_EQ(x[8], 4);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 0);
  }
}

TEST(TestAllocator4, constructor_bad) {
  try {
    Allocator<int, 11> x;
    ASSERT_EQ(1, 0);
  } catch (std::bad_alloc &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_coalesce_left) {
  Allocator<int, 100> x;
  typedef typename Allocator<int, 100>::value_type value_type;
  typedef typename Allocator<int, 100>::pointer pointer;

  const value_type v = 2;
  const int s[4] = {10, 8, 1, 23};
  pointer p[4] = {nullptr, nullptr, nullptr, nullptr};
  for (int i = 0; i < 4; ++i) {
    if (i == 3) {
      for (int j = 0; j < 3; ++j) {
        x.deallocate(p[j], s[j]);
      }
    }
    p[i] = x.allocate(s[i]);
    if (p[i] != nullptr) {
      pointer e = p[i] + s[i];
      pointer b = p[i];
      while (b != e) {
        x.construct(b, v);
        ++b;
      }
      ASSERT_EQ(s[i], std::count(p[i], e, v));
      while (p[i] != e) {
        --e;
        x.destroy(e);
      }
      if (i == 3) {
        x.deallocate(p[i], s[i]);
      }
    }
  }
}

TEST(TestAllocator4, deallocate_coalesce_right) {
  Allocator<int, 100> x;
  typedef typename Allocator<int, 100>::value_type value_type;
  typedef typename Allocator<int, 100>::pointer pointer;

  const value_type v = 2;
  const int s[4] = {10, 8, 1, 23};
  pointer p[4] = {nullptr, nullptr, nullptr, nullptr};
  for (int i = 0; i < 4; ++i) {
    if (i == 3) {
      for (int j = 2; j >= 0; --j) {
        x.deallocate(p[j], s[j]);
      }
    }
    p[i] = x.allocate(s[i]);
    if (p[i] != nullptr) {
      pointer e = p[i] + s[i];
      pointer b = p[i];
      while (b != e) {
        x.construct(b, v);
        ++b;
      }
      ASSERT_EQ(s[i], std::count(p[i], e, v));
      while (p[i] != e) {
        --e;
        x.destroy(e);
      }
      if (i == 3) {
        x.deallocate(p[i], s[i]);
      }
    }
  }
}

TEST(TestAllocator4, deallocate_coalesce_both) {
  Allocator<int, 100> x;
  typedef typename Allocator<int, 100>::value_type value_type;
  typedef typename Allocator<int, 100>::pointer pointer;

  const value_type v = 2;
  const int s[4] = {10, 8, 1, 23};
  pointer p[4] = {nullptr, nullptr, nullptr, nullptr};
  for (int i = 0; i < 4; ++i) {
    if (i == 3) {
      x.deallocate(p[0], s[0]);
      x.deallocate(p[2], s[2]);
      x.deallocate(p[1], s[1]);
    }
    p[i] = x.allocate(s[i]);
    if (p[i] != nullptr) {
      pointer e = p[i] + s[i];
      pointer b = p[i];
      while (b != e) {
        x.construct(b, v);
        ++b;
      }
      ASSERT_EQ(s[i], std::count(p[i], e, v));
      while (p[i] != e) {
        --e;
        x.destroy(e);
      }
      if (i == 3) {
        x.deallocate(p[i], s[i]);
      }
    }
  }
}

TEST(TestAllocator4, deallocate_nullptr) {
  Allocator<int, 100> x;
  try {
    x.deallocate(nullptr, 0);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_out_of_bounds) {
  Allocator<int, 100> x;
  auto s = 1;
  try {
    auto b = x.allocate(s);
    x.deallocate(b + 100, 0);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_wrong_ptr) {
  Allocator<int, 100> x;
  try {
    auto s = 3;
    auto p = x.allocate(s);
    x.deallocate(p - 1, s);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_wrong_ptr2) {
  Allocator<int, 100> x;
  try {
    auto s = 1;
    auto v = 2;
    auto p = x.allocate(s);
    x.construct(p, v);
    x.deallocate(p + 1, s);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_wrong_ptr3) {
  Allocator<int, 100> x;
  try {
    auto s = 1;
    auto p = x.allocate(s);
    new (p - 1) int(-100);
    x.deallocate(p, s);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_wrong_ptr4) {
  Allocator<int, 100> x;
  try {
    auto s = 1;
    auto v = 2;
    auto p = x.allocate(s);
    new (p - 1) int(4);
    new (p + 1) int(4);
    x.deallocate(p, s);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_wrong_ptr5) {
  Allocator<int, 100> x;
  try {
    auto s = 1;
    auto v = 2;
    auto p = x.allocate(s);
    new (p + 1) int(100);
    x.deallocate(p, s);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_wrong_n) {
  Allocator<int, 100> x;
  try {
    auto s = 3;
    auto p = x.allocate(s);
    x.deallocate(p, s + 1);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, deallocate_wrong_n2) {
  Allocator<int, 100> x;
  try {
    auto s = 5;
    auto p = x.allocate(s);
    x.deallocate(p, s - 3);
    ASSERT_EQ(0, 1);
  } catch (std::invalid_argument &e) {
    ASSERT_EQ(1, 1);
  }
}

TEST(TestAllocator4, not_valid_zero) {
  Allocator<int, 100> x;
  auto s = 23;
  auto p = x.allocate(s);
  new (p - 1) int(0);
  ASSERT_EQ(x.valid(), false);
}

TEST(TestAllocator4, not_valid_size) {
  Allocator<int, 100> x;
  auto s = 23;
  auto p = x.allocate(s);
  new (p - 1) int(92);
  ASSERT_EQ(x.valid(), false);
}
