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
 *  Implements the Ticks class.
 *
 *  @see    Ticks
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/types.h"
#include "com/diag/desperado/Ticks.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Destructor.
//
Ticks::~Ticks() {
}


//
//  Convert platform ticks into seconds and nanoseconds.
//
void Ticks::seconds(
    ticks_t ticks,
    uint64_t& seconds,
    uint32_t& nanoseconds
) {
    ticks_t frequency = Platform::instance().frequency();
    seconds = ticks / frequency;
    ticks %= frequency;
    ticks *= Constant::ns_per_s;
    ticks /= frequency;
    nanoseconds = static_cast<uint32_t>(ticks);
}


//
//  Convert platform ticks into days, hours, minutes, seconds, and
//  nanoseconds.
void Ticks::elapsed(
    ticks_t ticks,
    unsigned int& days,
    unsigned int& hours,
    unsigned int& minutes,
    unsigned int& seconds,
    uint32_t& nanoseconds
) {
    ticks_t frequency = Platform::instance().frequency();
    ticks_t divisor = frequency * Constant::s_per_d;
    days = ticks / divisor;
    ticks %= divisor;
    divisor = frequency * Constant::s_per_h;
    hours = ticks / divisor;
    ticks %= divisor;
    divisor = frequency * Constant::s_per_min;
    minutes = ticks / divisor;
    ticks %= divisor;
    seconds = ticks / frequency;
    ticks %= frequency;
    ticks *= Constant::ns_per_s;
    ticks /= frequency;
    nanoseconds = static_cast<uint32_t>(ticks);
}


//
//  Show this object on the output object.
//
void Ticks::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


#include "com/diag/desperado/End.h"
