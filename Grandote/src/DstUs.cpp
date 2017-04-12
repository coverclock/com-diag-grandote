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
 *  Implements the DstUs class.
 *
 *  @see    DstUs
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/DstUs.h"
#include "com/diag/desperado/DstNever.h"
#include "com/diag/desperado/DstUs1966.h"
#include "com/diag/desperado/DstUs1986.h"
#include "com/diag/desperado/DstUs2007.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


static DstNever dstnever;
static DstUs1966 dstus1966;
static DstUs1986 dstus1986;
static DstUs2007 dstus2007;


//
//  Destructor.
//
DstUs::~DstUs() {
}


//
//  Return true if the date and time fall within the DST
//  rule, false otherwise.
//
bool DstUs::operator() (const DateTime& dt) const {
    DaylightSavingTime* rule = 0;
    uint64_t year = dt.getYear();
    
    if (year < 1966) {
        rule = &dstnever;
    } else if (year < 1986) {
        rule = &dstus1966;
    } else if (year < 2007) {
        rule = &dstus1986;
    } else {
        rule = &dstus2007;
    }

    return (*rule)(dt);
}


//
//  Show this object on the output object.
//
void DstUs::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->DaylightSavingTime::show(level, display, indent + 1);
    printf("%s dstnever:\n", sp);
    dstnever.show(level, display, indent + 2);
    printf("%s dstus1966:\n", sp);
    dstus1966.show(level, display, indent + 2);
    printf("%s dstus1986:\n", sp);
    dstus1986.show(level, display, indent + 2);
    printf("%s dstus2007:\n", sp);
    dstus2007.show(level, display, indent + 2);
}


#include "com/diag/desperado/End.h"
