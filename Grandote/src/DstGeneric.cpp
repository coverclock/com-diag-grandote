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
 *  Implements the DstGeneric class.
 *
 *  @see    DstGeneric
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/DstGeneric.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Destructor.
//
DstGeneric::~DstGeneric() {
}


//
//  Return true if the date and time fall within the DST
//  rule, false otherwise. Complicated by the fact that in
//  the Northern Hemisphere DST "springs forward and falls
//  back", but in the Southern Hemisphere DST "springs back
//  and falls forward".
//
bool DstGeneric::operator() (const DateTime& dt) const {

    bool rc;
    const Event* first;
    const Event* last;

    if (this->begins.month <= this->ends.month) {
        // Northern Hemisphere
        rc = false;
        first = &(this->begins);
        last = &(this->ends);
    } else {
        // Southern Hemisphere
        rc = true;
        first = &(this->ends);
        last = &(this->begins);
    }

    do {

        uint8_t month = dt.getMonth();

        if (!((first->month <= month) && (month <= last->month))) {
            break;
        }      

        if (first->month == month) {
            uint8_t begin = dt.ordinal(first->ordinal, first->weekday);
            uint8_t day = dt.getDay();
            uint8_t hour = dt.getHour();
            if ((day < begin) || ((day == begin) && (hour < first->hour))) {
                break;
            }
        }

        if (last->month == month) {
            uint8_t end = dt.ordinal(last->ordinal, last->weekday);
            uint8_t day = dt.getDay();
            uint8_t hour = dt.getHour();
            if ((day > end) || ((day == end) && (hour >= last->hour))) {
                break;
            }
        }

        rc = !rc;

    } while (false);

    return rc;
}


//
//  Show this object on the output object.
//
void DstGeneric::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    Date date;
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->DaylightSavingTime::show(level, display, indent + 1);
    printf("%s begins:\n", sp);
    printf("%s  ordinal=%d=%s\n",
        sp, this->begins.ordinal, date.ordinalToString(this->begins.ordinal));
    printf("%s  weekday=%d=%s\n",
        sp, this->begins.weekday, date.weekdayToString(this->begins.weekday));
    printf("%s  month=%d=%s\n",
        sp, this->begins.month, date.monthToString(this->begins.month));
    printf("%s  hour=%u=%02u:00:00\n",
        sp, this->begins.hour, this->begins.hour);
    printf("%s ends:\n", sp);
    printf("%s  ordinal=%d=%s\n",
        sp, this->ends.ordinal, date.ordinalToString(this->ends.ordinal));
    printf("%s  weekday=%d=%s\n",
        sp, this->ends.weekday, date.weekdayToString(this->ends.weekday));
    printf("%s  month=%d=%s\n",
        sp, this->ends.month, date.monthToString(this->ends.month));
    printf("%s  hour=%u=%02u:00:00\n",
        sp, this->ends.hour, this->ends.hour);
}


#include "com/diag/desperado/End.h"
