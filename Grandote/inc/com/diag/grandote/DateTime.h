#ifndef _COM_DIAG_DESPERADO_DATETIME_H_
#define _COM_DIAG_DESPERADO_DATETIME_H_

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
 *  Declares the DateTime class.
 *
 *  @see    DateTime
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Date.h"
#include "com/diag/desperado/Time.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Composes a Date object and a Time object into a single object.
 *
 *  @see    CommonEra
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class DateTime : public Object {

public:

    /**
     *  Constructor. Date and time is represented by the seperate LT
     *  components. Passing values for the parameters which do
     *  not represent a valid Local Time date and time yields
     *  unpredictable results. No conversion is done for the date
     *  or time.
     *
     *  @param  yr      is the year of the epoch [1..maximumyear].
     *
     *  @param  mh      is the month of the year [1..12].
     *
     *  @param  dy      is the day of the month [1..31] (depending on
     *                  the month and year).
     *
     *  @param  hr      is the hour of the day [0..23].
     *
     *  @param  me      is the minute of the hour [0..59].
     *
     *  @param  sd      is the second of the minute [0..59].
     *
     *  @param  nd      is the fraction of a second in nanoseconds
     *                  [0..999999999].
     */
    explicit DateTime(
        uint64_t yr = 1,
        uint8_t mh = 1,
        uint8_t dy = 1,
        uint8_t hr = 0,
        uint8_t me = 0,
        uint8_t sd = 0,
        uint32_t nd = 0
    );

    /**
     *  Constructor. Date and time are represented by objects.
     *  Passing objects containing invalid Local Time date or
     *  time yields unpredictable results. No conversion is done
     *  for the date or time.
     *
     *  @param  de      refers to a date object.
     *
     *  @param  te      refers to a time object.
     */
    explicit DateTime(
        const Date& de,
        const Time& te
    );

    /**
     *  Destructor.
     */
    virtual ~DateTime();

    /**
     *  This defines a buffer large enough to contain a date
     *  and time printed in canonical form including
     *  a terminating NUL character.
     */
    typedef char String[sizeof(Date::String) + sizeof(Time::String)];

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
     *  Returns a constant reference to the date.
     *
     *  @return a constant reference to the date.
     */
    const Date& getDate() const;

    /**
     *  Returns a constant reference to the time.
     *
     *  @return a constant reference to the time.
     */
    const Time& getTime() const;

    /**
     *  Returns the year.
     *
     *  @return the year.
     */
    uint64_t getYear() const;

    /**
     *  Returns the month of the year.
     *
     *  @return the month of the year.
     */
    uint8_t getMonth() const;

    /**
     *  Returns the day of the month.
     *
     *  @return the day of the month.
     */
    uint8_t getDay() const;

    /**
     *  Returns the hour of the day.
     *
     *  @return the hour of the day.
     */
    uint8_t getHour() const;

    /**
     *  Returns the minute of the hour.
     *
     *  @return the minute of the hour.
     */
    uint8_t getMinute() const;

    /**
     *  Returns the second of the minute.
     *
     *  @return the second of the minute.
     */
    uint8_t getSecond() const;

    /**
     *  Returns the fraction of a second in nanoseconds.
     *
     *  @return the fractions of a second in nanoseconds.
     */
    uint32_t getNanosecond() const;

    /**
     *  Year settor.
     *
     *  @param  yr      is the year [1..maximumyear].
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
     *  Hour settor.
     *
     *  @param  hr      is the hour of the day.
     */
    void setHour(uint8_t hr);

    /**
     *  Minute settor.
     *
     *  @param  me      is the minute of the hour.
     *
     */
    void setMinute(uint8_t me);

    /**
     *  Second settor.
     *
     *  @param  sd      is the second of the minute.
     *
     */
    void setSecond(uint8_t sd);

    /**
     *  Nanosecond settor.
     *
     *  @param  nd      is the nanosecond of the second.
     *
     */
    void setNanosecond(uint32_t nd);

    /**
     *  Returns true if this object represents a valid Local
     *  Time date and time. There is no prohibition against
     *  constructing invalid date and time objects. Validity implies
     *  merely that the date and time fields are correct, not
     *  that the object can be successfully be converted from
     *  one date and time form (like CE seconds) to another.
     *
     *  @return true if this object is valid, false otherwise.
     */
    virtual bool isValid() const;

    /**
     *  Returns true if this object is in a leap year, false otherwise.
     *
     *  @return true if this object is in a leap year, false otherwise.
     */
    bool isLeapYear() const;

    /**
     *  Returns the number of days in the month of this object.
     *
     *  @return the number of days in the month of this object.
     */
    uint8_t cardinal() const;

    /**
     *  Computes an index indicating the day of the week, zero
     *  for Sunday, six for Saturday, etc. for this object.
     *
     *  @return the ISO8601 day of the week: one (Monday) through
     *          seven (Sunday).
     */
    Date::Weekday weekday() const;

    /**
     *  Returns the day of the month for the n-th day of the week,
     *  e.g. the first Wednesday, the third Monday, etc., for this
     *  object. If there is no n-th of the week (for example, if
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
    uint8_t ordinal(Date::Ordinal nth, Date::Weekday wy) const;

    /**
     *  Computes the julian day of the year [1..366] for this object.
     *
     *  @return the julian day of the year.
     */
    uint16_t julian() const;

    /**
     *  Returns AM if the time in this object falls in
     *  antemeridiem (A.M.), PM if in postmeridiem (P.M.).
     *  Also returns the hour converted into a twelve-hour
     *  clock.
     *
     *  @param  twelve  refers to a variable into which is
     *                  placed the hour converted to a twelve
     *                  hour clock.
     *
     *  @return AM for A.M., PM for P.M.
     */
    Time::Meridiem oclock(uint8_t& twelve) const;

    /**
     *  Compares two dates and times.
     *
     *  @param  that    refers to another date and time object.
     *
     *  @return <0 if (this < that), 0 of (this==that), >0 if (this>that).
     */
    int compare(const DateTime& that) const;

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
     *  This is the date of the epoch.
     */
    Date date;

    /**
     *  This is the time of day.
     */
    Time time;

};


