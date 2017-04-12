/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the CommonEra class.
 *
 *  @see    CommonEra
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#if 0
#define DESPERADO_HAS_DEBUGGING
#endif


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/LeapSeconds.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Ticks.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  An quadricentury is a block of 400 years which has a constant number
//  of days, due of the way leap years are calculated.
//
static const seconds_t s_per_d = Constant::s_per_d;
static const uint32_t y_per_quadricentury = 400;
static const uint32_t d_per_quadricentury = (303 * 365) + (97 * 366);
static const seconds_t s_per_quadricentury = s_per_d * d_per_quadricentury;


//
//  A century is a block of 100 years which has a constant number of days.
//  There are only three centuries in a quadricentury because the last
//  block of 100 years has one more day in it, due to the way leap years
//  are calculated.
//
static const uint32_t y_per_century = 100;
static const uint32_t d_per_century = (76 * 365) + (24 * 366);
static const seconds_t s_per_century = s_per_d * d_per_century;


//
//  A quadriyear is a block of four years which has a constant number of
//  days. There are only twenty-three quadriyears per century because the
//  last block of four years may have a different number of days, due to
//  the way leap years are calculated.
//
static const uint32_t y_per_quadriyear = 4;
static const uint32_t d_per_quadriyear = (3 * 365) + (1 * 366);
static const seconds_t s_per_quadriyear = s_per_d * d_per_quadriyear;


//
//  The number of second in a year depends on whether it is a leap
//  year or not.
//
static const uint32_t s_per_year[2] = {
    365 * Constant::s_per_d,        // non-leap years
    366 * Constant::s_per_d         // leap-years
};


//
//  The number of seconds in a month depends on whether it is in
//  a leap year, and where it is relative to February.
//
static const uint32_t s_per_month[2][Constant::months_per_year] = {
    {                               // non-leap years
        31 * Constant::s_per_d,     // January
        28 * Constant::s_per_d,     // February
        31 * Constant::s_per_d,     // March
        30 * Constant::s_per_d,     // April
        31 * Constant::s_per_d,     // May
        30 * Constant::s_per_d,     // June
        31 * Constant::s_per_d,     // July
        31 * Constant::s_per_d,     // August
        30 * Constant::s_per_d,     // September
        31 * Constant::s_per_d,     // October
        30 * Constant::s_per_d,     // November
        31 * Constant::s_per_d      // December
    },
    {                               // leap years
        31 * Constant::s_per_d,     // January
        29 * Constant::s_per_d,     // February
        31 * Constant::s_per_d,     // March
        30 * Constant::s_per_d,     // April
        31 * Constant::s_per_d,     // May
        30 * Constant::s_per_d,     // June
        31 * Constant::s_per_d,     // July
        31 * Constant::s_per_d,     // August
        30 * Constant::s_per_d,     // September
        31 * Constant::s_per_d,     // October
        30 * Constant::s_per_d,     // November
        31 * Constant::s_per_d      // December
    }
};


static bool d_bug = false;


//
//  Turn debugging on and off.
//
bool CommonEra::debug(bool now) {
    bool was = d_bug;
    d_bug = now;
    return was;
}


//
//  Constructor.
//
CommonEra::CommonEra() :
    DateTime()
{
}


//
//  Constructor.
//
CommonEra::CommonEra(
    uint64_t yr,
    uint8_t mh,
    uint8_t dy,
    uint8_t hr,
    uint8_t me,
    uint8_t sd,
    uint32_t nd
) :
    DateTime(yr, mh, dy, hr, me, sd, nd)
{
}


//
//  Destructor.
//
CommonEra::~CommonEra() {
}


//
//  Convert this object to a string.
//
size_t CommonEra::toString(String string, size_t size) const {
    DateTime::String datetimebuffer;
    this->DateTime::toString(datetimebuffer);
    int octets = snprintf(string, size, "%sZ", datetimebuffer);
    return ((0 <= octets) && (octets < static_cast<int>(size)))
        ? static_cast<size_t>(octets)
        : (0 < size) ? (size - 1) : 0;
}


