/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Throttle class.
 *
 *  @see    Throttle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Throttle.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
Throttle::Throttle() {
}


//
//  Destructor.
//
Throttle::~Throttle() {
}


//
//  Reset this throttle to its just constructed state.
//
void Throttle::reset() {
    this->reset(this->time());
}


//
//  Reset this throttle to its just constructed state.
//
void Throttle::reset(ticks_t /* ticks */) {
}


//
//  Return true if this throttle is admissible, false otherwise.
//
ticks_t Throttle::admissible() {
    return this->admissible(this->time());
}


//
//  Return true if this throttle is admissible, false otherwise.
//
ticks_t Throttle::admissible(ticks_t /* ticks */) {
    return 0;
}


//
//  Commit the new throttle state with one event.
//
bool Throttle::commit() {
    return this->commit(1);
}


//
//  Commit the new throttle state with the specified number of events.
//
bool Throttle::commit(size_t /* n */) {
    return true;
}


//
//  Roll back the new throttle state.
//
bool Throttle::rollback() {
    return true;
}


//
//  Get the current time in ticks.
//
ticks_t Throttle::time() {
    return 0;
}


//
//  Get the throttle frequency in ticks per second.
//
ticks_t Throttle::frequency() {
    return 0;
}


//
//  Return the alarm state of this throttle.
//
bool Throttle::isAlarmed() const {
    return false;
}


//
//  Return the approximate state of this throttle.
//
bool Throttle::isApproximate() const {
    return false;
}


//
//  Show this object on the output object.
//
void Throttle::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


#include "com/diag/desperado/End.h"
