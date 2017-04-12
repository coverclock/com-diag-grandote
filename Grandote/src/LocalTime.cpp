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
 *  Implements the LocalTime class.
 *
 *  @see    LocalTime
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/LocalTime.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/DstNever.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


static DstNever dstnever;


//
//  Constructor.
//
LocalTime::LocalTime() :
    DateTime(),
    offset(this->offunset),
    rule(0),
    dst(false)
{
} 


//
//  Constructor.
//
LocalTime::LocalTime(int32_t ot) :
    DateTime(),
    rule(&dstnever),
    dst(false)
{
    TimeZone zone;
    this->offset = zone.normalize(ot);
}


//
//  Constructor.
//
LocalTime::LocalTime(int32_t ot, DaylightSavingTime& re) :
    DateTime(),
    rule(&re)
{
    TimeZone zone;
    this->offset = zone.normalize(ot);
    this->dst = (*(this->rule))(*this);
}


//
//  Destructor.
//
LocalTime::~LocalTime() {
}


//
//  Convert this object to a string.
//
size_t LocalTime::toString(String string, size_t size) const {
    DateTime::String datetimebuffer;
    this->DateTime::toString(datetimebuffer);
    int32_t ot = this->getOffset();
    uint32_t effective = (0 > ot) ? -ot : ot;
    int octets = ::snprintf(string, size, "%s%c%u%s",
        datetimebuffer,
        (0 > ot) ? '-' : '+', effective,
        this->dst ? "DST" : "");
    return ((0 <= octets) && (octets < static_cast<int>(size)))
        ? static_cast<size_t>(octets)
        : (0 < size) ? (size - 1) : 0;
}


//
//  Return true if valid, false otherwise.
//
bool LocalTime::isValid() const {
    TimeZone zone;
    int32_t ot = this->getOffset();
    return this->DateTime::isValid() && zone.isValid(ot);
}


//
//  Reintialize this object using epoch seconds.
//
void LocalTime::fromSeconds(seconds_t sd, uint32_t nd) {
    sd += nd / Constant::ns_per_s;
    nd %= Constant::ns_per_s;
    int32_t ot = this->getOffset();
    sd += ot;
    CommonEra ce(0);
    ce.fromSeconds(sd, nd);
    DaylightSavingTime& re = this->getDaylightSavingTime();
    this->dst = re(ce);
    if (this->dst) {
        sd += Constant::s_per_h;
    }
    ce.fromSeconds(sd, nd);
    this->setYear(ce.getYear());
    this->setMonth(ce.getMonth());
    this->setDay(ce.getDay());
    this->setHour(ce.getHour());
    this->setMinute(ce.getMinute());
    this->setSecond(ce.getSecond());
    this->setNanosecond(ce.getNanosecond());
}


//
//  Reinitialize this object using platform ticks.
//
void LocalTime::fromTicks(ticks_t tk) {
    CommonEra ce(0);
    ce.fromTicks(tk);
    this->fromCommonEra(ce);
}


//
//  Reinitialize this object using a Common Era object.
//
void LocalTime::fromCommonEra(const CommonEra& ce) {
    uint32_t nanoseconds;
    seconds_t seconds = ce.toSeconds(nanoseconds);
    this->fromSeconds(seconds, nanoseconds);
}


//
//  Set this common era object from the current time.
//
void LocalTime::fromNow() {
    this->fromTicks(Platform::instance().time());
}


//
//  Compare two objects. Note that we can't compare the
//  rules, since two different rules might be equivalent.
//
int LocalTime::compare(const LocalTime& that) const {
    int rc = 0;
    int32_t ot = this->getOffset();
    int32_t thatot = that.getOffset();
    bool dt = this->getDst();
    bool thatdt = that.getDst();
    if (ot < thatot) {
        rc = -1;
    } else if (ot > thatot) {
        rc = 1;
    } else if (dt && (!thatdt)) {
        rc = -1;
    } else if ((!dt) && thatdt) {
        rc = 1;
    } else {
        rc = this->DateTime::compare(that);
    }
    return rc;
}


//
//  Show this object on the output object.
//
void LocalTime::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->DateTime::show(level, display, indent + 1);
    printf("%s offset=%d\n", sp, this->offset);
    printf("%s rule=%p\n", sp, this->rule);
    if (0 != this->rule) {
        this->rule->show(level, display, indent + 2);
    }
    printf("%s dst=%d\n", sp, this->dst);
}


#include "com/diag/desperado/End.h"