//
//  Set this common era object from epoch seconds.
//
void CommonEra::fromAtomicSeconds(uint64_t ad, uint32_t nd) {

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: fromAtomicSeconds(%llu,%lu)\n",
        __FILE__, __LINE__, ad, nd));

    //  Nanoseconds.

    uint32_t na = 0;

    ad += nd / Constant::ns_per_s;
    na = nd % Constant::ns_per_s;

    this->setNanosecond(na);

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu nanoseconds=%lu\n",
        __FILE__, __LINE__, ad, this->getNanosecond()));

    //  Year.

    uint64_t ye = 1;

    uint64_t quadricenturies = ad /s_per_quadricentury;
    ad -= quadricenturies * s_per_quadricentury;
    ye += quadricenturies * y_per_quadricentury;

    DEBUG_PRINTF_IF(d_bug,
        ("%s[%d]: ad=%llu quadricenturies=%llu year=%llu\n",
        __FILE__, __LINE__, ad, quadricenturies, ye));

    //  You can have at most three centuries; four centuries
    //  have one too few days compared to a quadricentury, since
    //  a quadricentury includes an additional leap day.

    uint32_t centuries = ad /s_per_century;
    if (3 < centuries) {
        centuries = 3;
    }
    ad -= centuries * s_per_century;
    ye += centuries * y_per_century;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu centuries=%lu year=%llu\n",
        __FILE__, __LINE__, ad, centuries, ye));

    //  You can have at most twenty-three quadriyears; twenty-four
    //  quadriyears will not have a consistent number of days.

    uint32_t quadriyears = ad / s_per_quadriyear;
    if (23 < quadriyears) {
        quadriyears = 23;
    }
    ad -= quadriyears * s_per_quadriyear;
    ye += quadriyears * y_per_quadriyear;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu quadriyears=%lu year=%llu\n",
        __FILE__, __LINE__, ad, quadriyears, ye));

    uint32_t sc;
    bool ly;

    do {
        ly = this->getDate().isLeapYear(ye);
        sc = s_per_year[ly];
        if (sc > ad) {
            break;
        }
        ++ye;
        ad -= sc;
    } while (true);

    this->setYear(ye);

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu year=%llu\n",
        __FILE__, __LINE__, ad, this->getYear()));

    //  Month.

    uint8_t mo = 1;

    ly = this->getDate().isLeapYear(ye);
    do {
        sc = s_per_month[ly][mo - 1];
        if (sc > ad) {
            break;
        }
        ++mo;
        ad -= sc;
    } while (true);

    this->setMonth(mo);

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu month=%u\n",
        __FILE__, __LINE__, ad, this->getMonth()));

    //  Day.

    uint8_t da = 1;

    da += ad / Constant::s_per_d;
    ad %= Constant::s_per_d;

    this->setDay(da);

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu day=%u\n",
        __FILE__, __LINE__, ad, this->getDay()));

    //  Hour.

    uint8_t ho;

    ho = ad / Constant::s_per_h;
    ad %= Constant::s_per_h;

    this->setHour(ho);

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu hour=%u\n",
        __FILE__, __LINE__, ad, this->getHour()));

    //  Minute.

    uint8_t mi;

    mi = ad / Constant::s_per_min;
    ad = ad % Constant::s_per_min;

    this->setMinute(mi);

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu minute=%u\n",
        __FILE__, __LINE__, ad, this->getMinute()));

    //  Second.

    this->setSecond(ad);

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu second=%u\n",
        __FILE__, __LINE__, ad, this->getSecond()));
}


//
//  Set this common era object from epoch seconds.
//
void CommonEra::fromSeconds(seconds_t sd, uint32_t nd) {
    LeapSeconds& lsrule = Platform::instance().getLeapSeconds();
    int ld = 0;
    const CommonEra* ce = lsrule.find(sd, ld);
    if (0 != ce) {
        *this = *ce;
        this->setNanosecond(nd);
    } else {
        sd -= ld;
        this->fromAtomicSeconds(sd, nd);
    }
}


