// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------------

// -------
// defines
// -------
#define IS_TESTING

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator
#include <stdexcept> // range_error

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
  const int32_t sentinel = 100 - sizeof(uint32_t) * 2;
  ASSERT_EQ(x[0], sentinel);
}

TEST(TestAllocator2, index) {
  my_allocator<int, 100> x;
  const int32_t sentinel = 100 - sizeof(uint32_t) * 2;
  ASSERT_EQ(x[0], sentinel);
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

// ----------------
// Additional Tests
// ----------------

TEST(TestMyAllocatorAllocate, test_basic) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);

  // Checks for first fit condition
  ASSERT_EQ(&alloc[4], reinterpret_cast<int32_t *>(data_ptr));

  // Checks sentinel values
  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
}

TEST(TestMyAllocatorBlock, test_equality) {
  my_allocator<uint32_t, 100>::Block first_block(10, 10, false);
  my_allocator<uint32_t, 100>::Block second_block(10, 10, false);

  ASSERT_EQ(first_block, second_block);
}

TEST(TestMyAllocatorBlockIterator, test_general) {
  my_allocator<uint32_t, 100> alloc;
  my_allocator<uint32_t, 100>::Block first_block =
      alloc.get_block_from_index(alloc.SENTINEL_SIZE_BYTES);

  ASSERT_EQ(first_block.size, 100 - alloc.SENTINEL_SIZE_BYTES * 2);
  ASSERT_EQ(first_block.index, 4);
  ASSERT_EQ(first_block.is_occupied, false);

  // Create a block iterator using private methods in `my_allocator`
  my_allocator<uint32_t, 100>::block_iterator block_iter(&alloc, first_block);

  ASSERT_EQ(*block_iter, first_block);

  // Advance to the dummy block
  ++block_iter;

  // Check that going past the last block results in an exception
  bool exception_handled = false;
  try {
    ++block_iter;
  } catch (range_error &e) {
    exception_handled = true;
  }

  ASSERT_TRUE(exception_handled);
}

TEST(TestMyAllocatorDeallocate, test_initial) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  size_t s = 4;
  alloc.deallocate(data_ptr, s);

  my_allocator<uint32_t, 100>::Block tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 92);
}

TEST(TestMyAllocatorDeallocate, test_upper_bound) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  alloc.allocate(1);
  size_t s = 4;
  alloc.deallocate(data_ptr, s);

  my_allocator<uint32_t, 100>::Block tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 4);
}

TEST(TestMyAllocatorDeallocate, test_lower_bound) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1);
  uint32_t *data_ptr_second = alloc.allocate(1);
  size_t s = 4;
  alloc.deallocate(data_ptr_second, s);

  my_allocator<uint32_t, 100>::Block tested_block =
      alloc.get_block_from_index(16);

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 80);
}

TEST(TestMyAllocatorDeallocate, test_sandwhich_bound) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1);
  uint32_t *data_ptr_middle = alloc.allocate(1);
  alloc.allocate(1);
  size_t s = 4;
  alloc.deallocate(data_ptr_middle, s);

  my_allocator<uint32_t, 100>::Block tested_block =
      alloc.get_block_from_index(16);

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 4);
}

TEST(TestMyAllocatorDeallocate, test_upper_coalesce) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1);
  uint32_t *data_ptr_middle = alloc.allocate(1);
  uint32_t *data_ptr_upper = alloc.allocate(1);
  size_t s = 4;

  alloc.deallocate(data_ptr_upper, s);
  alloc.deallocate(data_ptr_middle, s);

  my_allocator<uint32_t, 100>::Block tested_block =
      alloc.get_block_from_index(16);

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 80);
}

TEST(TestMyAllocatorDeallocate, test_lower_coalesce) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  uint32_t *data_ptr_middle = alloc.allocate(1);
  alloc.allocate(1);
  size_t s = 4;

  alloc.deallocate(data_ptr, s);
  alloc.deallocate(data_ptr_middle, s);

  my_allocator<uint32_t, 100>::Block tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 16);
}

