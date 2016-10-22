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

// -----------
// constructor
// -----------

TEST(MyTestAllocator, constructor_1) {
  my_allocator<int, 50> x;
  ASSERT_EQ(x[0], 42);
  ASSERT_EQ(x[46], 42);
}

TEST(MyTestAllocator, constructor_2) {
  my_allocator<double, 16> x;
  ASSERT_EQ(x[0], 8);
  ASSERT_EQ(x[12], 8);
}

TEST(MyTestAllocator, constructor_3) {
  bool correct = false;
  try {
    my_allocator<double, 12> x;
  } catch (bad_alloc &ba) {
    correct = true;
  }
  if (!correct)
    throw;
}

// --------
// allocate
// --------

TEST(MyTestAllocator, allocate_1) {
  my_allocator<int, 50> x;

  int *p = x.allocate(1);
  ASSERT_NE(p, nullptr);

  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], 30);
  ASSERT_EQ(x[46], 30);
}

TEST(MyTestAllocator, allocate_2) {
  my_allocator<int, 50> x;

  int i = 0;
  int *p[4];
  while (i < 4) {
    p[i] = x.allocate(1);
    ASSERT_NE(p[i++], nullptr);
  }

  ASSERT_EQ(x[0], -4);
  ASSERT_EQ(x[8], -4);
  ASSERT_EQ(x[12], -4);
  ASSERT_EQ(x[20], -4);
  ASSERT_EQ(x[24], -4);
  ASSERT_EQ(x[32], -4);
  ASSERT_EQ(x[36], -6);
  ASSERT_EQ(x[46], -6);
}

TEST(MyTestAllocator, allocate_3) {
  my_allocator<int, 50> x;

  bool correct = false;
  try {
    x.allocate(11);
  } catch (bad_alloc &ba) {
    correct = true;
  }
  if (!correct)
    throw;
}

// ----------
// deallocate
// ----------

TEST(MyTestAllocator, deallocate_1) {
  my_allocator<int, 50> x;

  int *p = x.allocate(1);
  x.construct(p, 3);
  x.destroy(p);
  x.deallocate(p, 1);

  ASSERT_EQ(x[0], 42);
  ASSERT_EQ(x[46], 42);
}

TEST(MyTestAllocator, deallocate_2) {
  my_allocator<int, 50> x;

  int *p[3];
  int i = 0;
  while (i < 3) {
    p[i] = x.allocate(1);
    x.construct(p[i++], 3);
  }
  x.destroy(p[0]);
  x.deallocate(p[0], 1);
  x.destroy(p[2]);
  x.deallocate(p[2], 1);

  ASSERT_EQ(x[0], 4);
  ASSERT_EQ(x[8], 4);
  ASSERT_EQ(x[12], -4);
  ASSERT_EQ(x[20], -4);
  ASSERT_EQ(x[24], 18);
  ASSERT_EQ(x[46], 18);
}

TEST(MyTestAllocator, deallocate_3) {
  my_allocator<int, 50> x;

  int *p[4];
  int i = 0;
  while (i < 4) {
    p[i] = x.allocate(1);
    x.construct(p[i++], 3);
  }
  while (i > 0) {
    x.destroy(p[--i]);
    x.deallocate(p[i], 1);
  }

  ASSERT_EQ(x[0], 42);
  ASSERT_EQ(x[46], 42);
}

// -----
// valid
// -----

TEST(MyTestAllocator, valid_1) {
  my_allocator<int, 50> x;
  x[0] = 50;
  ASSERT_FALSE(x.valid());
}

TEST(MyTestAllocator, valid_2) {
  my_allocator<int, 50> x;
  x[46] = 40;
  ASSERT_FALSE(x.valid());
}

TEST(MyTestAllocator, valid_3) {
  my_allocator<int, 50> x;
  x[0] = 12;
  x[16] = 12;
  x[20] = 22;
  x[46] = 22;
  ASSERT_FALSE(x.valid());
}
