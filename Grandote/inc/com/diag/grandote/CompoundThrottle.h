#ifndef _COM_DIAG_DESPERADO_COMPOUNDTHROTTLE_H_
#define _COM_DIAG_DESPERADO_COMPOUNDTHROTTLE_H_

/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2007-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Declares the CompoundThrottle class.
 *
 *  @see    CompoundThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Throttle.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Creates a throttle by combining zero to two throttles.
 *
 *  @see    Throttle
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class CompoundThrottle : public Throttle {

public:

    /**
     *  Constructor.
     */
    explicit CompoundThrottle();

    /**
     *  Constructor.
     * 
     *  @param  pp          typically refers to a peak rate throttle.
     */
    explicit CompoundThrottle(Throttle& pp);

    /**
     *  Constructor.
     * 
     *  @param  pp          typically refers to a peak rate throttle.
     *
     *  @param  ss          typically refers to a sustained rate throttle.
     */
    explicit CompoundThrottle(Throttle& pp, Throttle& ss);

    /**
     *  Destructor.
     */
    virtual ~CompoundThrottle();

    using Throttle::reset;

    /**
     *  Resets the throttle to its just-constructed state. This may
     *  allow traffic contract violations to occur since subsequent
     *  admission decisions will be based on the reinitialized state.
     *  Gets the current time in ticks as a parameter rather than by
     *  calling the time method.
     *
     *  @param  ticks           is the current time in ticks.
     */
    virtual void reset(ticks_t ticks);

    using Throttle::admissible;

    /**
     *  Returns the number of ticks, in units indicated by the frequency
     *  of the throttle, that the event (or the first event, if multiple
     *  events are to be emitted)  must be delayed in order to be
     *  admissible. If the event is immediately admissible, zero is returned.
     *  If the throttle is not a time-based throttle, the maximum possible
     *  number of ticks is returned. Gets the current time in ticks as a
     *  parameter rather than by calling the time method.
     *
     *  @param  ticks           is the current time in ticks.
     *
     *  @return true if the request is admissible at this time, false
     *          otherwise.
     */
    virtual ticks_t admissible(ticks_t ticks);

    using Throttle::commit;

    /**
     *  Commits the update to the throttle state computed by the
     *  call to admissible(). Should be called when the event is
     *  emitted, whether or not it was admissible. Allows the specification
     *  of multiple events to be emitted. If the event size is greater
     *  than one, all events are assumed to be emitted and the throttle
     *  state is updated appropriately.
     *
     *  @param n            is the number of events being emitted.
     *
     *  @return true if the throttle is not alarmed, false otherwise.
     */
    virtual bool commit(size_t n);

    /**
     *  Rolls back the update to the throttle state computed by the
     *  call to admissible(). Should be called when the event is not
     *  emitted, whether or not it was admissible. 
     *
     *  @return true if the throttle is not alarmed, false otherwise.
     */
    virtual bool rollback();

    /**
     *  Returns the current time in the units of ticks indicated
     *  by the throttle frequency. If the throttle is not a
     *  time-based throttle, this value is undefined.
     *
     *  @return the current time in ticks or undefined if not a
     *  time-based throttle.
     */
    virtual ticks_t time();

    /**
     *  Returns the frequency of the throttle. This is the number
     *  of ticks per second implemented by the throttle. If the
     *  throttle is not a time-based throttle, zero is returned.
     *
     *  @return the current time in ticks per second or zero if not a
     *  time-based throttle.
     */
    virtual ticks_t frequency();

    /**
     *  Returns true if the throttle is alarmed, false otherwise.
     *  Throttles alarm when an inadmissible event is emitted and
     *  the resulting throttle state is emitted.
     *
     *  @return true if the throttle is alarmed, false otherwise.
     */
    virtual bool isAlarmed() const;

    /**
     *  Returns true if the event stream is sufficiently out of specification
     *  such that the throttle is now only approximate. This typically
     *  occurs because of integer overflow in the internal throttle state.
     *
     *  @return true if the throttle is approximate, false otherwise.
     */
    virtual bool isApproximate() const;

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
     *  Throttle for peak contract.
     */
    Throttle* peak;

    /**
     *  Throttle for sustained contract.
     */
    Throttle* sustained;

};

#include "com/diag/desperado/End.h"


#endif
