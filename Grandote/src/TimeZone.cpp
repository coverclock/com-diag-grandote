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
 *  Implements the TimeZone class.
 *
 *  @see    TimeZone
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"


#include "com/diag/desperado/Begin.h"


struct Zone {
    int32_t     seconds;
    const char* name;
};


#define MILITARY(_SYMBOL_) \
    { \
        (TimeZone::Seconds::_SYMBOL_), \
        (#_SYMBOL_) \
    }

static const Zone MILSPEC[] = {
    MILITARY(YANKEE),
    MILITARY(XRAY),
    MILITARY(WHISKY),
    MILITARY(VICTOR),
    MILITARY(UNIFORM),
    MILITARY(TANGO),
    MILITARY(SIERRA),
    MILITARY(ROMEO),
    MILITARY(QUEBEC),
    MILITARY(PAPA),
    MILITARY(OSCAR),
    MILITARY(NOVEMBER),
    MILITARY(ZULU),
    MILITARY(ALPHA),
    MILITARY(BRAVO),
    MILITARY(CHARLIE),
    MILITARY(DELTA),
    MILITARY(ECHO),
    MILITARY(FOXTROT),
    MILITARY(GOLF),
    MILITARY(HOTEL),
    MILITARY(INDIA),
    MILITARY(KILO),
    MILITARY(LIMA),
    MILITARY(MIKE)
};


//
//  Destructor.
//
TimeZone::~TimeZone() {
}


//
//  Return the milspec time zone name.
//
const char* TimeZone::milspec(int32_t ot) const {
    int32_t offset = this->normalize(ot);
    static const Zone otherwise = { 0, "JULIET" };
    const Zone* found = &otherwise;
    for (size_t ii = 0; ii < countof(MILSPEC); ++ii) {
        if (MILSPEC[ii].seconds == offset) {
            found = &(MILSPEC[ii]);
            break;
        }
    }
    return found->name;
}


#define CIVILIAN(_SYMBOL_) \
    { \
        (TimeZone::Seconds::_SYMBOL_), \
        (#_SYMBOL_), \
    }

static const Zone CIVILIAN[] = {
    CIVILIAN(MIT),
    CIVILIAN(HST),
    CIVILIAN(AST),
    CIVILIAN(PST),
    CIVILIAN(MST),
    CIVILIAN(CST),
    CIVILIAN(EST),
    CIVILIAN(PRT),
    CIVILIAN(CNT),
    CIVILIAN(AGT),
    CIVILIAN(BET),
    CIVILIAN(CAT),
    CIVILIAN(UTC),
    CIVILIAN(ECT),
    CIVILIAN(ART),
    CIVILIAN(EAT),
    CIVILIAN(MET),
    CIVILIAN(NET),
    CIVILIAN(PLT),
    CIVILIAN(IST),
    CIVILIAN(BST),
    CIVILIAN(VST),
    CIVILIAN(CTT),
    CIVILIAN(JST),
    CIVILIAN(ACT),
    CIVILIAN(AET),
    CIVILIAN(SST),
    CIVILIAN(NST)
};

//
//  Return the civilian time zone name.
//
const char* TimeZone::civilian(int32_t ot) const {
    int32_t offset = this->normalize(ot);
    static const Zone otherwise = { 0, "LCT" };
    const Zone* found = &otherwise;
    for (size_t ii = 0; ii < countof(CIVILIAN); ++ii) {
        if (CIVILIAN[ii].seconds == offset) {
            found = &(CIVILIAN[ii]);
            break;
        }
    }
    return found->name;
}


//
//  Show this object on the output object.
//
void TimeZone::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


#include "com/diag/desperado/End.h"
