#ifndef _COM_DIAG_DESPERADO_GEOMETRICTHROTTLE_H_
#define _COM_DIAG_DESPERADO_GEOMETRICTHROTTLE_H_

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
 *  Declares the GeometricThrottle class.
 *
 *  @see    GeometricThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Throttle.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a throttle that decays geometrically by admitting
 *  the first time it is called, then the second time, then the
 *  fourth time, then the eight time, and so on. After thirty-two
 *  admissions, it never admits again unless it is reset, at which
 *  point the process starts all over again. This throttle requires
 *  4,294,967,296 admission attempts to get to the thirty-second
 *  admission.
 *
 *  This is not a time-based throttle.
 *
 *  Geometric throttles are good for controlling the rate of error
 *  messages which lose importance quickly the longer the problem they
 *  are complaining about goes unrectified. Geometrically throttled
 *  messages communicate a sense of urgency early, but later only offer
 *  an occasional reminder. A good tactic is to reset the throttle when
 *  the problem goes away so that the next occurrence starts fresh.
 *
 *  Geometric throttles carry very little state and are computationally
 *  very efficient.
 *
 *  @see    Throttle
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class GeometricThrottle : public Throttle {

public:

    /**
     *  Constructor.
     */
    explicit GeometricThrottle();

    /**
     *  Destructor.
     */
    virtual ~GeometricThrottle();

    /**
     *  Resets the throttle to its just-constructed state. This may
     *  allow traffic contract violations to occur since subsequent
     *  admission decisions will be based on the reinitialized state.
     */
    virtual void reset();

    /**
     *  Resets the throttle to its just-constructed state. This may
     *  allow traffic contract violations to occur since subsequent
     *  admission decisions will be based on the reinitialized state.
     *  Gets the current time in ticks as a parameter rather than by
     *  calling the time method.
     *
     *  @param  ticks           is the current time in ticks. This
     *                          parameter has no effect on this throttle.
     */
    virtual void reset(ticks_t ticks);

    /**
     *  Returns the number of ticks, in units indicated by the frequency
     *  of the throttle, that the event (or the first event, if multiple
     *  events are to be emitted)  must be delayed in order to be
     *  admissible. If the event is immediately admissible, zero is returned.
     *  If the throttle is not a time-based throttle, the maximum possible
     *  number of ticks is returned.
     *
     *  @return true if the request is admissible at this time, false
     *          otherwise.
     */
    virtual ticks_t admissible();

    /**
     *  Returns the number of ticks, in units indicated by the frequency
     *  of the throttle, that the event (or the first event, if multiple
     *  events are to be emitted)  must be delayed in order to be
     *  admissible. If the event is immediately admissible, zero is returned.
     *  If the throttle is not a time-based throttle, the maximum possible
     *  number of ticks is returned. Gets the current time in ticks as a
     *  parameter rather than by calling the time method.
     *
     *  @param  ticks           is the current time in ticks. This parameter
     *                          has no effect on this throttle.
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
     *  state is updated appropriately. The number of emitted events
     *  has no effect on this throttle's behavior; only the first event
     *  is throttled.
     *
     *  @param n            is the number of events being emitted. This
     *                      parameter has no effect on this throttle.
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
     *  by the throttle frequency. Since this throttle is not a
     *  time-based throttle, this value is undefined.
     *
     *  @return the current time in ticks or undefined if not a
     *  time-based throttle.
     */
    virtual ticks_t time();

    /**
     *  Returns the frequency of the throttle. This is the number
     *  of ticks per second implemented by the throttle. Since this
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
     *  Total number of consecutive attempts before next admission.
     */
    unsigned int consecutive;

    /**
     *  Incremental number of consecutive attempts before next admission.
     */
    unsigned int countdown;

    /**
     *  True if alarmed, false otherwise.
     */
    bool alarmed;

    /**
     *  Candidate consecutive number of consecutive attempts.
     */
    unsigned int consecutive1;

    /**
     *  Candidate incremental number of consecutive attempts.
     */
    unsigned int countdown1;

    /**
     *  Candidate alarming state, not yet committed.
     */
    bool alarmed1;

};

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the GeometricThrottle unit test. This may
 *  take a while on a typical system.
 *
 *  @param  pVM     points to the Ficl VM in whose context this
 *                  function is being executed if being run from a Ficl
 *                  shell, null otherwise.
 *
 *  @param  limit   is the number of admissions attempts to make. If
 *                  this parameter is less than zero, the full unit
 *                  test is run, which can take hours, depending on
 *                  the speed of the host processor. (But it is a good
 *                  idea to run it at least once to validate a new
 *                  port of this code.)
 *
 *  @return the number of errors detected by the unit test/
 */
CXXCAPI int unittestGeometricThrottle(void* pVM, int limit);
#endif


#endif
