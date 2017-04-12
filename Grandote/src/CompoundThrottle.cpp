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
 *  Implements the CompoundThrottle class.
 *
 *  @see    CompoundThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */



#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/CompoundThrottle.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"


static Throttle promiscuous;


//
//  Constructor.
//
CompoundThrottle::CompoundThrottle() :
    Throttle(),
    peak(&promiscuous),
    sustained(&promiscuous)
{
}


//
//  Constructor.
//
CompoundThrottle::CompoundThrottle(Throttle& pp) :
    Throttle(),
    peak(&pp),
    sustained(&promiscuous)
{
}


//
//  Constructor.
//
CompoundThrottle::CompoundThrottle(Throttle& pp, Throttle& ss) :
    Throttle(),
    peak(&pp),
    sustained(&ss)
{
}


//
//  Destructor.
//
CompoundThrottle::~CompoundThrottle() {
}


//
//  Reset this throttle to its just constructed state. It is assumed that
//  a request is immediately admissible in the newly constructed state.
//
void CompoundThrottle::reset(ticks_t ticks) {
    this->peak->reset(ticks);
    this->sustained->reset(ticks);
}


//
//  Evaluate this throttle.
//
ticks_t CompoundThrottle::admissible(ticks_t ticks) {
    ticks_t peakadmissible = this->peak->admissible(ticks);
    ticks_t sustainedadmissible = this->sustained->admissible(ticks);
    return (peakadmissible > sustainedadmissible) ?
        peakadmissible : sustainedadmissible;
}


//
//  Commit the new throttle state.
//
bool CompoundThrottle::commit(size_t n) {
    bool peakcommit = this->peak->commit(n);
    bool sustainedcommit = this->sustained->commit(n);
    return peakcommit && sustainedcommit;
}


//
//  Roll back the throttle state.
// 
bool CompoundThrottle::rollback() {
    bool peakrollback = this->peak->rollback();
    bool sustainedrollback = this->sustained->rollback();
    return peakrollback && sustainedrollback;
}


//
//  Return the time of the peak throttle.
//
ticks_t CompoundThrottle::time() {
    return this->peak->time();
}


//
//  Return the frequency of the peak throttle.
//
ticks_t CompoundThrottle::frequency() {
    return this->peak->frequency();
}


//
//  Return the alarm state of this throttle.
//
bool CompoundThrottle::isAlarmed() const {
    bool peakalarmed = this->peak->isAlarmed();
    bool sustainedalarmed = this->sustained->isAlarmed();
    return peakalarmed || sustainedalarmed;
}


//
//  Return the approximate state of this throttle.
//
bool CompoundThrottle::isApproximate() const {
    bool peakapproximate = this->peak->isApproximate();
    bool sustainedapproximate = this->sustained->isApproximate();
    return peakapproximate || sustainedapproximate;
}


//
//  Show this object on the output object.
//
void CompoundThrottle::show(
    int level,
    Output* display,
    int indent
) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Throttle::show(level, display, indent + 1);
    printf("%s peak:\n", sp);
    this->peak->show(level, display, indent + 2);
    printf("%s sustained:\n", sp);
    this->sustained->show(level, display, indent + 2);
}


#include "com/diag/desperado/End.h"
