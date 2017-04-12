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
 *  Implements the CellRateThrottle class.
 *
 *  @see    CellRateThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/CellRateThrottle.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"

//
//  Compute the peak increment.
//
ticks_t CellRateThrottle::increment(
    uint32_t pcr,
    uint32_t /* cdvt */
) {
    Gcra gcra;
    ticks_t hz = gcra.frequency();
    ticks_t increment = (0 < pcr) ? (hz + pcr - 1) / pcr : intmaxof(ticks_t);
    return increment;
}


//
//  Compute the peak limit.
//
ticks_t CellRateThrottle::limit(
    uint32_t /* pcr */,
    uint32_t cdvt
) {
    Gcra gcra;
    ticks_t hz = gcra.frequency();
    ticks_t limit = (hz * cdvt) / Constant::us_per_s;
    return limit;
}


//
//  Compute the sustainable increment.
//
ticks_t CellRateThrottle::increment(
   uint32_t /* pcr */,
   uint32_t /* cdvt */,
   uint32_t scr,
   uint32_t /* mbs */
) {
    Gcra gcra;
    ticks_t hz = gcra.frequency();
    ticks_t increment = (0 < scr) ? (hz + scr - 1) / scr : intmaxof(ticks_t);
    return increment;
}


//
//  Compute the sustainable limit.
//
ticks_t CellRateThrottle::limit(
   uint32_t pcr,
   uint32_t cdvt,
   uint32_t scr,
   uint32_t mbs
) {
    ticks_t limit = CellRateThrottle::limit(pcr, cdvt);
    ticks_t pincrement = CellRateThrottle::increment(pcr, cdvt);
    ticks_t sincrement = CellRateThrottle::increment(pcr, cdvt, scr, mbs);
    if ((1 < mbs) && (0 < pincrement) && (pincrement < sincrement)) {
        limit += (mbs - 1) * (sincrement - pincrement);
    }
    return limit;
}


//
//  Constructor.
//
CellRateThrottle::CellRateThrottle() :
    CompoundThrottle(),
    peak(),
    sustained()
{
}


//
//  Constructor.
//
CellRateThrottle::CellRateThrottle(
    uint32_t pcr,
    uint32_t cdvt
) :
    CompoundThrottle(peak),
    peak(
        this->increment(pcr, cdvt),
        this->limit(pcr, cdvt)
    )
{
    this->reset();
}


//
//  Constructor.
//
CellRateThrottle::CellRateThrottle(
    uint32_t pcr,
    uint32_t cdvt,
    uint32_t scr,
    uint32_t mbs
) :
    CompoundThrottle(peak, sustained),
    peak(
        this->increment(pcr, cdvt),
        this->limit(pcr, cdvt)
    ),
    sustained(
        this->increment(pcr, cdvt, scr, mbs),
        this->limit(pcr, cdvt, scr, mbs)
    )
{
    this->reset();
}



//
//  Destructor.
//
CellRateThrottle::~CellRateThrottle() {
}


//
//  Show this object on the output object.
//
void CellRateThrottle::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->CompoundThrottle::show(level, display, indent + 1);
    printf("%s peak:\n", sp);
    this->peak.show(level, display, indent + 2);
    printf("%s sustained:\n", sp);
    this->sustained.show(level, display, indent + 2);
}


#include "com/diag/desperado/End.h"
