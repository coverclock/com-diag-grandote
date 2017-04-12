#ifndef _COM_DIAG_DESPERADO_DATE_H_
#define _COM_DIAG_DESPERADO_DATE_H_

/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Declares the Date class.
 *
 *  @see    Date
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an object that contains a date with fields
 *  of sufficient size to contain a Common Era year, month
 *  and day of the month.
 *
 *  @see    CommonEra
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Date : public Object {

public:

    /**
     *  Constructor. Input fields are not validity checked. Hence,
     *  invalid dates may be contructed.
     *
     *  @param  yr      is the year of the epoch [1..maximumyear].
     *
     *  @param  mh      is the month of the year [1..12].
     *
     *  @param  dy      is the day of the month [1..[28..31]].
     */
    explicit Date(
        uint64_t yr = 1,
        uint8_t  mh = 1,
        uint8_t dy = 1
    );

    /**
     *  Destructor.
     */
    virtual ~Date();

    /**
     *  This defines a buffer large enough to contain a
     *  date printed in canonical form including a
     *  terminating NUL character. The buffer is long enough
     *  to contain the largest possible valid year.
     */
    typedef char String[sizeof("YYYYYYYYYYYY-MM-DD")];

    /**
     *  Convert this object into a string. The resulting string is
     *  guaranteed to be NUL terminated as long as the length of
     *  of the buffer is greater than zero.
     *
     *  @param  string      points to a date and time string array.
     *
     *  @param  size        is the size of the string array in octets.
     *
     *  @return the number of characters placed in the buffer not including
     *          the terminating NUL character. Since the buffer is guaranteed
     *          to be NUL terminated if it of greater than zero length,
     *          this return value is guaranteed to always be less than
     *          the value of the size parameter in the same circumstances.
     */
    virtual size_t toString(String string, size_t size = sizeof(String)) const;

    /**
     *  Year accessor.
     *
     *  @return the year of the epoch.
     */
    uint64_t getYear() const;

    /**
     *  Month accessor.
     *
     *  @return the month of the year.
     */
    uint8_t getMonth() const;

    /**
     *  Day accessor.
     *
     *  @return the day of the month.
     */
    uint8_t getDay() const;

    /**
     *  Year settor.
     *
     *  @param  yr      is the year of the epoch [1..maximumyear].
     */
    void setYear(uint64_t yr);

    /**
     *  Month settor.
     *
     *  @param  mh      is the month of the year [1..12].
     *
     */
    void setMonth(uint8_t mh);

    /**
     *  Day settor.
     *
     *  @param  dy      is the day of the month [1..[28..31]].
     *
     */
    void setDay(uint8_t dy);

    /**
     *  Returns true if this object represents a valid date
     *  in the Common Era. There is no prohibition against
     *  constructing invalid dates, although results are
     *  unpredictable.
     *
     *  @return true if this object is valid, false otherwise.
     */
    bool isValid() const;

    /**
     *  Returns true if the year is a leap year, false otherwise.
     *
     *  @param  yr      is the year [1..maximumyear].
     *
     *  @return true if the year is a leap year, false otherwise.
     */
    bool isLeapYear(uint64_t yr) const;

    /**
     *  Returns true if this object is in a leap year, false otherwise.
     *
     *  @return true if this object is in a leap year, false otherwise.
     */
    bool isLeapYear() const;

    /**
     *  These are the encodings for the months of the year.
     */
    enum Month {
        JANUARY     =    1,
        FEBRUARY    =    2,
        MARCH       =    3,
        APRIL       =    4,
        MAY         =    5,
        JUNE        =    6,
        JULY        =    7,
        AUGUST      =    8,
        SEPTEMBER   =    9,
        OCTOBER     =   10,
        NOVEMBER    =   11,
        DECEMBER    =   12
    };

    /**
     *  Returns a month name given a Month enumerator.
     *
     *  @param  mh      is the month.
     *
     *  @return the name of the month or "" if invalid.
     */
    virtual const char* monthToString(Month mh) const;

    /**
     *  Returns a month name of this object.
     *
     *  @return the name of the month or "" if invalid.
     */
    const char* monthToString() const;

    /**
     *  Returns the number of days in the specified month and year.
     *
     *  @param  yr      is the year.
     *
     *  @param  mh      is the month.
     *
     *  @return the number of days in the month;
     */
    virtual uint8_t cardinal(uint64_t yr, Month mh) const;

    /**
     *  Returns the number of days in the month of this object.
     *
     *  @return the number of days in the month of this object.
     */
    uint8_t cardinal() const;

    /**
     *  These are the ISO8601-conformant encodings for the days of the week.
     *  ISO8601 specifies that the week starts on Monday.
     */
    enum Weekday {
        MONDAY    = 1,
        TUESDAY   = 2,
        WEDNESDAY = 3,
        THURSDAY  = 4,
        FRIDAY    = 5,
        SATURDAY  = 6,
        SUNDAY    = 7
    };

    /**
     *  Returns a weekday name given a Weekday enumerator.
     *
     *  @param  wy      is the weekday.
     *
     *  @return the name of the weekday or "" if invalid.
     */
    virtual const char* weekdayToString(Weekday wy) const;

    /**
     *  Returns a weekday name for the weekday of this object.
     *
     *  @return the name of the weekday or "" if invalid.
     */
    const char* weekdayToString() const;

    /**
     *  Computes an index indicating the day of the week, zero
     *  for Sunday, six for Saturday, etc. for given date. Uses
     *  Zeller's Congruence Algorithm, circa 1882.
     *
     *  The ZCA was only intended to work with years in the range
     *  [1..9999]. But here's where something remarkable comes to
     *  light. Every block of 400 years (or one quadricentury) has
     *  a constant number of days, because 400 years is the cycle of
     *  the leap year algorithm. And it turns out the number of days
     *  in a quadricentury is an even multiple of seven. So if you
     *  take years modulo the number of years in a quadricentury, the
     *  weekday calculation comes out the same.
     *
     *  (I can't quite bring myself to believe that this is a coincidence,
     *  particularly given that seven is a prime number. This has to
     *  be something fundamental about the design of the Gregorian
     *  calendar.)
     *
     *  @param  yr      is the year [1..maximumyear].
     *
     *  @param  mh      is the month [1..12].
     *
     *  @param  dy      is the day of the month [1..31] depending on the
     *                  month.
     *
     *  @return the ISO8601 day of the week: one (Monday) through
     *          seven (Sunday).
     */
    Weekday weekday(uint64_t yr, uint8_t mh, uint8_t dy) const;

    /**
     *  Computes an index indicating the day of the week, zero
     *  for Sunday, six for Saturday, etc. for this object. Uses
     *  Zeller's Congruence Algorithm, circa 1882.
     *
     *  @return the ISO8601 day of the week: one (Monday) through
     *          seven (Sunday).
     */
    Weekday weekday() const;

    /**
     *  These are encodings for the ordinal of the day of the week.
     */
    enum Ordinal {
        FIRST   =   1,
        SECOND  =   2,
        THIRD   =   3,
        FOURTH  =   4,
        FIFTH   =   5,
        LAST    =   6
    };

    /**
     *  Returns a ordinal name given an Ordinal enumerator.
     *
     *  @param  od      is the ordinal.
     *
     *  @return the name of the ordinal or "" if invalid.
     */
    virtual const char* ordinalToString(Ordinal od) const;

    /**
     *  Returns the day of the month for the n-th day of the week,
     *  e.g. the first Wednesday, the third Monday, etc., for this
     *  object If there is no n-th of the week (for example, if
     *  there is no fifth Sunday in February), zero is returned.
     *
     *  @param  nth         is the ordinal, e.g. 1 for first, 2
     *                      for second, 6 for last.
     *
     *  @param  wy          is the weekday, e.g. MONDAY, FRIDAY.
     *
     *  @return the day of the month upon which the ordinal weekday
     *          falls, or zero if it does not exist.
     */
    virtual uint8_t ordinal(Ordinal nth, Weekday wy) const;

    /**
     *  Computes the julian day of the year [1..366] for the given date.
     *
     *  @param  yr      is the year [1..maximumyear].
     *
     *  @param  mh      is the month [1..12].
     *
     *  @param  dy      is the day of the month [1..31] depending on the
     *                  month.
     *
     *  @return the julian day of the year.
     */
    virtual uint16_t julian(uint64_t yr, uint8_t mh, uint8_t dy) const;

    /**
     *  Computes the julian day of the year [1..366] for this object.
     *
     *  @return the julian day of the year.
     */
    uint16_t julian() const;

    /**
     *  Compares two dates.
     *
     *  @param  that    refers to another date object.
     *
     *  @return <0 if (this < that), 0 of (this==that), >0 if (this>that).
     */
    int compare(const Date& that) const;

    /**
     *  Displays internal information about this object to the specified
     *  output object. Useful for debugging and troubleshooting.
     *
     *  @param  level   sets the verbosity of the output. What this means
     *                  is object dependent. However, the level is passed
     *                  from outer to inner objects this object calls the
     *                  show methods of its inherited or composited objects.
     *
     *  @param display  points to the output object to which output is
     *                  sent. If null (zero), the default platform output
     *                  object is used as the effective output object. The
     *                  effective output object is passed from outer to
     *                  inner objects as this object calls the show methods
     *                  of its inherited and composited objects.
     *
     *  @param  indent  specifies the level of indentation. One more than
     *                  this value is passed from outer to inner objects
     *                  as this object calls the show methods of its
     *                  inherited and composited objects.
     */
    virtual void show(int level = 0, Output* display = 0, int indent = 0) const;

private:

    /**
     *  This is the year of the Common Era epoch. Sixty-four bits may
     *  sound excessive for the year, but it guarantees that we can
     *  safely convert the full range of sixty-four bits of seconds since
     *  the Common Era epoch.
     */
    uint64_t year;

    /**
     *  This is the month of the Common Era year.
     */
    uint8_t month;

    /**
     *  This is the day of the Common Era month.
     */
    uint8_t day;

};


