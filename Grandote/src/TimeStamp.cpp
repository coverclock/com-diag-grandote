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
 *  Implements the TimeStamp class.
 *
 *  @see    TimeStamp
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/string.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/assert.h"
#include "com/diag/desperado/TimeStamp.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
TimeStamp::TimeStamp() {
    this->buffer[0] = '\0';
}


//
//  Destructor.
//
TimeStamp::~TimeStamp() {
}


const char* TimeStamp::iso8601() {
    CommonEra ce;
    ce.fromNow();
    return this->iso8601(ce);
}


const char* TimeStamp::iso8601(const CommonEra& ce) {
    ::snprintf(this->buffer, sizeof(this->buffer),
            "%04llu-%02u-%02uT%02u:%02u:%02uZ",
            ce.getYear(), ce.getMonth(), ce.getDay(),
            ce.getHour(), ce.getMinute(), ce.getSecond());
    this->buffer[sizeof(this->buffer) - 1] = '\0';
    assert(std::strlen(this->buffer) < sizeof(this->buffer));
    return this->buffer;
}


const char* TimeStamp::iso8601(const LocalTime& lt) {
    int oo = lt.getOffset() + (lt.getDst() ? TimeZone::s_per_h : 0);
    unsigned int aa = (0 > oo) ? -oo : oo;
    char sign = (0 > oo) ? '-' : '+';
    unsigned int hh = aa / Constant::s_per_h;
    unsigned int mm = (aa / Constant::s_per_min) - (hh * Constant::min_per_h);
    ::snprintf(this->buffer, sizeof(this->buffer),
            "%04llu-%02u-%02uT%02u:%02u:%02u%c%02u:%02u",
            lt.getYear(), lt.getMonth(), lt.getDay(),
            lt.getHour(), lt.getMinute(), lt.getSecond(),
            sign, hh, mm);
    this->buffer[sizeof(this->buffer) - 1] = '\0';
    assert(std::strlen(this->buffer) < sizeof(this->buffer));
    return this->buffer;
}


const char* TimeStamp::highprecision() {
    CommonEra ce;
    ce.fromNow();
    return this->highprecision(ce);
}


const char* TimeStamp::highprecision(const CommonEra& ce) {
    LocalTime lt(0);
    lt.fromCommonEra(ce);
    return this->highprecision(lt);
}


const char* TimeStamp::highprecision(const LocalTime& lt) {
    int oo = lt.getOffset() + (lt.getDst() ? TimeZone::s_per_h : 0);
    unsigned int aa = (0 > oo) ? -oo : oo;
    char sign = (0 > oo) ? '-' : '+';
    unsigned int hh = aa / Constant::s_per_h;
    unsigned int mm = (aa / Constant::s_per_min) - (hh * Constant::min_per_h);
    unsigned int ss = aa - (hh * Constant::s_per_h) -
        (mm * Constant::s_per_min);
    ::snprintf(this->buffer, sizeof(this->buffer),
            "%04llu-%02u-%02uT%02u:%02u:%02u.%09u%c%02u:%02u:%02u",
            lt.getYear(), lt.getMonth(), lt.getDay(),
            lt.getHour(), lt.getMinute(), lt.getSecond(),
            lt.getNanosecond(),
            sign, hh, mm, ss);
    this->buffer[sizeof(this->buffer) - 1] = '\0';
    assert(std::strlen(this->buffer) < sizeof(this->buffer));
    return this->buffer;
}


const char* TimeStamp::milspec() {
    CommonEra ce;
    ce.fromNow();
    return this->milspec(ce);
}


const char* TimeStamp::milspec(const CommonEra& ce) {
    LocalTime lt(0);
    lt.fromCommonEra(ce);
    return this->milspec(lt);
}


const char* TimeStamp::milspec(const LocalTime& lt) {
    TimeZone zone;
    ::snprintf(this->buffer, sizeof(this->buffer),
        "%04llu-%3.3s-%02u %02u:%02u:%02u%1.1s",
        lt.getYear(), lt.getDate().monthToString(), lt.getDay(),
        lt.getHour(), lt.getMinute(), lt.getSecond(),
        zone.milspec(lt.getOffset()));
    this->buffer[sizeof(this->buffer) - 1] = '\0';
    assert(std::strlen(this->buffer) < sizeof(this->buffer));
    return this->buffer;
}


