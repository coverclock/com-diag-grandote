/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_FIBONACCI_
#define _H__COM_DIAG_UNITTEST_FIBONACCI_

/**
 * @file
 *
 * Copyright 2011-2013 Digital Aggregates Corporation, Colorado, USA<BR>
 * Licensed under the terms in README.h<BR>
 * Chip Overclock (coverclock@diag.com)<BR>
 * http://www.diag.com/navigation/downloads/Hayloft.html<BR>
 */

#include "gtest/gtest.h"
#include "Fixture.h"
#include "com/diag/desperado/Fibonacci.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::desperado;

typedef Fixture FibonacciTest;

TEST_F(FibonacciTest, Sanity) {
	Fibonacci fibonacci;
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.next(), 1);
	EXPECT_EQ(fibonacci.next(), 2);
	EXPECT_EQ(fibonacci.next(), 3);
	EXPECT_EQ(fibonacci.next(), 5);
	EXPECT_EQ(fibonacci.get(), 5);
	EXPECT_EQ(fibonacci.next(), 8);
	fibonacci.reset();
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.next(), 1);
	EXPECT_EQ(fibonacci.next(), 2);
	EXPECT_EQ(fibonacci.next(), 3);
	EXPECT_EQ(fibonacci.next(), 5);
	EXPECT_EQ(fibonacci.get(), 5);
	EXPECT_EQ(fibonacci.next(), 8);
	fibonacci.reset();
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ((unsigned int)fibonacci, 1);
	EXPECT_EQ((unsigned int)fibonacci, 2);
	EXPECT_EQ((unsigned int)fibonacci, 3);
	EXPECT_EQ((unsigned int)fibonacci, 5);
	EXPECT_EQ(fibonacci.get(), 5);
	EXPECT_EQ((unsigned int)fibonacci, 8);
}

TEST_F(FibonacciTest, Overflow) {
	Fibonacci fibonacci;
	unsigned long long sum;
	unsigned long long cum = 0;
	unsigned int ndx = 0;
	unsigned int now = 0;
	unsigned int was;
	printf("%3s %10s %10s %12s %12s\n", "NDX", "WAS", "NOW", "SUM", "CUM");
	do {
		was = now;
		now = fibonacci;
		ASSERT_NE(now, 0);
		sum = was;
		sum += now;
		cum += now;
		printf("%3u %10u %10u %12llu %12llu\n", ndx, was, now, sum, cum);
		++ndx;
	} while (was < now);
	ASSERT_NE(now, 0);
}

TEST_F(FibonacciTest, Limit) {
	Fibonacci fibonacci(5);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.get(), 1);
	EXPECT_EQ(fibonacci.next(), 1);
	EXPECT_EQ(fibonacci.next(), 2);
	EXPECT_EQ(fibonacci.next(), 3);
	EXPECT_EQ(fibonacci.next(), 5);
	EXPECT_EQ(fibonacci.get(), 5);
	EXPECT_EQ(fibonacci.next(), 5);
	EXPECT_EQ(fibonacci.next(), 5);
	EXPECT_EQ(fibonacci.next(), 5);
}

}
}
}

#endif
