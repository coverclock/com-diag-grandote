/* vim: set ts=4 expandtab shiftwidth=4: */
#ifndef _H_COM_DIAG_UNITTEST_SECONDS_
#define _H_COM_DIAG_UNITTEST_SECONDS_

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
#include "com/diag/desperado/Seconds.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/generics.h"

namespace com {
namespace diag {
namespace unittest {

using namespace ::com::diag::desperado;

typedef Fixture SecondsTest;

TEST_F(SecondsTest, Seconds) {
	static const unsigned int BAD = intmaxof(unsigned int);
	Seconds seconds;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	Epochalseconds then = seconds.then();
	EXPECT_EQ(then, 0);
	Epochalseconds now = seconds.now();
	printf("NOW=%lld\n", now);
	const char * zulu = seconds.zulu(now);
	ASSERT_NE(zulu, (char *)0);
	printf("ZULU=\"%s\"\n", zulu);
	year = BAD;
	month = BAD;
	day = BAD;
	hour = BAD;
	minute = BAD;
	second = BAD;
	seconds.zulu(now, year, month, day, hour, minute, second);
	EXPECT_NE(year, BAD);
	EXPECT_NE(month, BAD);
	EXPECT_NE(day, BAD);
	EXPECT_NE(hour, BAD);
	EXPECT_NE(minute, BAD);
	EXPECT_NE(second, BAD);
	printf("ZULU=<%4.4u-%2.2u-%2.2u %2.2u:%2.2u:%2.2u>\n", year, month, day, hour, minute, second);
	ASSERT_EQ(std::system("date -u"), 0);
	const char * juliet = seconds.juliet(now);
	ASSERT_NE(juliet, (char *)0);
	printf("JULIET=\"%s\"\n", juliet);
	year = BAD;
	month = BAD;
	day = BAD;
	hour = BAD;
	minute = BAD;
	second = BAD;
	seconds.juliet(now, year, month, day, hour, minute, second);
	EXPECT_NE(year, BAD);
	EXPECT_NE(month, BAD);
	EXPECT_NE(day, BAD);
	EXPECT_NE(hour, BAD);
	EXPECT_NE(minute, BAD);
	EXPECT_NE(second, BAD);
	printf("JULIET=<%4.4u-%2.2u-%2.2u %2.2u:%2.2u:%2.2u>\n", year, month, day, hour, minute, second);
	ASSERT_EQ(std::system("date"), 0);
}

}
}
}

#endif