TEST(TestMyAllocatorDeallocate, test_sandwhich_coalesce) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  uint32_t *data_ptr_middle = alloc.allocate(1);
  uint32_t *data_ptr_upper = alloc.allocate(1);
  size_t s = 4;

  alloc.deallocate(data_ptr, s);
  alloc.deallocate(data_ptr_upper, s);
  alloc.deallocate(data_ptr_middle, s);

  my_allocator<uint32_t, 100>::Block tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 92);
}

TEST(TestMyAllocatorDeallocate, test_multi_sandwhich_coalesce) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  uint32_t *data_ptr_lower = alloc.allocate(2);
  uint32_t *data_ptr_middle = alloc.allocate(3);
  uint32_t *data_ptr_upper = alloc.allocate(4);
  size_t s = 4;

  alloc.deallocate(data_ptr, s);
  alloc.deallocate(data_ptr_upper, s);
  alloc.deallocate(data_ptr_middle, s);
  alloc.deallocate(data_ptr_lower, s);

  my_allocator<uint32_t, 100>::Block tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 92);
}

TEST(TestMyAllocatorDeallocate, test_multi_sandwhich_coalesce_2) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(3);
  uint32_t *data_ptr_lower = alloc.allocate(3);
  uint32_t *data_ptr_middle = alloc.allocate(3);
  uint32_t *data_ptr_upper = alloc.allocate(6);
  size_t s = 4;

  alloc.deallocate(data_ptr, s);
  alloc.deallocate(data_ptr_upper, s);
  alloc.deallocate(data_ptr_middle, s);
  alloc.deallocate(data_ptr_lower, s);

  my_allocator<uint32_t, 100>::Block tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 92);
}

TEST(TestMyAllocatorDeallocate, test_multi_sandwhich_coalesce_cascading) {
  my_allocator<uint32_t, 100> alloc;
  size_t s = 4;

  uint32_t *data_ptr = alloc.allocate(6);
  uint32_t *data_ptr_lower = alloc.allocate(4);
  alloc.deallocate(data_ptr, s);

  my_allocator<uint32_t, 100>::Block tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 24);

  uint32_t *data_ptr_middle = alloc.allocate(3);
  uint32_t *data_ptr_upper = alloc.allocate(4);

  alloc.deallocate(data_ptr_upper, s);
  alloc.deallocate(data_ptr_middle, s);
  alloc.deallocate(data_ptr_lower, s);

  tested_block = alloc.get_first_block();

  ASSERT_TRUE(alloc.get_start_sentinel_for_block(tested_block) > 0);
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block),
            alloc.get_start_sentinel_for_block(tested_block));
  ASSERT_EQ(alloc.get_terminating_sentinel_for_block(tested_block), 92);
}

TEST(TestMyAllocatorAllocate, test_two) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  alloc.allocate(1);

  // Checks for first fit condition
  ASSERT_EQ(&alloc[4], reinterpret_cast<int32_t *>(data_ptr));

  // Checks sentinel values
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
}

TEST(TestMyAllocatorAllocate, test_three) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  alloc.allocate(1);
  alloc.allocate(2);

  // Checks for first fit condition
  ASSERT_EQ(&alloc[4], reinterpret_cast<int32_t *>(data_ptr));

  // Checks sentinel values
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -8);
  ASSERT_EQ(alloc[36], -8);
}

TEST(TestMyAllocatorAllocate, test_reallocate) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  uint32_t *data_ptr_second = alloc.allocate(1);
  alloc.allocate(2);
  size_t s = 4;

  alloc.deallocate(data_ptr_second, s);
  alloc.allocate(1);

  // Checks for first fit condition
  ASSERT_EQ(&alloc[4], reinterpret_cast<int32_t *>(data_ptr));

  // Checks sentinel values
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -8);
  ASSERT_EQ(alloc[36], -8);
}

