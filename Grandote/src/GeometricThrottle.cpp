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
 *  Implements the GeometricThrottle class.
 *
 *  @see    GeometricThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/GeometricThrottle.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
GeometricThrottle::GeometricThrottle() :
    Throttle(),
    consecutive(1),
    countdown(1),
    alarmed(false),
    consecutive1(1),
    countdown1(1),
    alarmed1(false)
{
}


//
//  Destructor.
//
GeometricThrottle::~GeometricThrottle() {
}


//
//  Reset this throttle to its just constructed state.
//
void GeometricThrottle::reset() {
    this->reset(0);
}


//
//  Reset this throttle to its just constructed state.
//
void GeometricThrottle::reset(ticks_t /* ticks */) {
    this->consecutive = this->consecutive1 = 1;
    this->countdown = this->countdown1 = 1;
    this->alarmed = this->alarmed1 = false;
}


//
//  Evaluate this throttle and return true if the attempt
//  is admissible at this time. This throttle decays
//  geometrically, so it pretty quickly goes from admitting
//  pretty often to almost never.
//
ticks_t GeometricThrottle::admissible() {
    return admissible(0);
}


//
//  Evaluate this throttle and return true if the attempt
//  is admissible at this time. This throttle decays
//  geometrically, so it pretty quickly goes from admitting
//  pretty often to almost never.
//
ticks_t GeometricThrottle::admissible(ticks_t /* ticks */) {
    ticks_t delay = 0;

    do {

        this->countdown1 = this->countdown;
        this->consecutive1 = this->consecutive;

        if (this->countdown1 > 0) {
            --this->countdown1;
            if (0 == this->countdown1) {
                this->consecutive1 <<= 1;
                this->countdown1 = this->consecutive1 - this->consecutive;
                this->alarmed1 = false;
                break;
            }
        }

        this->alarmed1 = true;
        delay = intmaxof(ticks_t);

    } while (false);

    return delay;
}


//
//  Commit the new throttle state.
//
bool GeometricThrottle::commit(size_t /* n */) {
    this->alarmed = this->alarmed1;
    return this->rollback();
}


//
//  Roll back the throttle state.
//
bool GeometricThrottle::rollback() {
    this->countdown = this->countdown1;
    this->consecutive = this->consecutive1;
    return !this->alarmed;
}


//
//  Get the current time in ticks.
//
ticks_t GeometricThrottle::time() {
    return 0;
}


//
//  Get the throttle frequency in ticks per second.
//
ticks_t GeometricThrottle::frequency() {
    return 0;
}


//
//  Return the alarm state of this throttle.
//
bool GeometricThrottle::isAlarmed() const {
    return this->alarmed;
}


//
//  Return the approximate state of this throttle.
//
bool GeometricThrottle::isApproximate() const {
    return false;
}


//
//  Show this object on the output object.
//
void GeometricThrottle::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Throttle::show(level, display, indent + 1);
    printf("%s consecutive=%u\n", sp, this->consecutive);
    printf("%s countdown=%u\n", sp, this->countdown);
    printf("%s alarmed=%d\n", sp, this->alarmed);
    printf("%s consecutive1=%u\n", sp, this->consecutive1);
    printf("%s countdown1=%u\n", sp, this->countdown1);
    printf("%s alarmed1=%d\n", sp, this->alarmed1);
}


#include "com/diag/desperado/End.h"
