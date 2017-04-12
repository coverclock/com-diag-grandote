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
 *  Implements the Date class.
 *
 *  @see    Date
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Date.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


static const uint8_t d_per_month[2][Constant::months_per_year] = {
    {           // non-leap years
        31,     // January
        28,     // February
        31,     // March
        30,     // April
        31,     // May
        30,     // June
        31,     // July
        31,     // August
        30,     // September
        31,     // October
        30,     // November
        31      // December
    },
    {           // leap years
        31,     // January
        29,     // February
        31,     // March
        30,     // April
        31,     // May
        30,     // June
        31,     // July
        31,     // August
        30,     // September
        31,     // October
        30,     // November
        31      // December
    }
};


static const uint16_t cum_d_per_month[2][Constant::months_per_year] = {
    {           // non-leap years
        0,      // January
        31,     // February
        59,     // March
        90,     // April
        120,    // May
        151,    // June
        181,    // July
        212,    // August
        243,    // September
        273,    // October
        304,    // November
        334     // December
    },
    {           // leap years
        0,      // January
        31,     // February
        60,     // March
        91,     // April
        121,    // May
        152,    // June
        182,    // July
        213,    // August
        244,    // September
        274,    // October
        305,    // November
        335     // December
    }
};


static const char* months[] = {
    "",             // 0
    "January",      // 1
    "February",     // 2
    "March",        // 3
    "April",        // 4
    "May",          // 5
    "June",         // 6
    "July",         // 7
    "August",       // 8
    "September",    // 9
    "October",      // 10
    "November",     // 11
    "December"      // 12
};


static const char* weekdays[] = {
    "",             // 0
    "Monday",       // 1
    "Tuesday",      // 2
    "Wednesday",    // 3
    "Thursday",     // 4
    "Friday",       // 5
    "Saturday",     // 6
    "Sunday"        // 7
};


static const char* ordinals[] = {
    "",             // 0
    "first ",       // 1
    "second",       // 2
    "third",        // 3
    "fourth",       // 4
    "fifth",        // 5
    "last",         // 6
};


//
//  Constructor.
//
Date::Date(uint64_t yr, uint8_t mh, uint8_t dy) :
    year(yr),
    month(mh),
    day(dy)
{
}


//
//  Destructor.
//
Date::~Date() {
}


//
//  Convert to a string. We play some games with the output
//  format so it looks good for commonly used years in the
//  range 1..9999. The return value from snprintf() depends
//  on the version of glibc; we accomodate either semantic.
//
size_t Date::toString(String string, size_t size) const {
    const char* format = (10000 > this->year)
        ?  "%04llu-%02u-%02u" : "%llu-%02u-%02u";
    int octets = ::snprintf(string, size, format,
        this->year, this->month, this->day);
    return ((0 <= octets) && (octets < static_cast<int>(size)))
        ? static_cast<size_t>(octets)
        : (0 < size) ? (size - 1) : 0;
}


//
//  Return the name of the month.
//
const char* Date::monthToString(Month mh) const {
    static const int limit = countof(months);
    return months[((0 <= mh) && (mh < limit)) ? mh : 0];
}


//
//  Return the name of the weekday.
//
const char* Date::weekdayToString(Weekday dy) const {
    static const int limit = countof(weekdays);
    return weekdays[((0 <= dy) && (dy < limit)) ? dy : 0];
}


//
//  Return the name of the weekday.
//
const char* Date::ordinalToString(Ordinal od) const {
    static const int limit = countof(ordinals);
    return ordinals[((0 <= od) && (od < limit)) ? od : 0];
}


//
//  Returns true if this object is valid, false otherwise.
//
bool Date::isValid() const {
    return
        (0 < this->year)
    &&
        (Date::JANUARY <= this->month)
    &&
        (this->month <= Date::DECEMBER)
    &&
        (this->day <= d_per_month[this->isLeapYear()][this->month - 1])
    ;
}


//
//  Return the number of days in the specified month and year.
//
uint8_t Date::cardinal(uint64_t yr, Month mh) const {
    return d_per_month[this->isLeapYear(yr)][mh - 1];
}


//
//  Returns a weekday index from the given date. Zeller's Congruence
//  Algorithm returns Sunday through Saturday encoded zero through
//  seven. We have to jump through some hoops to get ISO8601, which
//  encodes Monday through Sunday as one through seven.
//
Date::Weekday Date::weekday(uint64_t yr, uint8_t mh, uint8_t dy) const {
    int ye = ((yr - 1) % 400) + 1;
    int a = (14 - mh) / 12;
    int y = ye - a;
    int m = mh + (12 * a) - 2;
    int d = dy + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12);
    return static_cast<Weekday>((((d % 7) + 6) % 7) + 1);
}


//
//  Returns the day of the month of the nth weekday.
//
uint8_t Date::ordinal(Ordinal nth, Weekday wy) const {
    const uint8_t dpm = d_per_month[this->isLeapYear()][this->month - 1];
    uint8_t from;
    uint8_t to;
    if ((FIRST <= nth) && (nth <= FIFTH)) {
        from = ((nth - 1) * 7) + 1;
        to = from + 6;
        if (dpm < to) {
            to = dpm;
        }
    } else {
        to = dpm;
        from = to - 6;
    }
    uint8_t rc = 0;
    uint8_t dw = this->weekday(this->year, this->month, from);
    for (; from <= to; ++from) {
        if (wy == dw) {
            rc = from;
            break;
        }
        dw = (dw % 7) + 1;
    }
    return rc;
}


//
//  Compute the julian day for the given date.
//
uint16_t Date::julian(uint64_t yr, uint8_t mh, uint8_t dy) const {
    return cum_d_per_month[this->isLeapYear(yr)][mh - 1] + dy;
}


//
//  Compare two objects.
//
int Date::compare(const Date& that) const {
    int rc = 0;
    if (this->year < that.year) {
        rc = -1;
    } else if (this->year > that.year) {
        rc = 1;
    } else if (this->month < that.month) {
        rc = -1;
    } else if (this->month > that.month) {
        rc = 1;
    } else if (this->day < that.day) {
        rc = -1;
    } else if (this->day > that.day) {
        rc = 1;
    }
    return rc;
}


//
//  Show this object on the output object.
//
void Date::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s year=%llu\n", sp, this->year);
    printf("%s month=%u\n", sp, this->month);
    printf("%s day=%u\n", sp, this->day);
    if (0 < level) {
        printf("%s isValid()=%d\n", sp, this->isValid());
        printf("%s isLeapYear()=%d\n", sp, this->isLeapYear());
        printf("%s weekday()=%d\n", sp, this->weekday());
        printf("%s julian()=%u\n", sp, this->julian());
    }
}


#include "com/diag/desperado/End.h"