const char* TimeStamp::civilian() {
    CommonEra ce;
    ce.fromNow();
    return this->civilian(ce);
}


const char* TimeStamp::civilian(const CommonEra& ce) {
    LocalTime lt(0);
    lt.fromCommonEra(ce);
    return this->civilian(lt);
}


const char* TimeStamp::civilian(const LocalTime& lt) {
    TimeZone zone;
    const char* timezone = zone.civilian(lt.getOffset());
    char effective[4];
    std::strncpy(effective, timezone, sizeof(effective));
    if (lt.getDst() && (3 <= std::strlen(effective))) {
        effective[1] = 'D';
    }
    ::snprintf(this->buffer, sizeof(this->buffer),
        "%04llu-%02u-%02u %02u:%02u:%02u %s",
        lt.getYear(), lt.getMonth(), lt.getDay(),
        lt.getHour(), lt.getMinute(), lt.getSecond(),
        effective);
    this->buffer[sizeof(this->buffer) - 1] = '\0';
    assert(std::strlen(this->buffer) < sizeof(this->buffer));
    return this->buffer;
}


const char* TimeStamp::log() {
    CommonEra ce;
    ce.fromNow();
    return this->log(ce);
}


const char* TimeStamp::log(const CommonEra& ce) {
    LocalTime lt(0);
    lt.fromCommonEra(ce);
    return this->log(lt);
}

// Log line space is at a premium. It pays to have as high a precision timestamp
// as the underlying platform supports, but no more. Generally about a
// microsecond is the best we can hope for on Linux platforms. But I've used
// vxWorks platforms that had time bases accurate to tens of nanoseconds. So
// consider altering the print format from ".%06u" to ".%09u" and removing the
// "/ 1000" nanosecond divisor.
const char* TimeStamp::log(const LocalTime& lt) {
    TimeZone zone;
    ::snprintf(this->buffer, sizeof(this->buffer),
        "%04llu-%02u-%02u %02u:%02u:%02u.%06u%1.1s",
        lt.getYear(), lt.getMonth(), lt.getDay(),
        lt.getHour(), lt.getMinute(), lt.getSecond(),
        lt.getNanosecond() / 1000,
        zone.milspec(lt.getOffset()));
    this->buffer[sizeof(this->buffer) - 1] = '\0';
    assert(std::strlen(this->buffer) < sizeof(this->buffer));
    return this->buffer;
}


const char* TimeStamp::formal() {
    CommonEra ce;
    ce.fromNow();
    return this->formal(ce);
}


const char* TimeStamp::formal(const CommonEra& ce) {
    LocalTime lt(0);
    lt.fromCommonEra(ce);
    return this->formal(lt);
}


const char* TimeStamp::formal(const LocalTime& lt) {
    const Date& date = lt.getDate();
    const Time& time = lt.getTime();
    TimeZone zone;
    const char* timezone = zone.civilian(lt.getOffset());
    char effective[4];
    std::strncpy(effective, timezone, sizeof(effective));
    if (lt.getDst() && (3 <= std::strlen(effective))) {
        effective[1] = 'D';
    }
    uint8_t twelve;
    const char* meridiem = time.meridiemToString(time.oclock(twelve));
    ::snprintf(this->buffer, sizeof(this->buffer),
        "%s, %s %u, %04llu, %u:%02u %s %s",
        date.weekdayToString(), date.monthToString(), lt.getDay(), lt.getYear(),
        twelve, lt.getMinute(), meridiem,
        effective);
    this->buffer[sizeof(this->buffer) - 1] = '\0';
    assert(std::strlen(this->buffer) < sizeof(this->buffer));
    return this->buffer;
}


//
//  Show this object on the output object.
//
void TimeStamp::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s buffer[%lu]=\"%.*s\"[%lu]\n",
            sp, sizeof(this->buffer), sizeof(this->buffer),
            this->buffer, ::strnlen(this->buffer, sizeof(this->buffer)));
}


#include "com/diag/desperado/End.h"
