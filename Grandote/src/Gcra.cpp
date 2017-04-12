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
 *  Implements the Gcra class.
 *
 *  @see    Gcra
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */



#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Gcra.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"

//
//  Constructor.
//
Gcra::Gcra() :
    Throttle()
{
    this->i = 0;
    this->l = intmaxof(ticks_t);
    this->max = intmaxof(ticks_t);
    this->reset(0);
}


//
//  Constructor.
//
Gcra::Gcra(ticks_t increment, ticks_t limit) :
    Throttle()
{
    this->i = increment;
    this->l = limit;
    if (increment == 0) {
        this->max = intmaxof(ticks_t);
    } else {
        this->max = intmaxof(ticks_t) / increment;
    }
    this->reset();
}


//
//  Destructor.
//
Gcra::~Gcra() {
}



//
//  Reset this throttle to its just constructed state. It is assumed that
//  a request is immediately admissible in the newly constructed state.
//
void Gcra::reset(ticks_t ticks) {
    this->x = 0;
    this->x1 = 0;
    this->now = ticks;
    this->then = ticks - this->i;
    this->alarmed = false;
    this->alarmed1 = false;
    this->approximate = false;
}


//
//  Evaluate this throttle and return true if the attempt
//  is admissible, false otherwise. This throttle enforces
//  a traffic contract using a Generic Cell Rate Algorithm
//  (GCRA) as defined in ATM Forum Traffic Management 4.0.
//
ticks_t Gcra::admissible(ticks_t ticks) {
    ticks_t delay = 0;
    this->alarmed1 = false;
    this->now = ticks;
    ticks_t elapsed = this->now - this->then;
    if (this->x <= elapsed) {
        this->x1 = 0;
    } else {
       this->x1 = this->x - elapsed;
       if (this->x1 > this->l) {
           delay = this->x1 - this->l;
           this->alarmed1 = true;
       }
    }
    return delay;
}


//
//  Commit the new throttle state.
//
bool Gcra::commit(size_t n) {
    this->then = this->now;
    if (n > max) {
        this->x = intmaxof(ticks_t);
        approximate = true;
    } else {
        ticks_t increment = n * this->i;
        ticks_t maximum = intmaxof(ticks_t) - increment;
        if (this->x1 > maximum) {
            this->x = intmaxof(ticks_t);
            approximate = true;
        } else {
            this->x = this->x1 + (n * this->i);
        }
    }
    this->alarmed = this->alarmed1;
    return !this->alarmed;
}


//
//  Roll back the throttle state.
//
bool Gcra::rollback() {
    return !this->alarmed;
}


//
//  Return the time.
//
ticks_t Gcra::time() {
    return Platform::instance().time();
}


//
//  Return the frequency.
//
ticks_t Gcra::frequency() {
    return Platform::instance().frequency();
}


//
//  Return the alarm state of this throttle.
//
bool Gcra::isAlarmed() const {
    return this->alarmed;
}


//
//  Return the approximation state of this throttle.
//
bool Gcra::isApproximate() const {
    return this->approximate;
}


//
//  Show this object on the output object.
//
void Gcra::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Throttle::show(level, display, indent + 1);
    printf("%s then=%llu\n", sp, this->then);
    printf("%s now=%llu=+%llu\n", sp, this->now, this->now - this->then);
    printf("%s i=%llu\n", sp, this->i);
    printf("%s l=%llu\n", sp, this->l);
    printf("%s x=%llu\n", sp, this->x);
    printf("%s x1=%llu\n", sp, this->x1);
    printf("%s alarmed=%d\n", sp, this->alarmed);
    printf("%s alarmed1=%d\n", sp, this->alarmed1);
    printf("%s approximate=%d\n", sp, this->approximate);
}


#include "com/diag/desperado/End.h"
