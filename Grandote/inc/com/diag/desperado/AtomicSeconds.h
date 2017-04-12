#ifndef _COM_DIAG_DESPERADO_ATOMICSECONDS_H_
#define _COM_DIAG_DESPERADO_ATOMICSECONDS_H_

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
 *  Declares the AtomicSeconds class.
 *
 *  @see    AtomicSeconds
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/types.h"
#include "com/diag/desperado/LeapSeconds.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/CommonEra.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Duplicates the interface of LeapSeconds but never returns any
 *  leap seconds. It can be used as the leap second rule by applications
 *  that only want to use Common Era dates but without taking leap
 *  seconds into account.
 *
 *  @see    LeapSeconds
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class AtomicSeconds : public LeapSeconds {

public:

    /**
     *  Constructor.
     */
    explicit AtomicSeconds();

    /**
     *  Destructor.
     */
    virtual ~AtomicSeconds();

    /**
     *  This is a short cut for applications which merely want to
     *  to know if leap seconds are observed or not. Objects of
     *  this type always return false.
     *
     *  @return true if leap seconds are observed, false otherwise.
     */
    virtual bool isObserved() const;

    /**
     *  If the provided Common Era object represents a date and time
     *  that is a leap second, this method returns a pointer to the
     *  equivalent number of Common Era seconds; otherwise, a null
     *  pointer is returned. Regardless of what is returned, the
     *  number of elapsed leap seconds that must be added to convert
     *  from atomic seconds to Common Era seconds is returned in the
     *  return result parameter.
     *
     *  @param  ce          refers to a Common Era object.
     *
     *  @param  ld          refers to a return result parameter into
     *                      which will be placed the number of leap
     *                      seconds to be added to convert from atomic
     *                      seconds to Common Era seconds. Note that
     *                      this is a signed quantity and can be
     *                      negative (although it has never occurred
     *                      that the Earth's rotation has sped up).
     *
     *  @return a pointer to the a number of seconds or null (0).
     */
    virtual const seconds_t* find(const CommonEra& ce, int& ld) const;

    /**
     *  If the provided Common Era seconds represents a date and time
     *  that is a leap second, this method returns a pointer to the
     *  equivalent Common Era object; otherwise, a null pointer is
     *  returned. Regardless of what is returned, the number of
     *  elapsed leap seconds that must be subtracted to convert
     *  from Common Era seconds to atomic seconds is returned in the
     *  return result parameter.
     *
     *  @param  sd          is a value in Common Era seconds.
     *
     *  @param  ld          refers to a return result parameter into
     *                      which will be placed the number of leap
     *                      seconds to be added to convert from atomic
     *                      seconds to Common Era seconds. Note that
     *                      this is a signed quantity and can be
     *                      negative (although it has never occurred
     *                      that the Earth's rotation has sped up).
     *
     *  @return a pointer to a Common Era object or null (0).
     */
    virtual const CommonEra* find(const seconds_t sd, int& ld) const;

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


//
//  Constructor.
//
inline AtomicSeconds::AtomicSeconds() {
}

#include "com/diag/desperado/End.h"


#endif