TEST(TestMyAllocatorAllocate, test_reallocate_twice) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  uint32_t *data_ptr_second = alloc.allocate(1);
  alloc.allocate(2);
  size_t s = 4;

  alloc.deallocate(data_ptr_second, s);
  data_ptr_second = alloc.allocate(1);
  alloc.deallocate(data_ptr_second, s);
  alloc.allocate(1);

  // Checks for first fit condition
  ASSERT_EQ(&alloc[4], reinterpret_cast<int32_t *>(data_ptr));

  // Checks sentinel values
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -8);
  ASSERT_EQ(alloc[36], -8);
}

TEST(TestMyAllocatorAllocate, test_reallocate_two) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  uint32_t *data_ptr_second = alloc.allocate(1);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  uint32_t *data_ptr_third = alloc.allocate(2);
  ASSERT_EQ(alloc[24], -8);
  ASSERT_EQ(alloc[36], -8);
  size_t s = 4;

  alloc.deallocate(data_ptr_second, s);
  ASSERT_EQ(alloc[12], 4);
  ASSERT_EQ(alloc[20], 4);
  alloc.allocate(2);
  ASSERT_EQ(alloc[12], 4);
  ASSERT_EQ(alloc[20], 4);
  ASSERT_EQ(alloc[40], -8);
  ASSERT_EQ(alloc[52], -8);
  alloc.deallocate(data_ptr_third, s);
  ASSERT_EQ(alloc[12], 20);
  ASSERT_EQ(alloc[36], 20);
  alloc.allocate(3);

  // Checks for first fit condition
  ASSERT_EQ(&alloc[4], reinterpret_cast<int32_t *>(data_ptr));

  // Checks sentinel values
  ASSERT_EQ(alloc[12], -20);
  ASSERT_EQ(alloc[36], -20);
}

TEST(TestMyAllocatorAllocate, test_reallocate_three) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);
  uint32_t *data_ptr_second = alloc.allocate(1);
  uint32_t *data_ptr_third = alloc.allocate(2);
  size_t s = 4;

  alloc.deallocate(data_ptr, s);
  alloc.deallocate(data_ptr_second, s);
  alloc.deallocate(data_ptr_third, s);
  ASSERT_EQ(alloc[0], alloc[96]);
  ASSERT_EQ(alloc[0], 92);
  alloc.allocate(1);
  ASSERT_EQ(alloc[12], alloc[96]);
  ASSERT_EQ(alloc[12], 80);
  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  alloc.allocate(1);
  ASSERT_EQ(alloc[24], alloc[96]);
  ASSERT_EQ(alloc[24], 68);
  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  alloc.allocate(1);

  // Checks sentinel values
  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -4);
  ASSERT_EQ(alloc[32], -4);
  ASSERT_EQ(alloc[36], alloc[96]);
  ASSERT_EQ(alloc[36], 56);
}
TEST(TestMyAllocatorAllocate, test_reallocate_three_unordered) {
  my_allocator<uint32_t, 100> alloc;

  uint32_t *data_ptr = alloc.allocate(1);        // 0,8
  uint32_t *data_ptr_second = alloc.allocate(1); // 12,20
  uint32_t *data_ptr_third = alloc.allocate(2);  // 24,36
  size_t s = 4;

  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -8);
  ASSERT_EQ(alloc[36], -8);

  alloc.deallocate(data_ptr, s); // free 0,8
  alloc.allocate(2);             // 40, 48

  ASSERT_EQ(alloc[0], 4);
  ASSERT_EQ(alloc[8], 4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -8);
  ASSERT_EQ(alloc[36], -8);
  ASSERT_EQ(alloc[40], -8);
  ASSERT_EQ(alloc[52], -8);
  ASSERT_EQ(alloc[56], 36);
  ASSERT_EQ(alloc[96], 36);

  alloc.deallocate(data_ptr_second, s);

  ASSERT_EQ(alloc[0], 16);
  ASSERT_EQ(alloc[20], 16);
  ASSERT_EQ(alloc[24], -8);
  ASSERT_EQ(alloc[36], -8);
  ASSERT_EQ(alloc[40], -8);
  ASSERT_EQ(alloc[52], -8);
  ASSERT_EQ(alloc[56], 36);
  ASSERT_EQ(alloc[96], 36);

  alloc.deallocate(data_ptr_third, s);

  ASSERT_EQ(alloc[0], 32);
  ASSERT_EQ(alloc[36], 32);
  ASSERT_EQ(alloc[40], -8);
  ASSERT_EQ(alloc[52], -8);
  ASSERT_EQ(alloc[56], 36);
  ASSERT_EQ(alloc[96], 36);

  alloc.allocate(1);

  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], 20);
  ASSERT_EQ(alloc[36], 20);
  ASSERT_EQ(alloc[40], -8);
  ASSERT_EQ(alloc[52], -8);
  ASSERT_EQ(alloc[56], 36);
  ASSERT_EQ(alloc[96], 36);

  alloc.allocate(1);

  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], 8);
  ASSERT_EQ(alloc[36], 8);
  ASSERT_EQ(alloc[40], -8);
  ASSERT_EQ(alloc[52], -8);
  ASSERT_EQ(alloc[56], 36);
  ASSERT_EQ(alloc[96], 36);
}
TEST(TestMyAllocatorAllocate, test_max_combine) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1); // 0,8
  alloc.allocate(1); // 12,20
  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], 68);
  ASSERT_EQ(alloc[96], 68);

  alloc.allocate(16);
  size_t s = 4;

  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -68);
  ASSERT_EQ(alloc[96], -68);
}
TEST(TestMyAllocatorAllocate, test_max) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1); // 0,8
  alloc.allocate(1); // 12,20
  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], 68);
  ASSERT_EQ(alloc[96], 68);

  alloc.allocate(17);
  size_t s = 4;

  ASSERT_EQ(alloc[0], -4);
  ASSERT_EQ(alloc[8], -4);
  ASSERT_EQ(alloc[12], -4);
  ASSERT_EQ(alloc[20], -4);
  ASSERT_EQ(alloc[24], -68);
  ASSERT_EQ(alloc[96], -68);
}