//
//  Get this common era object in non-atomic seconds plus nanoseconds.
//
seconds_t CommonEra::toAtomicSeconds(uint32_t& nd) const {
    seconds_t ad = 0;

    const uint64_t ye = this->getYear();
    const uint8_t mo = this->getMonth();
    const uint8_t da = this->getDay();
    const uint8_t ho = this->getHour();
    const uint8_t mi = this->getMinute();
    const uint8_t se = this->getSecond();
    uint32_t na = this->getNanosecond();

    DEBUG_PRINTF_IF(d_bug,
        ("%s[%d]: toAtomicSeconds(): %llu-%02u-%02uT%02u:%02u:%02u.%09u\n",
        __FILE__, __LINE__,
        ye, mo, da,
        ho, mi, se, na));

    //  Normalize nanosecond.
    ad += na / Constant::ns_per_s;
    na %= Constant::ns_per_s;

    //  Years.

    // Convert from one-based Common Era year to zero-based elapsed years.
    uint64_t years = ye - 1;

    uint64_t quadricenturies = years / y_per_quadricentury;
    ad += quadricenturies * s_per_quadricentury;
    years -= quadricenturies * y_per_quadricentury;

    DEBUG_PRINTF_IF(d_bug,
           ("%s[%d]: ad=%llu quadricenturies=%llu year=%llu\n",
        __FILE__, __LINE__, ad, quadricenturies, ye - years));

    uint32_t centuries = years / y_per_century;
    ad += centuries * s_per_century;
    years -= centuries * y_per_century;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu centuries=%lu year=%llu\n",
        __FILE__, __LINE__, ad, centuries, ye - years));

    uint32_t quadriyears = years / y_per_quadriyear;
    ad += quadriyears * s_per_quadriyear;
    years -= quadriyears * y_per_quadriyear;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu quadriyears=%lu year=%llu\n",
        __FILE__, __LINE__, ad, quadriyears, ye - years));

    bool ly;
    // Convert from zero-based elapsed years to one-based Common Era year.
    uint64_t yr = ye - years;
    while (yr < ye) {
        ly = this->getDate().isLeapYear(yr);
        ad += s_per_year[ly];
        ++yr;
        --years;
    }

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu year=%llu\n",
        __FILE__, __LINE__, ad, ye - years));

    //  Months.

    uint8_t mh = 1;
    ly = this->getDate().isLeapYear(ye);
    while (mh < mo) {
        ad += s_per_month[ly][mh - 1];
        ++mh;
    }

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu month=%u\n",
        __FILE__, __LINE__, ad, mh));

    //  Days.

    if (0 < da) {
        ad += (da - 1) * Constant::s_per_d;
    }

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu day=%u\n",
        __FILE__, __LINE__, ad, da));

    //  Hours.

    ad += ho * Constant::s_per_h;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu hour=%u\n",
        __FILE__, __LINE__, ad, ho));

    //  Minutes.

    ad += mi * Constant::s_per_min;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu minute=%u\n",
        __FILE__, __LINE__, ad, mi));

    //  Seconds.

    ad += se;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu second=%u\n",
        __FILE__, __LINE__, ad, se));

    //  Nanoseconds.

    nd = na;

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: ad=%llu nanoseconds=%lu\n",
        __FILE__, __LINE__, ad, na));

    return ad;
}


//
//  Get this common era object in epoch seconds.
//
uint64_t CommonEra::toAtomicSeconds() const {
    uint32_t nd;
    return this->toAtomicSeconds(nd);
}


//
//  Get this common era object in epoch seconds plus nanoseconds.
//
seconds_t CommonEra::toSeconds(uint32_t& nd) const {
    LeapSeconds& lsrule = Platform::instance().getLeapSeconds();
    int ld = 0;
    const seconds_t* sp = lsrule.find(*this, ld);
    seconds_t sd;
    if (0 != sp) {
        sd = *sp;
        nd = this->getNanosecond();
    } else {
        sd = this->toAtomicSeconds(nd);
        sd += ld;
    }
    return sd;
}


//
//  Get this common era object in epoch seconds.
//
seconds_t CommonEra::toSeconds() const {
    uint32_t nd;
    return this->toSeconds(nd);
}


//
//  Set this common era object from platform ticks.
//
void CommonEra::fromTicks(ticks_t tk) {

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: fromTicks(%llu)\n",
        __FILE__, __LINE__, tk));

    Platform& pl = Platform::instance();

    const Epoch& epoch = pl.getEpoch();
    seconds_t es = epoch.seconds;
    uint32_t en = epoch.nanoseconds;

    Ticks ticks;
    uint64_t ps;
    uint32_t pn;
    ticks.seconds(tk, ps, pn);

    en += pn;
    es += ps + (en / Constant::ns_per_s);
    en %= Constant::ns_per_s;

    bool lst = pl.getLeapSecondTicks();
    if (lst) {
        this->fromSeconds(es, en);
    } else {
        this->fromAtomicSeconds(es, en);
    }

    DEBUG_PRINTF_IF(d_bug, ("%s[%d]: es=%llu en=%lu\n",
        __FILE__, __LINE__, es, en));
}


//
//  Set this common era object from the current time.
//
void CommonEra::fromNow() {
    this->fromTicks(Platform::instance().time());
}


//
//  Compare two objects.
//
int CommonEra::compare(const CommonEra& that) const {
    return this->DateTime::compare(that);
}


//
//  Show this object on the output object.
//
void CommonEra::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->DateTime::show(level, display, indent + 1);
}


#include "com/diag/desperado/End.h"
