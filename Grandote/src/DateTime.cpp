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
 *  Implements the DateTime class.
 *
 *  @see    DateTime
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/DateTime.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor.
//
DateTime::DateTime(
    uint64_t yr,
    uint8_t mh,
    uint8_t dy,
    uint8_t hr,
    uint8_t me,
    uint8_t sd,
    uint32_t nd
) :
    date(yr, mh, dy),
    time(hr, me, sd, nd)
{
}


//
//  Constructor.
//
DateTime::DateTime(
    const Date& de,
    const Time& te
) :
    date(de),
    time(te)
{
}


//
//  Destructor.
//
DateTime::~DateTime() {
}


//
//  Convert this object into a string.
//
size_t DateTime::toString(String string, size_t size) const {
    Date::String datebuffer;
    Time::String timebuffer;

    this->date.toString(datebuffer, sizeof(datebuffer));
    this->time.toString(timebuffer, sizeof(timebuffer));

    int octets = ::snprintf(string, size, "%sT%s", datebuffer, timebuffer);

    return ((0 <= octets) && (octets < static_cast<int>(size)))
        ? static_cast<size_t>(octets)
        : (0 < size) ? (size - 1) : 0;
}


//
//  Return true if valid, false otherwise.
//
bool DateTime::isValid() const {
    return this->date.isValid() && this->time.isValid();
}


//
//  Compare two objects.
//
int DateTime::compare(const DateTime& that) const {
    int rc = this->date.compare(that.getDate());
    if (rc == 0) {
        rc = this->time.compare(that.getTime());
    }
    return rc;
}


//
//  Show this object on the output object.
//
void DateTime::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s date:\n", sp);
    this->date.show(level, display, indent + 2);
    printf("%s time:\n", sp);
    this->time.show(level, display, indent + 2);
}


#include "com/diag/desperado/End.h"
