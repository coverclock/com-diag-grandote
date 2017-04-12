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
 *  Implements the Time class.
 *
 *  @see    Time
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Time.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


static const char* MERIDIEMS[] = {
    "AM",
    "PM",
    ""
};


//
//  Constructor.
//
Time::Time(
    uint8_t hr,
    uint8_t me,
    uint8_t sd,
    uint32_t nd
) :
    nanosecond(nd % Constant::ns_per_s),
    hour(hr),
    minute(me),
    second(sd + (nd / Constant::ns_per_s))
{
}


//
//  Destructor.
//
Time::~Time() {
}


//
//  Convert to a string. The return value from snprintf() depends
//  on the version of glibc; we accomodate either semantic.
//
size_t Time::toString(String string, size_t size) const {
    int octets = ::snprintf(string, size, "%02u:%02u:%02u.%09u",
        this->hour, this->minute, this->second, this->nanosecond);
    return ((0 <= octets) && (octets < static_cast<int>(size)))
        ? static_cast<size_t>(octets)
        : (0 < size) ? (size - 1) : 0;
}


//
//  Returns true if this object is valid, false otherwise.
//
bool Time::isValid() const {
    return
        (Constant::h_per_d > this->hour)
    &&
        (Constant::min_per_h > this->minute)
    &&
        (Constant::s_per_min > this->second)
    &&
        (Constant::ns_per_s > this->nanosecond)
    ;
}


//
//  Return the name of the meridiem.
//
const char* Time::meridiemToString(Meridiem mm) const {
    static const int limit = countof(MERIDIEMS);
    return MERIDIEMS[((0 <= mm) && (mm < limit)) ? mm : limit - 1];
}

//
//  Return AM for A.M., PM for P.M.
//
Time::Meridiem Time::oclock(uint8_t hr, uint8_t& twelve) const {
    Meridiem rc = (12 > hr) ? AM : PM;
    if (0 == hr) {
        twelve = 12;
    } else if (hr <= 12) {
        twelve = hr;
    } else {
        twelve = hr - 12;
    }
    return rc;
}


//
//  Compare two objects.
//
int Time::compare(const Time& that) const {
    int rc = 0;
    if (this->hour < that.hour) {
        rc = -1;
    } else if (this->hour > that.hour) {
        rc = 1;
    } else if (this->minute < that.minute) {
        rc = -1;
    } else if (this->minute > that.minute) {
        rc = 1;
    } else if (this->second < that.second) {
        rc = -1;
    } else if (this->second > that.second) {
        rc = 1;
    } else if (this->nanosecond < that.nanosecond) {
        rc = -1;
    } else if (this->nanosecond > that.nanosecond) {
        rc = 1;
    }
    return rc;
}


//
//  Show this object on the output object.
//
void Time::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s hour=%u\n", sp, this->hour);
    printf("%s minute=%u\n", sp, this->minute);
    printf("%s second=%u\n", sp, this->second);
    printf("%s nanosecond=%u\n", sp, this->nanosecond);
    if (0 < level) {
        printf("%s isValid()=%d\n", sp, this->isValid());
        uint8_t twelve;
        Time::Meridiem ampm = this->oclock(twelve);
        printf("%s oclock(%u)=%d\n", sp, twelve, ampm);
    }
}


#include "com/diag/desperado/End.h"
