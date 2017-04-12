#ifndef _COM_DIAG_DESPERADO_LOCALTIME_H_
#define _COM_DIAG_DESPERADO_LOCALTIME_H_

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
 *  Declares the LocalTime class.
 *
 *  @see    LocalTime
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/types.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/DateTime.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/DaylightSavingTime.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a DataTime in local civilian time (LCT), taking into
 *  account leap seconds, time zone offset from UTC, and whether
 *  daylight saving time is in effect or not, when converting from
 *  a CommonEra DataTime. Common Era Era dates and times can be
 *  converted into Local Time dates and times, but the reverse is
 *  not the case. Because of DST, Local Time times are ambiguous.
 *
 *  @see    CommonEra
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class LocalTime : public DateTime {

public:

    /**
     *  This special value indicates that the UTC offset has not
     *  been set and must be retrieved from the platform.
     */
    static const int32_t offunset = intminof(int32_t);

    /**
     *  Constructor. The date and time are initialized to the
     *  Common Era epoch. The time zone offset and the
     *  daylight saving time rule are initialized from the
     *  platform defaults.
     */
    explicit LocalTime();

    /**
     *  Constructor. The date and time are initialized to the
     *  Common Era epoch and daylight saving time is not in use.
     *
     *  @param  ot              is the signed offset from UTC
     *                          in seconds [-43200..43200].
     */
    explicit LocalTime(int32_t ot);

    /**
     *  Constructor. The date and time are initialized to the
     *  Common Era epoch.
     *
     *  @param  ot              is the signed offset from UTC
     *                          in seconds [-43200..43200].
     *
     *  @param  re              refers to a daylight saving time
     *                          rule.
     */
    explicit LocalTime(int32_t ot, DaylightSavingTime& re);

    /**
     *  Destructor.
     */
    virtual ~LocalTime();

    /**
     *  This defines a buffer large enough to contain a date
     *  and time printed in canonical form including
     *  a terminating NUL character.
     */
    typedef char String[sizeof(DateTime::String) + sizeof("+2147483647DST")];

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
     *  Returns the offset in seconds.
     *
     *  @return the offset in seconds [-43200..43200].
     */
    int32_t getOffset() const;

    /**
     *  Returns a reference to the DST rule.
     *
     *  @return a reference to the DST rule.
     */
    DaylightSavingTime& getDaylightSavingTime() const;

    /**
     *  Returns true if DST is currently in force, false otherwise.
     *
     *  @return true if DST is currently in force, false otherwise.
     */
    bool getDst() const;

    /**
     *  Returns true if this object represents a valid Local
     *  Time date and time. There is no prohibition against
     *  constructing invalid Common Era objects. Validity implies
     *  merely that the date and time fields are correct, not
     *  that the object can be successfully be converted into
     *  seconds since the CE epoch and back again.
     *
     *  @return true if this object is valid, false otherwise.
     */
    bool isValid() const;

    /**
     *  Re-initialize this object using CE seconds.
     *
     *  @param  sd      is the time in CE seconds.
     *
     *  @param  nd      is the fraction of a second in nanoseconds
     *                  [0..999999999].
     */
    virtual void fromSeconds(seconds_t sd, uint32_t nd = 0);

    /**
     *  Re-initialize this object using platform ticks.
     *
     *  @param  tk      is the time in platform ticks.
     */
    virtual void fromTicks(ticks_t tk);

    /**
     *  Re-initialize this object using a CE object.
     *
     *  @param  ce      refers to the CE object;
     */
    virtual void fromCommonEra(const CommonEra& ce);

    /**
     *  Re-initialize this object using the current time.
     */
    virtual void fromNow();

    /**
     *  Compares two Local Time dates and times.
     *
     *  Comparing two Local Time object which do not have the same
     *  offset or daylight saving time state is undefined. Such
     *  objects will never compare equal.
     *
     *  @param  that    refers to another Local Time object.
     *
     *  @return <0 if (this < that), 0 of (this==that), >0 if (this>that).
     */
    int compare(const LocalTime& that) const;

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
     *  This is the signed offset from UTC in seconds.
     */
    int32_t offset;

    /**
     *  This points to the DST rule.
     */
    DaylightSavingTime* rule;

    /**
     *  This is true if the date and time of this object are currently
     *  within daylight saving time, false otherwise. This value must
     *  be computed when the conversion is done from a Common Era time.
     */
    bool dst;

};


//
//  Return offset.
//
inline int32_t LocalTime::getOffset() const {
    return (this->offunset == this->offset)
        ? Platform::instance().getOffset()
        : this->offset;
}


//
//  Return rule.
//
inline DaylightSavingTime& LocalTime::getDaylightSavingTime() const {
    return (0 == this->rule)
        ? Platform::instance().getDaylightSavingTime()
        : *this->rule;
}


//
//  Return DST in force.
//
inline bool LocalTime::getDst() const {
    return this->dst;
}

#include "com/diag/desperado/End.h"


#endif