//
//  Year accessor.
//
inline uint64_t Date::getYear() const {
    return this->year;
}


//
//  Month accessor.
//
inline uint8_t Date::getMonth() const {
    return this->month;
}


//
//  Day accessor.
//
inline uint8_t Date::getDay() const {
    return this->day;
}


//
//  Year settor.
//
inline void Date::setYear(uint64_t yr) {
    this->year = yr;
}


//
//  Month settor.
//
inline void Date::setMonth(uint8_t mh) {
    this->month = mh;
}


//
//  Day settor.
//
inline void Date::setDay(uint8_t dy) {
    this->day = dy;
}

//
//  Returns true if the year is a leap year, false otherwise.
//
inline bool Date::isLeapYear(uint64_t yr) const {
    return (0 == (yr % 4)) && ((0 != (yr % 100)) || (0 == (yr % 400)));
}


//
//  Returns true if this object is a leap year, false otherwise.
//
inline bool Date::isLeapYear() const {
    return this->isLeapYear(this->year);
}


//
//  Return the name of the month of this object.
//
inline const char* Date::monthToString() const {
    return this->monthToString(static_cast<Month>(this->month));
}


//
//  Return the number of days in the month of this object.
//
inline uint8_t Date::cardinal() const {
    return this->cardinal(this->year, static_cast<Month>(this->month));
}


//
//  Returns a weekday index from this object.
//
inline Date::Weekday Date::weekday() const {
    return this->weekday(this->year, this->month, this->day);
}


//
//  Return the name of the weekday of this object.
//
inline const char* Date::weekdayToString() const {
    return this->weekdayToString(this->weekday());
}


//
//  Compute the julian day for the this object.
//
inline uint16_t Date::julian() const {
    return this->julian(this->year, this->month, this->day);
}

#include "com/diag/desperado/End.h"


#endif
