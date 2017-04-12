#ifndef _COM_DIAG_DESPERADO_TIME_H_
#define _COM_DIAG_DESPERADO_TIME_H_

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
 *  Declares the Time class.
 *
 *  @see    Time
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an object that contains a time with fields
 *  of sufficient size to contain a coordinated universal time
 *  hour, minute, second, and nanosecond.
 *
 *  @see    CommonEra
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Time : public Object {

public:

    /**
     *  Constructor. Input fields are not validity checked. Hence,
     *  invalid times may be contructed.
     *
     *  @param  hr      is the hour of the day [0..23].
     *
     *  @param  me      is the minute of the hour [0..59].
     *
     *  @param  sd      is the second of the minute [0..59].
     *
     *  @param  nd      is the nanosecond of the second [0..999999999].
     */
    explicit Time(
        uint8_t hr = 0,
        uint8_t me = 0,
        uint8_t sd = 0,
        uint32_t nd = 0
    );

    /**
     *  Destructor.
     */
    virtual ~Time();

    /**
     *  This defines a buffer large enough to contain a
     *  time printed in canonical form including a
     *  terminating NUL character.
     */
    typedef char String[sizeof("hh:mm:ss.nnnnnnnnn")];

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
     *  Hour accessor.
     *
     *  @return the hour of the day.
     */
    uint8_t getHour() const;

    /**
     *  Minute accessor.
     *
     *  @return the minute of the hour.
     */
    uint8_t getMinute() const;

    /**
     *  Second accessor.
     *
     *  @return the second of the minute.
     */
    uint8_t getSecond() const;

    /**
     *  Nanosecond accessor.
     *
     *  @return the nanosecond of the second.
     */
    uint32_t getNanosecond() const;

    /**
     *  Hour settor.
     *
     *  @param  hr      is the hour of the epoch.
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
     *  Returns true if this object represents a valid time
     *  in the Common Era. There is no prohibition against
     *  constructing invalid times, although results are
     *  unpredictable.
     *
     *  @return true if this object is valid, false otherwise.
     */
    virtual bool isValid() const;

    /**
     *  These are the encodings for A.M. (antemeridiem) and
     *  P.M. (postmeridiem).
     */
    enum Meridiem {
        AM  =   0,
        PM  =   1
    };

    /**
     *  Returns a meridiem name given a Meridiem enumerator.
     *
     *  @param  mm      is the meridiem.
     *
     *  @return the name of the meridiem or "" if invalid.
     */
    virtual const char* meridiemToString(Meridiem mm) const;

    /**
     *  Returns a meridiem name of this object.
     *
     *  @return the name of the meridiem or "" if invalid.
     */
    const char* meridiemToString() const;

    /**
     *  Returns AM if the specified hour falls in
     *  antemeridiem (A.M.), PM if in postmeridiem (P.M.).
     *  Also returns the hour converted into a twelve-hour
     *  clock.
     *
     *  @param  hr      is an hour of the day.
     *
     *  @param  twelve  refers to a variable into which is
     *                  placed the hour converted to a twelve
     *                  hour clock.
     *
     *  @return AM for A.M., PM for P.M.
     */
    virtual Meridiem oclock(uint8_t hr, uint8_t& twelve) const;

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
    Meridiem oclock(uint8_t& twelve) const;

    /**
     *  Compares two times.
     *
     *  @param  that    refers to another time object.
     *
     *  @return <0 if (this < that), 0 of (this==that), >0 if (this>that).
     */
    int compare(const Time& that) const;

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
     *  This is the nanosecond of the Common Era second.
     */
    uint32_t nanosecond;

    /**
     *  This is the hour of the Common Era day.
     */
    uint8_t hour;

    /**
     *  This is the minute of the Common Era hour.
     */
    uint8_t minute;

    /**
     *  This is the second of the Common Era minute.
     */
    uint8_t second;

    /**
     *  This field is reserved.
     */
    uint8_t reserved;
};


//
//  Hour accessor.
//
inline uint8_t Time::getHour() const {
    return this->hour;
}


//
//  Minute accessor.
//
inline uint8_t Time::getMinute() const {
    return this->minute;
}


//
//  Second accessor.
//
inline uint8_t Time::getSecond() const {
    return this->second;
}


//
//  Nanosecond accessor.
//
inline uint32_t Time::getNanosecond() const {
    return this->nanosecond;
}


//
//  Hour settor.
//
inline void Time::setHour(uint8_t hr) {
    this->hour = hr;
}


//
//  Minute settor.
//
inline void Time::setMinute(uint8_t me) {
    this->minute = me;
}


//
//  Second settor.
//
inline void Time::setSecond(uint8_t sd) {
    this->second = sd;
}


//
//  Nanosecond settor.
//
inline void Time::setNanosecond(uint32_t nd) {
    this->nanosecond = nd;
}


//
//  Return false for A.M., true for P.M.
//
inline Time::Meridiem Time::oclock(uint8_t& twelve) const {
    return this->oclock(this->hour, twelve);
}


//
//  Return the meridiem name of this object.
//
inline const char* Time::meridiemToString() const {
    uint8_t twelve;
    return this->meridiemToString(this->oclock(twelve));
}

#include "com/diag/desperado/End.h"


#endif