TEST(TestMyAllocatorAllocate, test_too_big) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1); // 0,8
  alloc.allocate(1); // 12,20
  bool exception_handled = false;
  try {
    alloc.allocate(18);
  } catch (bad_alloc &e) {
    exception_handled = true;
  }

  ASSERT_TRUE(exception_handled);
}

TEST(TestMyAllocatorAllocate, test_too_big_sandwhich) {
  my_allocator<uint32_t, 100> alloc;
  size_t s = 4;

  alloc.allocate(1);                             // 0,8
  uint32_t *data_ptr_second = alloc.allocate(1); // 12,20
  alloc.allocate(17);                            // 12,20

  alloc.deallocate(data_ptr_second, s);

  bool exception_handled = false;
  try {
    alloc.allocate(2);
  } catch (bad_alloc &e) {
    exception_handled = true;
  }

  ASSERT_TRUE(exception_handled);
}

TEST(TestMyAllocatorValid, test_empty) {
  my_allocator<uint32_t, 100> alloc;

  ASSERT_TRUE(alloc.valid());
}
TEST(TestMyAllocatorValid, test_one) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1);
  ASSERT_TRUE(alloc.valid());
}
TEST(TestMyAllocatorValid, test_two) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1);
  alloc.allocate(1);

  ASSERT_TRUE(alloc.valid());
}
TEST(TestMyAllocatorValid, test_three) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(4);
  alloc.allocate(3);
  alloc.allocate(2);

  ASSERT_TRUE(alloc.valid());
}

