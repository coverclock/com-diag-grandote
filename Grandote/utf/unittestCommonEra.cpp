/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2004-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Desperadito library.
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    As a special exception, if other files instantiate templates or
    use macros or inline functions from this file, or you compile
    this file and link it with other works to produce a work based on
    this file, this file does not by itself cause the resulting work
    to be covered by the GNU Lesser General Public License. However
    the source code for this file must still be made available in
    accordance with the GNU Lesser General Public License.

    This exception does not invalidate any other reasons why a work
    based on this file might be covered by the GNU Lesser General
    Public License.

    Alternative commercial licensing terms are available from the copyright
    holder. Contact Digital Aggregates Corporation for more information.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 59 Temple Place, Suite 330,
    Boston, MA 02111-1307 USA, or http://www.gnu.org/copyleft/lesser.txt.



******************************************************************************/


/**
 *  @file
 *
 *  Implements the CommonEra unit test.
 *
 *  @see    CommonEra
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Date.h"
#include "com/diag/desperado/Date.h"
#include "com/diag/desperado/Time.h"
#include "com/diag/desperado/Time.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/LocalTime.h"
#include "com/diag/desperado/LocalTime.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/TimeStamp.h"
#include "com/diag/desperado/TimeStamp.h"
#include "com/diag/desperado/LeapSeconds.h"
#include "com/diag/desperado/LeapSeconds.h"
#include "com/diag/desperado/AtomicSeconds.h"
#include "com/diag/desperado/AtomicSeconds.h"
#include "com/diag/desperado/DstAlways.h"
#include "com/diag/desperado/DstAlways.h"
#include "com/diag/desperado/DstEu.h"
#include "com/diag/desperado/DstEu.h"
#include "com/diag/desperado/DstNever.h"
#include "com/diag/desperado/DstNever.h"
#include "com/diag/desperado/DstUs.h"
#include "com/diag/desperado/DstUs.h"
#include "com/diag/desperado/DstUs1966.h"
#include "com/diag/desperado/DstUs1966.h"
#include "com/diag/desperado/DstUs1986.h"
#include "com/diag/desperado/DstUs1986.h"
#include "com/diag/desperado/DstUs2007.h"
#include "com/diag/desperado/DstUs2007.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Desperado.h"

struct UT_LeapSecondDate {
    unsigned int year;
    unsigned int month;
    unsigned int day;
};

static UT_LeapSecondDate leapseconddate[] = {
    { 1972,  6, 30 },
    { 1972, 12, 31 },
    { 1973, 12, 31 },
    { 1974, 12, 31 },
    { 1975, 12, 31 },
    { 1976, 12, 31 },
    { 1977, 12, 31 },
    { 1978, 12, 31 },
    { 1979, 12, 31 },
    { 1981,  6, 30 },
    { 1982,  6, 30 },
    { 1983,  6, 30 },
    { 1985,  6, 30 },
    { 1987, 12, 31 },
    { 1989, 12, 31 },
    { 1990, 12, 31 },
    { 1992,  6, 30 },
    { 1993,  6, 30 },
    { 1994,  6, 30 },
    { 1995, 12, 31 },
    { 1997,  6, 30 },
    { 1998, 12, 31 },
    { 2005, 12, 31 }
};


static int testls(const CommonEra& lsd, bool expected, int expectedld) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    LeapSeconds& leapseconds = Platform::instance().getLeapSeconds();

    printf("%s[%d]: %04llu-%02u-%02uT%02u:%02u:%02u.%09uZ\n",
        __FILE__, __LINE__,
        lsd.getYear(), lsd.getMonth(), lsd.getDay(),
        lsd.getHour(), lsd.getMinute(), lsd.getSecond(),
        lsd.getNanosecond());

    int ld;
    const seconds_t* sp = leapseconds.find(lsd, ld);
    if (!expected) {
        if (0 != sp) {
            errorf("%s[%d]: (0!=0x%x) (%llu)!\n",
                __FILE__, __LINE__, sp, *sp);
                lsd.show();
                ++errors;
        } else {
            uint32_t nd;
            seconds_t sd = lsd.toSeconds(nd);
            CommonEra lsd2(0);
            lsd2.fromSeconds(sd, nd);
            if (
                (lsd.getYear()       != lsd2.getYear()) ||
                (lsd.getMonth()      != lsd2.getMonth()) ||
                (lsd.getDay()        != lsd2.getDay()) ||
                (lsd.getHour()       != lsd2.getHour()) ||
                (lsd.getMinute()     != lsd2.getMinute()) ||
                (lsd.getSecond()     != lsd2.getSecond()) ||
                (lsd.getNanosecond() != lsd2.getNanosecond())
           ) {
                errorf("%s[%d]: (lsd!=lsd2)!\n",
                    __FILE__, __LINE__);
                lsd.show();
                lsd2.show();
                ++errors;
            } else {
                uint32_t nd2 = 0;
                seconds_t sd2 = lsd2.toSeconds(nd2);
                if (sd2!=sd) {
                    errorf("%s[%d]: (%llu!=%llu)!\n",
                        __FILE__, __LINE__, sd2, sd);
                    lsd.show();
                    lsd2.show();
                    ++errors;
                }
                if (nd2!=nd) {
                    errorf("%s[%d]: (%u!=%u)!\n",
                        __FILE__, __LINE__, nd2, nd);
                    lsd.show();
                    lsd2.show();
                    ++errors;
                }
            }
        }
    } else {
        if (0 == sp) {
            errorf("%s[%d]: (0==0x%x)!\n",
                __FILE__, __LINE__, sp);
                lsd.show();
                ++errors;
        } else {
            uint32_t nd = lsd.getNanosecond();
            seconds_t sd = *sp;
            CommonEra lsd2(0);
            lsd2.fromSeconds(sd, nd);
            if (
                (lsd.getYear()       != lsd2.getYear()) ||
                (lsd.getMonth()      != lsd2.getMonth()) ||
                (lsd.getDay()        != lsd2.getDay()) ||
                (lsd.getHour()       != lsd2.getHour()) ||
                (lsd.getMinute()     != lsd2.getMinute()) ||
                (lsd.getSecond()     != lsd2.getSecond()) ||
                (lsd.getNanosecond() != lsd2.getNanosecond())
            ) {
                errorf("%s[%d]: (lsd!=lsd2)!\n",
                    __FILE__, __LINE__);
                lsd.show();
                lsd2.show();
                ++errors;
            } else {
                uint32_t nd2;
                seconds_t sd2 = lsd2.toSeconds(nd2);
                if (sd2!=sd) {
                    errorf("%s[%d]: (%llu!=%llu)!\n",
                        __FILE__, __LINE__, sd2, sd);
                    lsd.show();
                    lsd2.show();
                    ++errors;
                }
                if (nd2!=nd) {
                    errorf("%s[%d]: (%u!=%u)!\n",
                        __FILE__, __LINE__, nd2, nd);
                    lsd.show();
                    lsd2.show();
                    ++errors;
                }
            }
        }
    }

    if (expectedld != ld) {
        errorf("%s[%d]: (%ld!=%ld)!\n",
            __FILE__, __LINE__, expectedld, ld);
        lsd.show();
        ++errors;
    }

    return errors;
}

static int test0(
    Output& output,
    CommonEra& ce,
    unsigned long long yr,
    unsigned int mh,
    unsigned int dy,
    unsigned int hr,
    unsigned int me,
    unsigned int sd,
    bool lr,
    int wy,
    int jn,
    seconds_t ss,
    bool testseconds,
    unsigned int dpm
) {
    Print printf(output);
    Print errorf(Platform::instance().error());
    int errors = 0;
    CommonEra::String string;

    ce.show(1, &output);

    bool valid = ce.isValid();

    if (!valid) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, valid);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("true!=valid");
    }

    uint64_t year = ce.getYear();
    uint8_t month = ce.getMonth();
    uint8_t day = ce.getDay();
    uint8_t hour = ce.getHour();
    uint8_t minute = ce.getMinute();
    uint8_t second = ce.getSecond();
    uint32_t nanosecond = ce.getNanosecond();

    printf("object=(%llu,%u,%u,%u,%u,%u,%u)\n",
        year, month, day, hour, minute, second, nanosecond);

    size_t size = ce.toString(string);

    printf("string=\"%s\"[%lu]\n",
        string, size);

    uint8_t twelve; 

    ce.getTime().oclock(twelve);
    printf("timestamp=\"%s, %s %u, %04llu %02u:%02u%s\"\n",
        ce.getDate().weekdayToString(),
        ce.getDate().monthToString(), ce.getDay(), ce.getYear(),
        twelve, ce.getMinute(), ce.getTime().meridiemToString());

    printf("julian=%02llu%03u\n",
        ce.getYear() % 100, ce.julian());

    if (sizeof(CommonEra::String) <= size) {
        errorf("%s[%d]: (%lu<=%lu)!\n",
            __FILE__, __LINE__, sizeof(CommonEra::String), size);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("sizeof(CommonEra::String)<=size");
    }

    size_t octets = std::strlen(string);

    if (octets != size) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, octets, size);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("std:::strlen(string)!=size");
    }

    if (year != yr) {
        errorf("%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, year, yr);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("year!=yr");
    }

    if (month != mh) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, month, mh);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("month!=mh");
    }

    if (day != dy) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, day, dy);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("day!=dy");
    }

    if (hour != hr) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, hour, hr);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("hour!=hr");
    }

    if (minute != me) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, minute, me);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("minute!=me");
    }

    if (second != sd) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, second, sd);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("second!=sd");
    }

    if (nanosecond != 0) {
        errorf("%s[%d]: (%lu!=%lu)!?\n",
            __FILE__, __LINE__, nanosecond, 0);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("nanosecond!=0");
    }

    bool leapyear = ce.isLeapYear();

    if (leapyear != lr) {
        errorf("%s[%d]: (%d!=%d)!?\n",
            __FILE__, __LINE__, leapyear, lr);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("leapyear!=lr");
    }

    uint8_t days = ce.cardinal();

    if (days != dpm) {
        errorf("%s[%d]: (%u!=%u)!?\n",
            __FILE__, __LINE__, days, dpm);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("days!=dpm");
    }

    Date::Weekday weekday = ce.weekday();

    if (weekday != wy) {
        errorf("%s[%d]: (%d!=%d)!?\n",
            __FILE__, __LINE__, weekday, wy);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("weekday!=wy");
    }

    Date::Ordinal ord;

    if (dy <= 7) {
        ord = Date::FIRST;
    } else if (dy <= 14) {
        ord = Date::SECOND;
    } else if (dy <= 21) {
        ord = Date::THIRD;
    } else if (dy <= 28) {
        ord = Date::FOURTH;
    } else {
        ord = Date::FIFTH;
    }

    uint8_t dom = ce.ordinal(ord, static_cast<Date::Weekday>(wy));

    if (dom != dy) {
        errorf("%s[%d]: (%u(%d,%d)!=%u)!?\n",
            __FILE__, __LINE__, dom, ord, wy, dy);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("dom!=dy");
    }

    if (dy >= (dpm - 6)) {
        dom = ce.ordinal(Date::LAST, static_cast<Date::Weekday>(wy));
        if (dom != dy) {
            errorf("%s[%d]: (%u(%d,%d)!=%u)!?\n",
                __FILE__, __LINE__, dom, Date::LAST, wy, dy);
            ce.show(1, &Platform::instance().error(), 1);
            ++errors;
            DEBUG_FATAL("dom!=dy");
        }
    }

    uint16_t julian = ce.julian();

    if (julian != jn) {
        errorf("%s[%d]: (%u!=%u)!?\n",
            __FILE__, __LINE__, julian, jn);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("julian!=jn");
    }

    Time::Meridiem ampm = (12 > hr) ? Time::AM : Time::PM;
    uint8_t te = (0 == hr) ? hr + 12 : (13 <= hr) ? hr - 12 : hr;
    Time::Meridiem meridiem = ce.oclock(twelve);

    if (meridiem != ampm) {
        errorf("%s[%d]: (%d!=%d)!?\n",
            __FILE__, __LINE__, meridiem, ampm);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("meridiem!=ampm");
    }
    if (twelve != te) {
        errorf("%s[%d]: (%u!=%u)!?\n",
            __FILE__, __LINE__, twelve, te);
        ce.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("twelve!=te");
    }

    seconds_t sd1 = ce.toAtomicSeconds();

    if (testseconds) {
        if (sd1 != ss) {
            errorf("%s[%d]: (%llu!=%llu)&&(%llu!=0)!?\n",
                __FILE__, __LINE__, sd1, ss,
                    (sd1 > ss) ? sd1 - ss : ss - sd1);
            ce.show(1, &Platform::instance().error(), 1);
            ++errors;
            DEBUG_FATAL("sd1!=ss");
        }
    }

    CommonEra ce4;
    ce4.fromAtomicSeconds(sd1);
    seconds_t sd2 = ce4.toAtomicSeconds();

    if (sd2 != sd1) {
        errorf("%s[%d]: (%llu!=%llu)&&(%llu!=0)!?\n",
            __FILE__, __LINE__, sd1, sd2,
            (sd1 > sd2) ? sd1 - sd2 : sd2 - sd1);
        ce.show(1, &Platform::instance().error(), 1);
        ce4.show(1, &Platform::instance().error(), 1);
        ++errors;
        DEBUG_FATAL("sd2!=sd1");
    }

    return errors;
}

static int test1(
    Output& output,
    unsigned long long yr,
    unsigned int mh,
    unsigned int dy,
    unsigned int hr,
    unsigned int me,
    unsigned int sd,
    bool lr,
    int wy,
    int jn,
    seconds_t ss,
    bool testseconds,
    unsigned int dpm
) {
    Print printf(output);
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: constructor\n", __FILE__, __LINE__);

    CommonEra ce(yr, mh, dy, hr, me, sd);

    errors += test0(output,
                ce, yr, mh, dy, hr, me, sd,
                lr, wy, jn, ss, testseconds, dpm);

    printf("%s[%d]: copy constructor\n", __FILE__, __LINE__);

    CommonEra ce1 = ce;

    if (0 != ce.compare(ce1)) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, ce.compare(ce1));
        ce.show(0, &Platform::instance().error());
        ce1.show(0, &Platform::instance().error());
        ++errors;
    }

    errors += test0(output,
                ce1, yr, mh, dy, hr, me, sd,
                lr, wy, jn, ss, testseconds, dpm);

    printf("%s[%d]: assignment\n", __FILE__, __LINE__);

    CommonEra ce2;
    ce2 = ce;

    if (0 != ce.compare(ce2)) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, ce.compare(ce2));
        ce.show(0, &Platform::instance().error());
        ce2.show(0, &Platform::instance().error());
        ++errors;
    }

    errors += test0(output,
                ce2, yr, mh, dy, hr, me, sd,
                lr, wy, jn, ss, testseconds, dpm);

    printf("%s[%d]: initialize\n", __FILE__, __LINE__);

    CommonEra ce3;
    CommonEra newce3(yr, mh, dy, hr, me, sd);
    ce3 = newce3;

    if (0 != ce.compare(ce3)) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, ce.compare(ce3));
        ce.show(0, &Platform::instance().error());
        ce3.show(0, &Platform::instance().error());
        ++errors;
    }

    errors += test0(output,
                ce3, yr, mh, dy, hr, me, sd,
                lr, wy, jn, ss, testseconds, dpm);

    return errors;
}

static AtomicSeconds staticAtomicSeconds;
static CommonEra staticCommonEra;
static Date staticDate;
static DstAlways staticDstAlways;
static DstEu staticDstEu;
static DstGeneric staticDstGeneric;
static DstNever staticDstNever;
static DstUs staticDstUs;
static DstUs1966 staticDstUs1966;
static DstUs1986 staticDstUs1986;
static DstUs2007 staticDstUs2007;
static LeapSeconds staticLeapSeconds;
static LocalTime staticLocalTime;
static Time staticTime;
static TimeStamp staticTimeStamp;
static TimeZone staticTimeZone;

CXXCAPI int unittestCommonEra(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticAtomicSeconds.show();
    ::staticCommonEra.show();
    ::staticDate.show();
    ::staticDstAlways.show();
    ::staticDstEu.show();
    ::staticDstGeneric.show();
    ::staticDstNever.show();
    ::staticDstUs.show();
    ::staticDstUs1966.show();
    ::staticDstUs1986.show();
    ::staticDstUs2007.show();
    ::staticLeapSeconds.show();
    ::staticLocalTime.show();
    ::staticTime.show();
    ::staticTimeStamp.show();
    ::staticTimeZone.show();

    printf("%s[%d]: debug on\n", __FILE__, __LINE__);

    bool prior_debug = CommonEra::debug(true);

    if (false != prior_debug) {
        errorf("%s[%d]: (%d!=%d)!?\n",
            __FILE__, __LINE__, false, prior_debug);
        ++errors;
        DEBUG_FATAL("false!=prior_debug");
    }

    printf("%s[%d]: sanity\n", __FILE__, __LINE__);

    //  Some of this sanity test data was generated using the
    //  GNU cal program.

    errors += test1(
        Platform::instance().output(),
        1, Date::JANUARY, 1, 0, 0, 0,
        false,
        Date::MONDAY,
        1,
        0, true,
        31
    );

    errors += test1(
        Platform::instance().output(),
        1956, Date::JULY, 11, 7, 30, 0,
        true,
        Date::WEDNESDAY,
        193,
        0, false,
        31
    );

    errors += test1(
        Platform::instance().output(),
        1970, Date::JANUARY, 1, 0, 0, 0,
        false,
        Date::THURSDAY,
        1,
        0, false,
        31
    );

    errors += test1(
        Platform::instance().output(),
        1984, Date::FEBRUARY, 25, 14, 00, 00,
        true,
        Date::SATURDAY,
        56,
        0, false,
        29
    );

    errors += test1(
        Platform::instance().output(),
        2005, Date::FEBRUARY, 28, 11, 57, 45,
        false,
        Date::MONDAY,
        59,
        0, false,
        28
    );

    errors += test1(
        Platform::instance().output(),
        2038, Date::JANUARY, 19, 3, 14, 7,
        false,
        Date::TUESDAY,
        19,
        0, false,
        31
    );

    errors += test1(
        Platform::instance().output(),
        9996, Date::DECEMBER, 31, 23, 59, 59,
        true,
        Date::TUESDAY,
        366,
        0, false,
        31
    );

    errors += test1(
        Platform::instance().output(),
        9999, Date::DECEMBER, 31, 23, 59, 59,
        false,
        Date::FRIDAY,
        365,
        0, false,
        31
    );

    errors += test1(
        Platform::instance().output(),
        10000, Date::JANUARY, 1, 0, 0, 0,
        true,
        Date::SATURDAY,
        1,
        0, false,
        31
    );

    errors += test1(
        Platform::instance().output(),
        584554049254ULL, Date::NOVEMBER, 9, 7, 0, 15,
        false,
        Date::MONDAY,
        313,
        unsignedintmaxof(seconds_t), true,
        30
    );

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: Leap Seconds\n", __FILE__, __LINE__);

    //  The ITU-R TF.460 states that there will never be more
    //  than a single leap second added or substracted at a time,
    //  that that there is a strong preference to adjust leap
    //  seconds on the last day of June or December. If either of
    //  these rules are violated, this unit test may fail.

    LeapSeconds& lsrule = Platform::instance().getLeapSeconds();
    lsrule.show();

    for (size_t ii = 0; ii < countof(leapseconddate); ++ii) {
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month - 1,
                leapseconddate[ii].day - 1,
                23 - 1, 59 - 1, 60 - 1, 0
            );
            errors += testls(lsd, false, static_cast<int>(ii));
        }
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month,
                leapseconddate[ii].day - 1,
                23 - 1, 59 - 1, 60 - 1, 0
            );
            errors += testls(lsd, false, static_cast<int>(ii));
        }
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month,
                leapseconddate[ii].day,
                23 - 1, 59 - 1, 60 - 1, 0
            );
            errors += testls(lsd, false, static_cast<int>(ii));
        }
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month,
                leapseconddate[ii].day,
                23, 59 - 1, 60 - 1, 0
            );
            errors += testls(lsd, false, static_cast<int>(ii));
        }
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month,
                leapseconddate[ii].day,
                23, 59, 60 - 1, 999999999
            );
            errors += testls(lsd, false, static_cast<int>(ii));
        }
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month,
                leapseconddate[ii].day,
                23, 59, 60, 0
            );
            errors += testls(lsd, true, static_cast<int>(ii + 1));
        }
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month,
                leapseconddate[ii].day,
                23, 59, 60, 1
            );
            errors += testls(lsd, true, static_cast<int>(ii + 1));
        }
        {
            CommonEra lsd(
                leapseconddate[ii].year,
                leapseconddate[ii].month,
                leapseconddate[ii].day,
                23, 59, 60, 999999999
            );
            errors += testls(lsd, true, static_cast<int>(ii + 1));
        }
        //
        //  month+1 or day+1 may not yield a valid date.
        //  year+1 may yield a second valid leap second.
        //
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: nanoseconds\n", __FILE__, __LINE__);

    uint32_t mn = unsignedintmaxof(uint32_t);
    CommonEra ce1(1, 1, 1, 0, 0, 1, mn);
    uint8_t ss = ce1.getSecond();
    uint32_t nn = ce1.getNanosecond();
    uint8_t es = 1 + (mn / Constant::ns_per_s);
    uint32_t en = mn % Constant::ns_per_s;

    if (ss != es) {
        errorf("%s[%d]: (%u!=%u)!?\n",
            __FILE__, __LINE__, ss, es);
        ++errors;
        DEBUG_FATAL("ss!=es");
    }
    if (nn != en) {
        errorf("%s[%d]: (%lu!=%lu)!?\n",
            __FILE__, __LINE__, nn, en);
        ++errors;
        DEBUG_FATAL("nn!=en");
    }

    CommonEra ce2;

    ce2.fromAtomicSeconds(1, mn);
    ss = ce2.getSecond();
    nn = ce2.getNanosecond();
    if (ss != es) {
        errorf("%s[%d]: (%u!=%u)!?\n",
            __FILE__, __LINE__, ss, es);
        ++errors;
        DEBUG_FATAL("ss!=es");
    }
    if (nn != en) {
        errorf("%s[%d]: (%lu!=%lu)!?\n",
            __FILE__, __LINE__, nn, en);
        ++errors;
        DEBUG_FATAL("nn!=en");
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: toString\n", __FILE__, __LINE__);

    CommonEra maximum(0);
    char string[sizeof(CommonEra::String) + 1];
    size_t size;
    size_t actual;
    size_t length;

    maximum.fromAtomicSeconds(unsignedintmaxof(uint64_t), 999999999);

    maximum.show();

    for (size = 0; sizeof(string) >= size; ++size) {
        string[0] = '\0';
        actual = maximum.toString(string, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(string);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, string,
                size, length, actual,
                sizeof(CommonEra::String), sizeof(string));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(CommonEra::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(CommonEra::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(CommonEra::String)");
            }
            if ((*(string + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    DstUs dst;
    LocalTime localdst(signedintmaxof(int32_t), dst);
    char localdststring[sizeof(LocalTime::String) + 1];

    dst.show();

    localdst.fromSeconds(unsignedintmaxof(uint64_t), 999999999);

    localdst.show();

    for (size = 0; sizeof(localdststring) >= size; ++size) {
        localdststring[0] = '\0';
        actual = localdst.toString(localdststring, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(localdststring);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, localdststring,
                size, length, actual,
                sizeof(LocalTime::String), sizeof(localdststring));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(LocalTime::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(LocalTime::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(LocalTime::String)");
            }
            if ((*(localdststring + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    DstAlways always;
    LocalTime localalways(signedintmaxof(int32_t), always);
    char localalwaysstring[sizeof(LocalTime::String) + 1];

    always.show();

    localalways.fromSeconds(unsignedintmaxof(uint64_t), 999999999);

    localalways.show();

    for (size = 0; sizeof(localalwaysstring) >= size; ++size) {
        localalwaysstring[0] = '\0';
        actual = localalways.toString(localalwaysstring, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(localalwaysstring);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, localalwaysstring,
                size, length, actual,
                sizeof(LocalTime::String), sizeof(localalwaysstring));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(LocalTime::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(LocalTime::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(LocalTime::String)");
            }
            if ((*(localalwaysstring + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    DstNever never;
    LocalTime localnever(signedintminof(int32_t), never);
    char localneverstring[sizeof(LocalTime::String) + 1];

    never.show();

    localnever.fromSeconds(unsignedintminof(uint64_t), 0);

    localnever.show();

    for (size = 0; sizeof(localneverstring) >= size; ++size) {
        localneverstring[0] = '\0';
        actual = localnever.toString(localneverstring, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(localneverstring);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, localneverstring,
                size, length, actual,
                sizeof(LocalTime::String), sizeof(localneverstring));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(LocalTime::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(LocalTime::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(LocalTime::String)");
            }
            if ((*(localneverstring + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    DstEu eu;
    LocalTime localeu(signedintmaxof(int32_t), eu);
    char localeustring[sizeof(LocalTime::String) + 1];

    eu.show();

    localeu.fromSeconds(unsignedintmaxof(uint64_t), 999999999);

    localeu.show();

    for (size = 0; sizeof(localeustring) >= size; ++size) {
        localeustring[0] = '\0';
        actual = localeu.toString(localeustring, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(localeustring);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, localeustring,
                size, length, actual,
                sizeof(LocalTime::String), sizeof(localeustring));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(LocalTime::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(LocalTime::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(LocalTime::String)");
            }
            if ((*(localeustring + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    DstUs1966 us1966;
    LocalTime localus1966(signedintmaxof(int32_t), us1966);
    char localus1966string[sizeof(LocalTime::String) + 1];

    us1966.show();

    localus1966.fromSeconds(unsignedintmaxof(uint64_t), 999999999);

    localus1966.show();

    for (size = 0; sizeof(localus1966string) >= size; ++size) {
        localus1966string[0] = '\0';
        actual = localus1966.toString(localus1966string, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(localus1966string);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, localus1966string,
                size, length, actual,
                sizeof(LocalTime::String), sizeof(localus1966string));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(LocalTime::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(LocalTime::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(LocalTime::String)");
            }
            if ((*(localus1966string + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    DstUs1986 us1986;
    LocalTime localus1986(signedintmaxof(int32_t), us1986);
    char localus1986string[sizeof(LocalTime::String) + 1];

    us1986.show();

    localus1986.fromSeconds(unsignedintmaxof(uint64_t), 999999999);

    localus1986.show();

    for (size = 0; sizeof(localus1986string) >= size; ++size) {
        localus1986string[0] = '\0';
        actual = localus1986.toString(localus1986string, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(localus1986string);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, localus1986string,
                size, length, actual,
                sizeof(LocalTime::String), sizeof(localus1986string));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(LocalTime::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(LocalTime::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(LocalTime::String)");
            }
            if ((*(localus1986string + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    DstUs2007 us2007;
    LocalTime localus2007(signedintmaxof(int32_t), us2007);
    char localus2007string[sizeof(LocalTime::String) + 1];

    us2007.show();

    localus2007.fromSeconds(unsignedintmaxof(uint64_t), 999999999);

    localus2007.show();

    for (size = 0; sizeof(localus2007string) >= size; ++size) {
        localus2007string[0] = '\0';
        actual = localus2007.toString(localus2007string, size);
        if (0 == size) {
            if (actual > size) {
                errorf("%s[%d]: (%lu>%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual!=size");
            }
        } else {
            length = std::strlen(localus2007string);
            printf("%s[%d]: \"%s\"[%lu:%lu:%lu:%lu:%lu]\n",
                __FILE__, __LINE__, localus2007string,
                size, length, actual,
                sizeof(LocalTime::String), sizeof(localus2007string));
            if (actual >= size) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, size);
                ++errors;
                DEBUG_FATAL("actual>=size");
            }
            if (actual != length) {
                errorf("%s[%d]: (%lu!=%lu)!?\n",
                    __FILE__, __LINE__, actual, length);
                ++errors;
                DEBUG_FATAL("actual!=length");
            }
            if (actual >= sizeof(LocalTime::String)) {
                errorf("%s[%d]: (%lu>=%lu)!?\n",
                    __FILE__, __LINE__, actual, sizeof(LocalTime::String));
                ++errors;
                DEBUG_FATAL("actual>=sizeof(LocalTime::String)");
            }
            if ((*(localus2007string + actual) != '\0')) {
                errorf("%s[%d]: (![%lu]!=\0')!?\n",
                    __FILE__, __LINE__, actual);
                ++errors;
                DEBUG_FATAL("actual>size");
            }
        }
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: compare\n", __FILE__, __LINE__);

    for (unsigned int cy1 = 1; cy1 <= 3; ++cy1) {
    for (unsigned int cm1 = 1; cm1 <= 3; ++cm1) {
    for (unsigned int cd1 = 1; cm1 <= 3; ++cm1) {
    for (unsigned int ch1 = 1; ch1 <= 3; ++ch1) {
    for (unsigned int ct1 = 1; ct1 <= 3; ++ct1) {
    for (unsigned int cs1 = 1; cs1 <= 3; ++cs1) {
    for (unsigned int cn1 = 1; cn1 <= 3; ++cn1) {
        for (unsigned int cy2 = 1; cy2 <= 3; ++cy2) {
        for (unsigned int cm2 = 1; cm2 <= 3; ++cm2) {
        for (unsigned int cd2 = 1; cm2 <= 3; ++cm2) {
        for (unsigned int ch2 = 1; ch2 <= 3; ++ch2) {
        for (unsigned int ct2 = 1; ct2 <= 3; ++ct2) {
        for (unsigned int cs2 = 1; cs2 <= 3; ++cs2) {
        for (unsigned int cn2 = 1; cn2 <= 3; ++cn2) {
            CommonEra cethis1(cy1, cm1, cd1, ch1, ct1, cs1, cn1);
            CommonEra cethat2(cy2, cm2, cd2, ch2, ct2, cs2, cn2);
            int mine = 0;
            if (cy1 < cy2) mine = -1;
            else if (cy1 > cy2) mine = 1;
            else if (cm1 < cm2) mine = -1;
            else if (cm1 > cm2) mine = 1;
            else if (cd1 < cd2) mine = -1;
            else if (cd1 > cd2) mine = 1;
            else if (ch1 < ch2) mine = -1;
            else if (ch1 > ch2) mine = 1;
            else if (ct1 < ct2) mine = -1;
            else if (ct1 > ct2) mine = 1;
            else if (cs1 < cs2) mine = -1;
            else if (cs1 > cs2) mine = 1;
            else if (cn1 < cn2) mine = -1;
            else if (cn1 > cn2) mine = 1;
            int yours = cethis1.compare(cethat2);
            if (mine != yours) {
                errorf("%s[%d]: (%d!=%d)!\n", __FILE__, __LINE__, mine, yours);
                ++errors;
                DEBUG_FATAL("mine!=yours");
            }
        } } } } } } }
    } } } } } } }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: debug off\n", __FILE__, __LINE__);

    prior_debug = CommonEra::debug(false);
    if (true != prior_debug) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, prior_debug);
        ++errors;
        DEBUG_FATAL("true!=prior_debug");
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: Months\n", __FILE__, __LINE__);

    Date date;

    printf("%2d \"%s\"\n",
        0, date.monthToString(static_cast<Date::Month>(0)));
    printf("%2d \"%s\"\n",
        Date::JANUARY, date.monthToString(Date::JANUARY));
    printf("%2d \"%s\"\n",
        Date::FEBRUARY, date.monthToString(Date::FEBRUARY));
    printf("%2d \"%s\"\n",
        Date::MARCH, date.monthToString(Date::MARCH));
    printf("%2d \"%s\"\n",
        Date::APRIL, date.monthToString(Date::APRIL));
    printf("%2d \"%s\"\n",
        Date::MAY, date.monthToString(Date::MAY));
    printf("%2d \"%s\"\n",
        Date::JUNE, date.monthToString(Date::JUNE));
    printf("%2d \"%s\"\n",
        Date::JULY, date.monthToString(Date::JULY));
    printf("%2d \"%s\"\n",
        Date::AUGUST, date.monthToString(Date::AUGUST));
    printf("%2d \"%s\"\n",
        Date::SEPTEMBER,date.monthToString(Date::SEPTEMBER));
    printf("%2d \"%s\"\n",
        Date::OCTOBER, date.monthToString(Date::OCTOBER));
    printf("%2d \"%s\"\n",
        Date::NOVEMBER, date.monthToString(Date::NOVEMBER));
    printf("%2d \"%s\"\n",
        Date::DECEMBER, date.monthToString(Date::DECEMBER));

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: Weekdays\n", __FILE__, __LINE__);

    printf("%1d \"%s\"\n",
        0, date.weekdayToString(static_cast<Date::Weekday>(0)));
    printf("%1d \"%s\"\n",
        Date::MONDAY, date.weekdayToString(Date::MONDAY));
    printf("%1d \"%s\"\n",
        Date::TUESDAY, date.weekdayToString(Date::TUESDAY));
    printf("%1d \"%s\"\n",
        Date::WEDNESDAY,date.weekdayToString(Date::WEDNESDAY));
    printf("%1d \"%s\"\n",
        Date::THURSDAY, date.weekdayToString(Date::THURSDAY));
    printf("%1d \"%s\"\n",
        Date::FRIDAY, date.weekdayToString(Date::FRIDAY));
    printf("%1d \"%s\"\n",
        Date::SATURDAY, date.weekdayToString(Date::SATURDAY));
    printf("%1d \"%s\"\n",
        Date::SUNDAY, date.weekdayToString(Date::SUNDAY));

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: Ordinals\n", __FILE__, __LINE__);

    printf("%1d \"%s\"\n",
        0, date.ordinalToString(static_cast<Date::Ordinal>(0)));
    printf("%1d \"%s\"\n",
        Date::FIRST, date.ordinalToString(Date::FIRST));
    printf("%1d \"%s\"\n",
        Date::SECOND, date.ordinalToString(Date::SECOND));
    printf("%1d \"%s\"\n",
        Date::THIRD, date.ordinalToString(Date::THIRD));
    printf("%1d \"%s\"\n",
        Date::FOURTH, date.ordinalToString(Date::FOURTH));
    printf("%1d \"%s\"\n",
        Date::FIFTH, date.ordinalToString(Date::FIFTH));
    printf("%1d \"%s\"\n",
        Date::LAST, date.ordinalToString(Date::LAST));

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: Meridiems\n", __FILE__, __LINE__);

    Time time;

    printf("%1d \"%s\"\n",
        Time::AM, time.meridiemToString(Time::AM));
    printf("%1d \"%s\"\n",
        Time::PM, time.meridiemToString(Time::PM));
    printf("%1d \"%s\"\n",
        2, time.meridiemToString(static_cast<Time::Meridiem>(2)));

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: Time Zones\n", __FILE__, __LINE__);

    TimeZone zone;

    zone.show();

    printf("%9s %-16s %-16s %10s %s %10s %s\n",
        "OFFSET", "MILITARY", "CIVILIAN", "ORIGINAL", "?", "NORMALIZED", "?");

    for (int32_t oo = (-(24 * 3) * 3600); ((24 * 3) * 3600) >= oo; oo += (15 * 60)) {
        int aa = (0 > oo) ? -oo : oo;
        char sign = (0 > oo) ? '-' : '+';
        int hh = aa / 3600;
        int mm = (aa / 60) - (hh * 60);
        int ss = aa - (hh * 3600) - (mm * 60);
        int normalizedzone = zone.normalize(oo);
        bool normalizedzoneisvalid = zone.isValid(normalizedzone);
        printf("%c%02d:%02d:%02d %-16s %-16s %10d %d %10d %d\n",
            sign, hh, mm, ss,
            zone.milspec(oo),
            zone.civilian(oo),
            oo, zone.isValid(oo),
            normalizedzone, normalizedzoneisvalid);
        if (!normalizedzoneisvalid) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, true, normalizedzoneisvalid);
            ++errors;
        }
    }

    zone.show();

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: Time Stamps\n", __FILE__, __LINE__);

    TimeStamp timestamp;
    CommonEra epochnow;
    CommonEra rightnow;
    rightnow.fromNow();
    CommonEra maximumnow;
    maximumnow.fromAtomicSeconds(unsignedintmaxof(uint64_t), 999999999);
    LocalTime epochhere;
    LocalTime righthere;
    righthere.fromNow();
    LocalTime maximumhere;
    maximumhere.fromSeconds(unsignedintmaxof(uint64_t), 999999999);
    const char* label;

    timestamp.show();

    printf("%s[%d]: iso8601\n", __FILE__, __LINE__);

    label = "iso8601";

    printf("%s %s\n", label, timestamp.iso8601(epochnow));
    printf("%s %s\n", label, timestamp.iso8601(epochhere));
    printf("%s %s\n", label, timestamp.iso8601());
    printf("%s %s\n", label, timestamp.iso8601(rightnow));
    printf("%s %s\n", label, timestamp.iso8601(righthere));
    printf("%s %s\n", label, timestamp.iso8601(maximumnow));
    printf("%s %s\n", label, timestamp.iso8601(maximumhere));

    printf("%s[%d]: milspec\n", __FILE__, __LINE__);

    label = "milspec";

    printf("%s %s\n", label, timestamp.milspec(epochnow));
    printf("%s %s\n", label, timestamp.milspec(epochhere));
    printf("%s %s\n", label, timestamp.milspec());
    printf("%s %s\n", label, timestamp.milspec(rightnow));
    printf("%s %s\n", label, timestamp.milspec(righthere));
    printf("%s %s\n", label, timestamp.milspec(maximumnow));
    printf("%s %s\n", label, timestamp.milspec(maximumhere));

    printf("%s[%d]: civilian\n", __FILE__, __LINE__);

    label = "civilian";

    printf("%s %s\n", label, timestamp.civilian(epochnow));
    printf("%s %s\n", label, timestamp.civilian(epochhere));
    printf("%s %s\n", label, timestamp.civilian());
    printf("%s %s\n", label, timestamp.civilian(rightnow));
    printf("%s %s\n", label, timestamp.civilian(righthere));
    printf("%s %s\n", label, timestamp.civilian(maximumnow));
    printf("%s %s\n", label, timestamp.civilian(maximumhere));

    printf("%s[%d]: log\n", __FILE__, __LINE__);

    label = "log";

    printf("%s %s\n", label, timestamp.log(epochnow));
    printf("%s %s\n", label, timestamp.log(epochhere));
    printf("%s %s\n", label, timestamp.log());
    printf("%s %s\n", label, timestamp.log(rightnow));
    printf("%s %s\n", label, timestamp.log(righthere));
    printf("%s %s\n", label, timestamp.log(maximumnow));
    printf("%s %s\n", label, timestamp.log(maximumhere));

    printf("%s[%d]: formal\n", __FILE__, __LINE__);

    label = "formal";

    printf("%s %s\n", label, timestamp.formal(epochnow));
    printf("%s %s\n", label, timestamp.formal(epochhere));
    printf("%s %s\n", label, timestamp.formal());
    printf("%s %s\n", label, timestamp.formal(rightnow));
    printf("%s %s\n", label, timestamp.formal(righthere));
    printf("%s %s\n", label, timestamp.formal(maximumnow));
    printf("%s %s\n", label, timestamp.formal(maximumhere));

    printf("%s[%d]: high precision\n", __FILE__, __LINE__);

    label = "highprecision";

    printf("%s %s\n", label, timestamp.highprecision(epochnow));
    printf("%s %s\n", label, timestamp.highprecision(epochhere));
    printf("%s %s\n", label, timestamp.highprecision());
    printf("%s %s\n", label, timestamp.highprecision(rightnow));
    printf("%s %s\n", label, timestamp.highprecision(righthere));
    printf("%s %s\n", label, timestamp.highprecision(maximumnow));
    printf("%s %s\n", label, timestamp.highprecision(maximumhere));

    timestamp.show();

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int days;
    bool leaping;
    int weekday;
    int julian;
    seconds_t seconds;
    Output null;

    printf("%s[%d]: Daylight Saving Time\n", __FILE__, __LINE__);

    DstAlways dsttestalways;
    DstNever dsttestnever;
    DstEu dsttesteu;
    DstUs dsttestus;
    DstUs1966 dsttestus1966;
    DstUs1986 dsttestus1986;
    DstUs2007 dsttestus2007;
    DstGeneric::Event dsttestbegins = {
        Date::LAST,
        Date::SUNDAY,
        Date::OCTOBER,
        1
    };
    DstGeneric::Event dsttestends = {
        Date::LAST,
        Date::SUNDAY,
        Date::MARCH,
        1
    };
    DstGeneric dsttestau(dsttestbegins, dsttestends);

    Date dsttestdate;

    printf("%29s %2s %2s %2s %2s %2s %2s %2s %2s\n",
        "DATE", "NV", "EU", "US", "66", "86", "07", "AU", "AL");

    unsigned int dsttestyears[] = { 1956, 1976, 1996, 2016 };
    for (unsigned int ii = 0; ii < countof(dsttestyears); ++ii) {
        unsigned int yy = dsttestyears[ii];
        for (unsigned int mm = Date::JANUARY; mm <= Date::DECEMBER; ++mm) {
            unsigned int dds = dsttestdate.cardinal(yy,
                                static_cast<Date::Month>(mm));
            for (unsigned int dd = 1; dd <= dds; ++dd) {
                CommonEra ce(yy, mm, dd, 23, 59, 59, 999999999);
                printf("%04u-%02u-%02uT%02u:%02u:%02u.%09u"
                       " %2d %2d %2d %2d %2d %2d %2d %2d\n",
                    yy, mm, dd, 23, 59, 59, 999999999,
                    dsttestnever(ce),
                    dsttesteu(ce),
                    dsttestus(ce),
                    dsttestus1966(ce),
                    dsttestus1986(ce),
                    dsttestus2007(ce),
                    dsttestau(ce),
                    dsttestalways(ce));
            }
        }
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: seconds\n", __FILE__, __LINE__);

    weekday = static_cast<int>(Date::MONDAY);
    seconds = 0;

    year = 1;
    leaping = false;
    julian = 1;
    month = 1;
    day = 1;
    days = 31;
    for (hour = 0; 23 >= hour; ++hour) {
        printf("%s[%d]: %02u:%02u:%02u %5u\n",
            __FILE__, __LINE__, hour, 0, 0, seconds);
        for (minute = 0; 59 >= minute; ++minute) {
            for (second = 0; 59 >= second; ++second) {
                errors += test1(
                        null,
                        year, month, day,
                        hour, minute, second,
                        leaping,
                        weekday,
                        julian,
                        seconds, true,
                        days
                );
                ++seconds;
            }
        }
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: months\n", __FILE__, __LINE__);

    //  An effort is made not to use the very code
    //  we are trying to unit test. However, since both
    //  implementations were done by the same developer, it
    //  is possible, even likely, that the same bugs crept into
    //  both. On small teams, incestuousness is unavoidable.

    weekday = static_cast<int>(Date::MONDAY);
    seconds = 0;

    for (year = 1; 9999 >= year; ++year) {
        if (0 == (year % 100)) {
            printf("%s[%d]: %04u\n", __FILE__, __LINE__, year);
        }
        if (0 == (year % 100)) {
            leaping= (0 == (year % 400));
        } else {
            leaping = (0 == (year % 4));
        }
        julian = 1;
        for (month = 1; 12 >= month; ++month) {
            switch (month) {
            case 4:
            case 6:
            case 9:
            case 11:
                days = 30;
                break;
            case 2:
                days = leaping ? 29 : 28;
                break;
            default:
                days = 31;
                break;
            }
            day = 1;
            hour = 0;
            minute = 0;
            second = 0;
            errors += test1(
                            null,
                            year, month, day,
                            hour, minute, second,
                            leaping,
                            weekday,
                            julian,
                            seconds, true,
                            days
                    );
            seconds += days * 24 * 60 * 60;
            weekday = ((weekday + days - 1) % 7) + 1;
            julian += days;
        }
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: days\n", __FILE__, __LINE__);

    weekday = static_cast<int>(Date::MONDAY);
    seconds = 0;

    for (year = 1; 9999 >= year; ++year) {
        if (0 == (year % 100)) {
            printf("%s[%d]: %04u\n", __FILE__, __LINE__, year);
        }
        if (0 == (year % 100)) {
            leaping= (0 == (year % 400));
        } else {
            leaping = (0 == (year % 4));
        }
        julian = 1;
        for (month = 1; 12 >= month; ++month) {
            switch (month) {
            case 4:
            case 6:
            case 9:
            case 11:
                days = 30;
                break;
            case 2:
                days = leaping ? 29 : 28;
                break;
            default:
                days = 31;
                break;
            }
            for (day = 1; days >= day; ++day) {
                hour = 0;
                minute = 0;
                second = 0;
                errors += test1(
                        null,
                        year, month, day,
                        hour, minute, second,
                        leaping,
                        weekday,
                        julian,
                        seconds, true,
                        days
                        );
                seconds += 24 * 60 * 60;
                weekday = (weekday % 7) + 1;
                ++julian;
            }
        }
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: hours\n", __FILE__, __LINE__);

    weekday = static_cast<int>(Date::MONDAY);
    seconds = 0;

    for (year = 1; 9999 >= year; ++year) {
        if (0 == (year % 100)) {
            printf("%s[%d]: %04u\n", __FILE__, __LINE__, year);
        }
        if (0 == (year % 100)) {
            leaping= (0 == (year % 400));
        } else {
            leaping = (0 == (year % 4));
        }
        julian = 1;
        for (month = 1; 12 >= month; ++month) {
            switch (month) {
            case 4:
            case 6:
            case 9:
            case 11:
                days = 30;
                break;
            case 2:
                days = leaping ? 29 : 28;
                break;
            default:
                days = 31;
                break;
            }
            for (day = 1; days >= day; ++day) {
                for (hour = 0; 23 >= hour; ++hour) {
                    minute = 0;
                    second = 0;
                    errors += test1(
                            null,
                            year, month, day,
                            hour, minute, second,
                            leaping,
                            weekday,
                            julian,
                            seconds, true,
                            days
                    );
                    seconds += 60 * 60;
                }
                weekday = (weekday % 7) + 1;
                ++julian;
            }
        }
    }

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    printf("%s[%d]: years\n", __FILE__, __LINE__);

    weekday = static_cast<int>(Date::MONDAY);
    seconds = 0;

    bool interesting;

    for (year = 1; 400 >= year; ++year) {
        interesting = (1 == year) ||
                      (4 == year) ||
                      (100 == year) ||
                      (400 == year);
        if (interesting) {
            printf("%s[%d]: %04u\n", __FILE__, __LINE__, year);
        }
        if (0 == (year % 100)) {
            leaping= (0 == (year % 400));
        } else {
            leaping = (0 == (year % 4));
        }
        julian = 1;
        for (month = 1; 12 >= month; ++month) {
            switch (month) {
            case 4:
            case 6:
            case 9:
            case 11:
                days = 30;
                break;
            case 2:
                days = leaping ? 29 : 28;
                break;
            default:
                days = 31;
                break;
            }
            for (day = 1; days >= day; ++day) {
                for (hour = 0; 23 >= hour; ++hour) {
                    for (minute = 0; 59 >= minute; ++minute) {
                        for (second = 0; 59 >= second; ++second) {
                            if (interesting) {
                                errors += test1(
                                        null,
                                        year, month, day,
                                        hour, minute, second,
                                        leaping,
                                        weekday,
                                        julian,
                                        seconds, true,
                                        days
                                );
                            }
                            ++seconds;
                        }
                    }
                }
                weekday = (weekday % 7) + 1;
                ++julian;
            }
        }
    }

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__, errors);

    return errors;
}