//
//  Access date.
//
inline const Date& DateTime::getDate() const {
    return this->date;
}


//
//  Access time.
//
inline const Time& DateTime::getTime() const {
    return this->time;
}


//
//  Access year.
//
inline uint64_t DateTime::getYear() const {
    return this->date.getYear();
}


//
//  Access month.
//
inline uint8_t DateTime::getMonth() const {
    return this->date.getMonth();
}


//
//  Access day.
//
inline uint8_t DateTime::getDay() const {
    return this->date.getDay();
}


//
//  Access hour.
//
inline uint8_t DateTime::getHour() const {
    return this->time.getHour();
}


//
//  Access minute.
//
inline uint8_t DateTime::getMinute() const {
    return this->time.getMinute();
}


//
//  Access second.
//
inline uint8_t DateTime::getSecond() const {
    return this->time.getSecond();
}


//
//  Access nanoseconds.
//
inline uint32_t DateTime::getNanosecond() const {
    return this->time.getNanosecond();
}


//
//  Year settor.
//
inline void DateTime::setYear(uint64_t yr) {
    this->date.setYear(yr);
}


//
//  Month settor.
//
inline void DateTime::setMonth(uint8_t mh) {
    this->date.setMonth(mh);
}


//
//  Day settor.
//
inline void DateTime::setDay(uint8_t dy) {
    this->date.setDay(dy);
}


//
//  Hour settor.
//
inline void DateTime::setHour(uint8_t hr) {
    this->time.setHour(hr);
}


//
//  Minute settor.
//
inline void DateTime::setMinute(uint8_t me) {
    this->time.setMinute(me);
}


//
//  Second settor.
//
inline void DateTime::setSecond(uint8_t sd) {
    this->time.setSecond(sd);
}


//
//  CE nanosecond settor.
//
inline void DateTime::setNanosecond(uint32_t nd) {
    this->time.setNanosecond(nd);
}


//
//  Returns true if the common era object is a leap year, false otherwise.
//
inline bool DateTime::isLeapYear() const {
    return this->date.isLeapYear();
}


//
//  Returns the number of days in the month of this common era object.
//
inline uint8_t DateTime::cardinal() const {
    return this->date.cardinal();
}


//
//  Returns a weekday index from this common era object.
//
inline Date::Weekday DateTime::weekday() const {
    return this->date.weekday();
}


//
//  Returns the ordinal day of the month for this common era object.
//
inline uint8_t DateTime::ordinal(Date::Ordinal nth, Date::Weekday wy) const {
    return this->date.ordinal(nth, wy);
}


//
//  Compute the julian day for the this object.
//
inline uint16_t DateTime::julian() const {
    return this->date.julian();
}


//
//  Return false for A.M., true for P.M.
//
inline Time::Meridiem DateTime::oclock(uint8_t& twelve) const {
    return this->time.oclock(twelve);
}

#include "com/diag/desperado/End.h"


#endif