TEST(TestMyAllocatorValid, test_empty_fail) {
  my_allocator<uint32_t, 100> alloc;

  alloc.a[0] = 0;

  ASSERT_FALSE(alloc.valid());
}
TEST(TestMyAllocatorValid, test_one_fail) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1);

  alloc.a[12] *= -1;

  ASSERT_FALSE(alloc.valid());
}
TEST(TestMyAllocatorValid, test_two_fail) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(1);
  alloc.allocate(1);

  alloc.a[12] = -16;

  ASSERT_FALSE(alloc.valid());
}
TEST(TestMyAllocatorValid, test_three_fail) {
  my_allocator<uint32_t, 100> alloc;

  alloc.allocate(4);
  alloc.allocate(3);
  alloc.allocate(2);

  alloc.a[0] += 1;

  ASSERT_FALSE(alloc.valid());
}

TEST(TestMyAllocatorValid, test_smaller_block_fail) {
  my_allocator<uint32_t, 100> alloc;

  alloc.a[0] = -3;
  alloc[7] = -3;
  alloc[11] = 81;
  alloc[96] = 81;

  ASSERT_FALSE(alloc.valid());
}

TEST(TestMyAllocatorValid, test_not_reachable_fail) {
  my_allocator<uint32_t, 100> alloc;

  alloc[0] = -4;
  alloc[8] = -4;

  ASSERT_FALSE(alloc.valid());
}

TEST(TestMyAllocatorValid, test_too_big_fail) {
  my_allocator<uint32_t, 100> alloc;

  alloc.a[0] = 104;

  ASSERT_FALSE(alloc.valid());
}

TEST(TestMyAllocatorConstruct, test_small_int) {
  my_allocator<uint32_t, 12> alloc;
  ASSERT_EQ(alloc[0], alloc[8]);
  ASSERT_EQ(alloc[0], 4);
}
TEST(TestMyAllocatorConstruct, test_big_int) {
  my_allocator<uint32_t, 1200> alloc;
  ASSERT_EQ(alloc[0], alloc[1196]);
  ASSERT_EQ(alloc[0], 1192);
}

TEST(TestMyAllocatorConstruct, test_small_double) {
  my_allocator<double, 16> alloc;
  ASSERT_EQ(alloc[0], alloc[12]);
  ASSERT_EQ(alloc[0], 8);
}
TEST(TestMyAllocatorConstruct, test_big_double) {
  my_allocator<double, 1200> alloc;
  ASSERT_EQ(alloc[0], alloc[1196]);
  ASSERT_EQ(alloc[0], 1192);
}

TEST(TestMyAllocatorConstruct, test_small_float) {
  my_allocator<float, 16> alloc;
  ASSERT_EQ(alloc[0], alloc[12]);
  ASSERT_EQ(alloc[0], 8);
}
TEST(TestMyAllocatorConstruct, test_big_float) {
  my_allocator<float, 1200> alloc;
  ASSERT_EQ(alloc[0], alloc[1196]);
  ASSERT_EQ(alloc[0], 1192);
}

TEST(TestMyAllocatorAllocate, test_small_double_allocate) {
  my_allocator<double, 16> alloc;
  alloc.allocate(1);
  ASSERT_EQ(alloc[0], alloc[12]);
  ASSERT_EQ(alloc[0], -8);
}
TEST(TestMyAllocatorAllocate, test_big_double_allocate) {
  my_allocator<double, 1200> alloc;
  alloc.allocate(100);
  ASSERT_EQ(alloc[0], alloc[804]);
  ASSERT_EQ(alloc[804], -800);
  ASSERT_EQ(alloc[808], alloc[1196]);
  ASSERT_EQ(alloc[808], 384);
}
TEST(TestMyAllocatorDeallocate, test_small_double_deallocate) {
  my_allocator<double, 16> alloc;
  double *ptr = alloc.allocate(1);
  size_t s = 8;

  alloc.deallocate(ptr, s);

  ASSERT_EQ(alloc[0], alloc[12]);
  ASSERT_EQ(alloc[0], 8);
}
TEST(TestMyAllocatorDeallocate, test_big_double_deallocate) {
  my_allocator<double, 1200> alloc;
  double *ptr = alloc.allocate(100);
  size_t s = 8;

  alloc.deallocate(ptr, s);
  ASSERT_EQ(alloc[0], alloc[1196]);
  ASSERT_EQ(alloc[0], 1192);
}
