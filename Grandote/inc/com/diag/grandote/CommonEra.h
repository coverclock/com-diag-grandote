#ifndef _COM_DIAG_DESPERADO_COMMONERA_H_
#define _COM_DIAG_DESPERADO_COMMONERA_H_

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
 *  Declares the CommonEra class.
 *
 *  @see    CommonEra
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/types.h"
#include "com/diag/desperado/DateTime.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an object containing a Common Era date and UTC
 *  time, supporting conversions to and from both UTC seconds,
 *  which take leap seconds into account, and TAI seconds, which
 *  do not, from the Common Era epoch.
 *
 *  The Common Era is an ISO proleptic standardization of the Gregorian
 *  calendar. The beginning of the Common Era (CE), commonly referred
 *  to as its <I>epoch</I>, was January 1, 0001, 00:00:00UTC or
 *  0001-01-01T00:00:00+00 in ISO 8601 format. The year before 1CE is
 *  1BCE or Before Common Era. There is no year zero CE.
 *
 *  This class makes an attempt to handle leap seconds so that the
 *  time of day is always in coordinated universal time (UTC). This is
 *  very problematic since leap seconds are added at the discretion
 *  of the International Earth Rotation Service (IERS). The decision
 *  of when and where to add leap seconds may not be made much far
 *  in advance of actually doing so. This means that dates in the
 *  future are difficult to calculate in UTC since it cannot be known
 *  much in advance how many seconds will be in any particular year
 *  in the future.
 *  
 *  Is is infeasible to test all possible CommonEra values. The
 *  unit test pretty thoroughly tests CE years [1..9999].
 *
 *  @see    ISO, <I>Data elements and interchange formats -
 *          Information interchange - Representation of dates
 *          and times</I>, ISO8601:1988(E), International
 *          Organization for Standardization, Geneva, Switzerland,
 *          First edition, 1988-06-15
 *
 *  @see    ITU-R, <I>Standard-frequency and time-signal emissions</I>,
 *          ITU-R TF.460-6, Internation Telecommunications Union,
 *          Radiocommunication Assembly, 2002
 *
 *  @see    E. M. Reingold, N. Dershowitz, <I>Calendrical
 *          Calculations: The Millennium Edition</I>,
 *          Cambridge University Press, Cambridge, U.K., 2002
 *
 *  @see    E. M. Reingold, <I>Errata and Notes for Calendrical
 *          Calculations: The Millennium Edition</I>,
 *          http://emr.cs.uiuc.edu/~reingold
 *
 *  @see    L. Latham, <I>Standard C Date/Time Library</I>,
 *          R&D Books, Lawrence, KS, 1998
 *
 *  @see    J. R. Stockton, <I>Date and Time Formats</I>,
 *          http://www.merlyn.demon.co.uk
 *
 *  @see    G. Klyne, C. Newman, <I>Date and Time on the Internet:
 *          Timestamps</I>, RFC 3339, The Internet Society, July 2002
 *
 *  @see    U. S. Code, Title 15, Chapter 6, Subchapter IX, "Standard Time"
 *
 *  @see    I. R. Bartky, E. Harrison, "Standard and Daylight-saving
 *          Time", <I>Scientific American</I>, May 1979
 *
 *  @see    M. Wolf, C. Wicksteed, <I>Date and Time Formats</I>,
 *          NOTE-datetime, World Wide Web Consortium, September 1997
 *
 *  @see    P. Chan et al., <I>The Java Class Libraries</I>, Second
 *          Edition, Volume 1, Addison-Wesley, 1998, pp. 1775-1776
 *
 *  @see    POSIX.1, ISO9945-1:1996(E), Annex B, 2.2.2
 *
 *  @see    time(2)
 *
 *  @see    glibc-2.2.5/time/offtime.c
 *
 *  @see    http://www.timeanddate.com
 *
 *  @see    http://greenwichmeantime.com
 *
 *  @see    http://wikipedia.org/Time_zone
 *
 *  @see    U. S. Naval Observatory,
 *          http://tycho.usno.navy.mil/leapsec.html
 *
 *  @see    http://wikipedia.org/Leap_second
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class CommonEra : public DateTime {

public:

    /**
     *  Turns class debugging on and off. For this class method
     *  to have any affect, the class must have been built with
     *  debugging support.
     *
     *  @param  now     turns debugging on if true, off otherwise.
     *
     *  @return the prior value of debugging.
     */
    static bool debug(bool now);

    /**
     *  Constructor. The date and time are initialized to the Common Era
     *  epoch.
     */
    explicit CommonEra();

    /**
     *  Constructor. Passing values for the parameters which do
     *  not represent a valid Common Era date and time yields
     *  unpredictable results.
     *
     *  @param  yr      is the CE year of the epoch [1..maximumyear].
     *
     *  @param  mh      is the CE month of the year [1..12].
     *
     *  @param  dy      is the CE day of the month [1..31] (depending on
     *                  the month and year).
     *
     *  @param  hr      is the CE hour of the day [0..23].
     *
     *  @param  me      is the CE minute of the hour [0..59].
     *
     *  @param  sd      is the CE second of the minute [0..59].
     *
     *  @param  nd      is the fraction of a second in nanoseconds
     *                  [0..999999999].
     */
    explicit CommonEra(
        uint64_t yr,
        uint8_t mh = 1,
        uint8_t dy = 1,
        uint8_t hr = 0,
        uint8_t me = 0,
        uint8_t sd = 0,
        uint32_t nd = 0
    );

    /**
     *  Destructor.
     */
    virtual ~CommonEra();

    /**
     *  This defines a buffer large enough to contain a date
     *  and time printed in canonical form including
     *  a terminating NUL character.
     */
    typedef char String[sizeof(DateTime::String) + sizeof("Z")];

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
     *  Re-initialize this object using atomic seconds. Atomic seconds
     *  are an intermediate form of seconds which does not take into account
     *  leap seconds added to International Atomic Time (TAI) to get
     *  coordinated universal time (UTC).
     *
     *  @param  ad      is the time in atomic seconds.
     *
     *  @param  nd      is the fraction of a second in nanoseconds
     *                  [0..999999999].
     */
    virtual void fromAtomicSeconds(uint64_t ad, uint32_t nd = 0);

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
     *  Represent this object in atomic seconds. Atomic seconds are an
     *  intermediate form of seconds which does not take into account
     *  leap seconds added to International Atomic Time (TAI) to get
     *  coordinated universal time (UTC).
     *
     *  @param  nd      refers to a variable into which the fractional
     *                  part in nanoseconds will be returned.
     *
     *  @return the time in this object in CE seconds.
     */
    virtual uint64_t toAtomicSeconds(uint32_t& nd) const;

    /**
     *  Represent this object in atomic seconds. Atomic seconds are an
     *  intermediate form of seconds which does not take into account
     *  leap seconds added to International Atomic Time (TAI) to get
     *  coordinated universal time (UTC).
     *
     *  @return the time in this object in CE seconds.
     */
    virtual uint64_t toAtomicSeconds() const;

    /**
     *  Represent this object in CE seconds and return the fractional
     *  part in nanoseconds. This can differ from simply getting the
     *  nanoseconds, since a value of nanoseconds larger than 999999999
     *  will be normalized into the return value in seconds. (This will
     *  only be the case if this object was initialized with an
     *  unnormalized nanoseconds value.)
     *
     *  @param  nd      refers to a variable into which the fractional
     *                  part in nanoseconds will be returned.
     *
     *  @return the time in this object in CE seconds.
     */
    virtual seconds_t toSeconds(uint32_t& nd) const;

    /**
     *  Represent this object in CE seconds.
     *
     *  @return the time in this object in CE seconds.
     */
    virtual seconds_t toSeconds() const;

    /**
     *  Re-initialize this object using platform ticks.
     *
     *  @param  tk      is the time in platform ticks.
     */
    virtual void fromTicks(ticks_t tk);

    /**
     *  Re-initialize this object using the current time.
     */
    virtual void fromNow();

    /**
     *  Compares two Common Era dates and times.
     *
     *  @param  that    refers to another Common Era object.
     *
     *  @return <0 if (this < that), 0 of (this==that), >0 if (this>that).
     */
    int compare(const CommonEra& that) const;

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

};

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the CommonEra unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestCommonEra(void);
#endif


#endif
