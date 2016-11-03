// -------------------------------------
// projects/allocator/TestAllocator1.c++
// Copyright (C) 2015
// Glenn P. Downing
// Kshitij Delvadiya	KMD2557
// Vincent Chow			VYC232
// -------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator
#include <limits>	//max size_t

#include "gtest/gtest.h"

#include "Allocator.h"




TEST(TestAllocatorDowning, const_index) {
	const my_allocator<int, 100> x;
	ASSERT_EQ(x[0], 92);
}

TEST(TestAllocatorDowning, index) {
	my_allocator<int, 100> x;
	ASSERT_EQ(x[0], 92);
}



// // --------------
// // TestAllocator1
// // --------------

TEST(TestAllocator1, good_constructor1){
	const my_allocator<int, 1000> x;
	ASSERT_EQ(992, x[0]);
	ASSERT_EQ(992, x[1000 - sizeof(int)]);
}

TEST(TestAllocator1, good_constructor2){
	const my_allocator<double, 16> x;
	ASSERT_EQ(8, x[0]);
	ASSERT_EQ(8, x[16 - sizeof(int)]);
}

TEST(TestAllocator1, bad_constructor1){
	//Should throw bad_alloc since N < sizeof(T) + (2*sizeof(int))
	bool error_thrown = false;
	try{
		const my_allocator<int, 11> x;
	}catch(bad_alloc& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}

TEST(TestAllocator1, bad_constructor2){
	//Should throw bad_alloc since N < sizeof(T) + (2*sizeof(int))
	bool error_thrown = false;
	try{
		const my_allocator<double, 15> x;
	}catch(bad_alloc& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}



// // --------------
// // TestAllocator2
// // --------------




TEST(TestAllocator2, validSimple) {
	my_allocator<int, 100> x;
	x[0]= -20;
	x[24]= -20;
	x[28] = 64;
	x[96] = 64;
	ASSERT_TRUE(x.valid());
}

TEST(TestAllocator2, validNotCoalesced){
	my_allocator<int, 100> x;
	x[0]= 20;
	x[24]= 20;
	x[28] = 64;
	x[96] = 64;
	ASSERT_FALSE(x.valid());
}

TEST(TestAllocator2, validComplicated){
	my_allocator<double, 112> x;
	// 0		4	28		32	36	44	48		52	92		96		100	108	112
	// [-24]	24	[-24]	[8]	8	[8]	[-40]	40	[-40]	[-8]	8	[-8]
	x[0]= -24;
	x[28]= -24;
	x[32] = 8;
	x[44] = 8;
	x[48]= -40;
	x[92]= -40;
	x[96] = -8;
	x[108] = -8;
	ASSERT_TRUE(x.valid());
}





// // --------------
// // TestAllocator3
// // --------------

TEST(TestAllocator3, bad_allocate_less_than_0){
	//Should throw bad_alloc since n < 0
	bool error_thrown = false;
	my_allocator<int, 100> x;
	try{
		x.allocate(-1);
	}catch(bad_alloc& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}

TEST(TestAllocator3, bad_allocate_greater_than_N){
	//Should throw bad alloc since n > N
	bool error_thrown = false;
	my_allocator<int, 100> x;
	try{
		x.allocate(100 - 2*sizeof(int) + 1);
	}catch(bad_alloc& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}

TEST(TestAllocator3, bad_allocate_no_space){
	//Should throw bad alloc since n > amount of space left
	bool error_thrown = false;
	my_allocator<int, 100> x;
	x.allocate(5);
	try{
		x.allocate(17);
	}catch(bad_alloc& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}

TEST(TestAllocator3, allocate_once){
	my_allocator<int, 100> x;
	int* p = x.allocate(5);
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(64, x[28]);
	ASSERT_EQ(64, x[96]);
	ASSERT_EQ(&x[0 + sizeof(int)], p);
}

TEST(TestAllocator3, allocate_twice_split){
	//Allocate twice, split the second block
	my_allocator<int, 100> x;
	int* p1 = x.allocate(5);
	// 0            24      28                                  96
	// [-20]    20  [-20]   [64]            64                  [64]
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(64, x[28]);
	ASSERT_EQ(64, x[96]);
	ASSERT_EQ(&x[0 + sizeof(int)], p1); 

	int* p2 = x.allocate(13);
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(-52, x[28]);
	ASSERT_EQ(-52, x[84]);
	ASSERT_EQ(4, x[88]);
	ASSERT_EQ(4, x[96]);
	ASSERT_EQ(&x[0 + 3*sizeof(int) + 5*sizeof(int)], p2);    
}

TEST(TestAllocator3, allocate_twice_whole){
	//Allocate twice, take up the whole second block
	my_allocator<int, 100> x;
	int* p1 = x.allocate(5);
	// 0            24      28                                  96
	// [-20]    20  [-20]   [64]            64                  [64]
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(64, x[28]);
	ASSERT_EQ(64, x[96]);
	ASSERT_EQ(&x[0 + sizeof(int)], p1);

	int* p2 = x.allocate(14);
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(-64, x[28]);
	ASSERT_EQ(-64, x[96]);
	ASSERT_EQ(&x[0 + 3*sizeof(int) + 5*sizeof(int)], p2);    
}

TEST(TestAllocator3, allocate_all){
	//Allocate 3 times, take up the entire space and try to allocate 1 more
	my_allocator<int, 100> x; 
	int* p1 = x.allocate(5);
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(64, x[28]);
	ASSERT_EQ(64, x[96]);    
	ASSERT_EQ(&x[0 + sizeof(int)], p1); 

	int* p2 = x.allocate(3);
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(-12, x[28]);
	ASSERT_EQ(-12, x[44]);
	ASSERT_EQ(44, x[48]);    
	ASSERT_EQ(44, x[96]);
	ASSERT_EQ(&x[0 + 3*sizeof(int) + 5*sizeof(int)], p2);       

	int* p3 = x.allocate(11);
	ASSERT_EQ(-20, x[0]);
	ASSERT_EQ(-20, x[24]);
	ASSERT_EQ(-12, x[28]);
	ASSERT_EQ(-12, x[44]);
	ASSERT_EQ(-44, x[48]);    
	ASSERT_EQ(-44, x[96]);
	ASSERT_EQ(&x[0 + 5*sizeof(int) + 8*sizeof(int)], p3);

	bool error_thrown = false;
	try{
		x.allocate(1);
	}catch(bad_alloc& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}








// // --------------
// // TestAllocator4
// // --------------


TEST(TestAllocator4, deallocate_good_pointer){
	my_allocator<int, 100> x;
	int* p = x.allocate(16);
	bool error_thrown = false;
	try{
		x.deallocate(p, 16);
	}catch(invalid_argument& e){
		error_thrown = true;
	}
	ASSERT_FALSE(error_thrown);
}

TEST(TestAllocator4, deallocate_bad_pointer1){
	my_allocator<int, 100> x;
	int* p = x.allocate(16);
	bool error_thrown = false;
	try{
		x.deallocate(p + 4, 16);
	}catch(invalid_argument& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}

TEST(TestAllocator4, deallocate_bad_pointer2){
	my_allocator<int, 100> x;
	int* p = x.allocate(16);
	bool error_thrown = false;
	try{
		x.deallocate(p - 4, 16);
	}catch(invalid_argument& e){
		error_thrown = true;
	}
	ASSERT_TRUE(error_thrown);
}


TEST(TestAllocator4, deallocateSimple){
	my_allocator<int, 100> x;
	x.allocate(5);
	int* p2 = x.allocate(16);
	x.deallocate(p2, 5);
	const my_allocator<int, 100> y(x);
	// 0            24      28                                  96
	// [-20] 20      [-20]    [-64]            64                  [-64]
	ASSERT_EQ(-20 , y[0] );
	ASSERT_EQ(-20 , y[24]);
	ASSERT_EQ( 64 , y[28]);
	ASSERT_EQ( 64 , y[96]);
}

TEST(TestAllocator4, deallocateCoalesceSimpleRight){
	my_allocator<int, 100> x;
	int* p1 = x.allocate(5);
	int* p2 = x.allocate(16);
	x.deallocate(p2, 5);
	const my_allocator<int, 100> y(x);
	// 0            24      28                                  96
	// [-20] 20      [-20]    [-64]            64                  [-64]
	ASSERT_EQ(-20 , y[0] );
	ASSERT_EQ(-20 , y[24]);
	ASSERT_EQ( 64 , y[28]);
	ASSERT_EQ( 64 , y[96]);
	
	x.deallocate(p1, 16);
	const my_allocator<int, 100> z(x);
	ASSERT_EQ(92, z[0] );
	ASSERT_EQ(92, z[96]);
}

TEST(TestAllocator4, deallocateCoalesceRight){
	my_allocator<int, 100> x;
	int* p1 = x.allocate(5);
	int* p2 = x.allocate(3);
	// 0            24      28          44      48                          96
	// [-20]    20  [-20]   [-12]   12  [-12]   [44]    44                  [44]
	const my_allocator<int, 100> a(x);
	ASSERT_EQ( -20 , a[0] );
	ASSERT_EQ( -20 , a[24]);
	ASSERT_EQ( -12 , a[28]);
	ASSERT_EQ( -12 , a[44]);
	ASSERT_EQ(  44 , a[48]);
	ASSERT_EQ(  44 , a[96]);    
	
	// 0          24      28               96
	// [-20] 20  [-20]   [64]    64      [64]
	x.deallocate(p2, 3);
	const my_allocator<int, 100> b(x);
	ASSERT_EQ(-20 , b[0] );
	ASSERT_EQ(-20 , b[24]);
	ASSERT_EQ( 64 , b[28]);
	ASSERT_EQ( 64 , b[96]);
	
	// 0            96
	// [92]         [92] 
	x.deallocate(p1, 5);
	const my_allocator<int, 100> c(x);
	ASSERT_EQ(  92 , c[0] );
	ASSERT_EQ(  92 , c[96]);
}

TEST(TestAllocator4, deallocateCoalesceSimpleLeft){
	my_allocator<int, 100> x;
	int* p1 = x.allocate(5);
	int* p2 = x.allocate(16);
	// 0            24      28                                  96
	// [-20] 20    [-20]    [-64]            64                [-64]
	const my_allocator<int, 100> a(x);
	ASSERT_EQ( -20 , a[0] );
	ASSERT_EQ( -20 , a[24]);
	ASSERT_EQ( -64 , a[28]);
	ASSERT_EQ( -64 , a[96]);
	
	// 0            24      28                                  96
	// [20] 20    [20]    [-64]            64                [-64]
	x.deallocate(p1, 5);
	const my_allocator<int, 100> b(x);
	ASSERT_EQ(  20 , b[0] );
	ASSERT_EQ(  20 , b[24]);
	ASSERT_EQ( -64 , b[28]);
	ASSERT_EQ( -64 , b[96]);
	
	// 0                                                       96
	// [92]                         92                        [92]
	x.deallocate(p2, 16);
	const my_allocator<int, 100> c(x);
	ASSERT_EQ( 92 , c[0] );
	ASSERT_EQ( 92 , c[96]);
}


TEST(TestAllocator4, deallocateCoalesceLeft){
	my_allocator<int, 100> x;
	int* p1 = x.allocate(5);
	int* p2 = x.allocate(3);
	int* p3 = x.allocate(11);
	// 0            24      28          44      48                          96
	// [-20]    20  [-20]   [-12]   12  [-12]   [-44]    44                  [-44]
	const my_allocator<int, 100> a(x);
	ASSERT_EQ( -20 , a[0] );
	ASSERT_EQ( -20 , a[24]);
	ASSERT_EQ( -12 , a[28]);
	ASSERT_EQ( -12 , a[44]);
	ASSERT_EQ( -44 , a[48]);
	ASSERT_EQ( -44 , a[96]);    
	
	// 0            24      28          44      48                          96
	// [-20]    20  [-20]   [12]   12  [12]   [-44]    44                  [-44]
	x.deallocate(p2, 3);
	const my_allocator<int, 100> b(x);
	ASSERT_EQ( -20 , b[0] );
	ASSERT_EQ( -20 , b[24]);
	ASSERT_EQ(  12 , b[28]);
	ASSERT_EQ(  12 , b[44]);
	ASSERT_EQ( -44 , b[48]);
	ASSERT_EQ( -44 , b[96]);
	
	// 0                       44      48                          96
	// [40]         40         [40]   [-44]    44                  [-44]
	x.deallocate(p1, 5);
	const my_allocator<int, 100> c(x);
	ASSERT_EQ(  40 , c[0] );
	ASSERT_EQ(  40 , c[44]);
	ASSERT_EQ( -44 , c[48]);
	ASSERT_EQ( -44 , c[96]);
	
	// 0            24      28        44      48                          96
	// [92]                      92                                       [92]
	x.deallocate(p3, 11);
	const my_allocator<int, 100> d(x);
	ASSERT_EQ(  92 , d[0] );
	ASSERT_EQ(  92 , d[96]);
}

TEST(TestAllocator4, deallocateNotAligned){
	//	0			89
	//	[85]	85	[85]
	my_allocator<int, 93> x;
	
	//	0			24		28			89
	//	[-20]	20	[-20]	[57]	57	[57]
	int* p1 = x.allocate(5);
	const my_allocator<int, 93> a(x);
	ASSERT_EQ( -20 , a[0] );
	ASSERT_EQ( -20 , a[24]);
	ASSERT_EQ(  57 , a[28]);
	ASSERT_EQ(  57 , a[89]);

	//	0			24		28			60		64			89
	//	[-20]	20	[-20]	[-28]	28	[-28]	[21]	21	[21]
	int* p2 = x.allocate(7);
	const my_allocator<int, 93> b(x);
	ASSERT_EQ( -20 , b[0] );
	ASSERT_EQ( -20 , b[24]);
	ASSERT_EQ( -28 , b[28]);
	ASSERT_EQ( -28 , b[60]);
	ASSERT_EQ(  21 , b[64]);
	ASSERT_EQ(  21 , b[89]);

	//	0			24		28			60		64			89
	//	[-20]	20	[-20]	[-28]	28	[-28]	[-21]	21	[-21]
	int* p3 = x.allocate(5);
	const my_allocator<int, 93> c(x);
	ASSERT_EQ( -20 , c[0] );
	ASSERT_EQ( -20 , c[24]);
	ASSERT_EQ( -28 , c[28]);
	ASSERT_EQ( -28 , c[60]);
	ASSERT_EQ( -21 , c[64]);
	ASSERT_EQ( -21 , c[89]);

	//	0			24		28			60		64			89
	//	[20]	20	[20]	[-28]	28	[-28]	[-21]	21	[-21]
	x.deallocate(p1, 5);
	const my_allocator<int, 93> d(x);
	ASSERT_EQ(  20 , d[0] );
	ASSERT_EQ(  20 , d[24]);
	ASSERT_EQ( -28 , d[28]);
	ASSERT_EQ( -28 , d[60]);
	ASSERT_EQ( -21 , d[64]);
	ASSERT_EQ( -21 , d[89]);

	//	0			24		28			60		64			89
	//	[20]	20	[20]	[-28]	28	[-28]	[21]	21	[21]
	x.deallocate(p3, 5);
	const my_allocator<int, 93> e(x);
	ASSERT_EQ(  20 , e[0] );
	ASSERT_EQ(  20 , e[24]);
	ASSERT_EQ( -28 , e[28]);
	ASSERT_EQ( -28 , e[60]);
	ASSERT_EQ(  21 , e[64]);
	ASSERT_EQ(  21 , e[89]);

	//	0			89
	//	[85]	85	[85]
	x.deallocate(p2, 7);
	const my_allocator<int, 93> f(x);
	ASSERT_EQ(  85 , f[0] );
	ASSERT_EQ(  85 , f[89]);

}


// --------------
// TestAllocator5
// --------------

template <typename A>
struct TestAllocator5 : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A             allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::size_type  size_type;
    typedef typename A::pointer    pointer;
};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_1;

TYPED_TEST_CASE(TestAllocator5, my_types_1);

TYPED_TEST(TestAllocator5, test_1) {
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
        x.deallocate(p, s);
    }
}

TYPED_TEST(TestAllocator5, test_10) {
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
        x.deallocate(b, s);
    }
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
    typedef typename A::pointer    pointer;
};

typedef testing::Types<
            my_allocator<int,    100>,
            my_allocator<double, 100>>
        my_types_2;

TYPED_TEST_CASE(TestAllocator6, my_types_2);

TYPED_TEST(TestAllocator6, test_1) {
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
        x.deallocate(p, s);
    }
}

TYPED_TEST(TestAllocator6, test_10) {
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
        x.deallocate(b, s);
    }
}

