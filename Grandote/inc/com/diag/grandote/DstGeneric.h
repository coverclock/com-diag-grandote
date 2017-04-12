#ifndef _COM_DIAG_DESPERADO_DSTGENERIC_H_
#define _COM_DIAG_DESPERADO_DSTGENERIC_H_

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
 *  Declares the DstGeneric class.
 *
 *  @see    DstGeneric
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/DaylightSavingTime.h"
#include "com/diag/desperado/Date.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Encodes a rule to decide whether a given DateTime falls within
 *  daylight saving time (DST) depending on parameters provided
 *  during construction, serving as the base class for most other
 *  daylight saving time rules. The events that indicate the
 *  beginning and ending of daylight saving time may be expressed
 *  in terms of an ordinal weekday (e.g. FIRST SUNDAY) of a
 *  month (e.g. OCTOBER) and an hour. Daylight saving time
 *  rules which cannot be expressed in these terms must be implemented
 *  as a new class derived from the base class of this class. So far,
 *  the standard rules for DST in the US and the EU can be expressed
 *  this way. US and EU DST rules are all derived from this class.
 *  The class supports both Northern Hemisphere ("spring forward, fall
 *  back") and Southern Hemisphere ("spring back, fall forward") rules.
 *  For example, an Australian DST rule could be derived from this class
 *  in a manner identical to the US and EU rules.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class DstGeneric : public DaylightSavingTime {

public:

    /**
     *  Describes an event at which daylight saving time begins or ends.
     */
    struct Event {

        /**
         *  A date weekday ordinal, typically FIRST, SECOND or LAST.
         */
        Date::Ordinal ordinal;

        /**
         *  A weekday, typically SUNDAY.
         */
        Date::Weekday weekday;

        /**
         *  A month.
         */
        Date::Month month;

        /**
         *  An hour, typically 1 (for 01.00) in the EU, 2 (for 2AM) in the US.
         */
        unsigned int hour;
    };

    /**
     *  Constructor.
     */
    explicit DstGeneric();

    /**
     *  Constructor.
     *
     *  @param  bs      is the beginning event of DST.
     *
     *  @param  es      is the ending event of DST. The month of the
     *                  ending event can be greater (as for the Northern
     *                  hemisphere) or less (as for the Southern
     *                  hemisphere) than the month of the beginning event.
     */
    explicit DstGeneric(const Event& bs, const Event& es);

    /**
     *  Destructor.
     */
    virtual ~DstGeneric();

    /**
     *  Return true if the specified date and time fall within
     *  the daylight saving time rule, false otherwise.
     *
     *  @param  dt          refers to a date and time object.
     *
     *  @return true if the specified date and time fall within
     *          daylight savings time, false otherwise.
     */
    virtual bool operator() (
        const DateTime& dt
    ) const;

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
     *  This event begins daylight saving time.
     */
    Event begins;

    /**
     *  This event ends daylight saving time.
     */
    Event ends;

};


//
//  Constructor.
//
inline DstGeneric::DstGeneric() {
}


//
//  Constructor.
//
inline DstGeneric::DstGeneric(const Event& bs, const Event& es) :
    begins(bs),
    ends(es)
{
}

#include "com/diag/desperado/End.h"


#endif
