/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
    This file is part of the Digital Aggregates Grandote library.
    
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
 *  Implements the Platform class.
 *
 *  @see    Platform
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "platform.h"
#include "com/diag/grandote/stdarg.h"
#include "com/diag/grandote/littleendian.h"
#include "com/diag/grandote/lowtohigh.h"
#include "com/diag/grandote/Platform.h"
#include "com/diag/grandote/CriticalSection.h"
#include "com/diag/grandote/Vintage.h"
#include "com/diag/grandote/Ticks.h"
#include "com/diag/grandote/TimeZone.h"
#include "com/diag/grandote/DstNever.h"
#include "com/diag/grandote/LeapSeconds.h"
#include "com/diag/grandote/Constant.h"
#include "com/diag/grandote/cxxcapi.h"


#if defined(GRANDOTE_PLATFORM_IS_Diminuto)
#   include "com/diag/grandote/Diminuto.h"
typedef CXXCTYPE(::com::diag::grandote::, Diminuto) OperatingSystem;
#elif defined(GRANDOTE_PLATFORM_IS_Linux)
#   include "com/diag/grandote/Linux.h"
typedef CXXCTYPE(::com::diag::grandote::, Linux) OperatingSystem;
#   elif defined(GRANDOTE_PLATFORM_IS_Cygwin)
#   include "com/diag/grandote/Cygwin.h"
typedef CXXCTYPE(::com::diag::grandote::, Cygwin) OperatingSystem;
#else
#   error GRANDOTE_PLATFORM_IS_* not defined!
#endif


namespace com { namespace diag { namespace grandote {

static Mutex mutex;

//
//  Allocate and construct a suitable Platform object and return a
//  reference to it.
//
Platform& Platform::factory() {
    return *(new OperatingSystem);
}


//
//  This points to the initial default platform object.
//
static Platform* instant = 0;


//
//	This points to the current system platform object. An application may
//	create multiple platform objects, but there is only one current system
//	platform object. It is initially the default platform object created during
//	static initialization. But the application can set it to be any other
//	platform object. When this happens, the new platform object is not taken;
//	it is up to the application to manage its memory.
//
Platform* Platform::singleton = 0;


//
//  This forces at least one Vintage object to be linked into any
//  application that uses Platform, allowing the ident(1) command
//  to be used against the resulting application.
//
static Vintage vintage;


//
//  This is the default Daylight Saving Time rule.
//
static DstNever dstneverrule;


//
//  This is the default Leap Seconds rule.
//
static LeapSeconds leapsecondsrule;


//
//  Set the system platform.
//
void Platform::instance(Platform& that) {
   Platform::singleton = &that;
}


//
//  Get the system platform.
//
Platform& Platform::instance() {
	CriticalSection guard(mutex);
	if (singleton == 0) {
		delete instant;
		instant = singleton = &(factory());
	}
    return *singleton;
}


//
//  Constructor.
//
Platform::Platform() :
    leapseconds(false),
    lsrule(&leapsecondsrule),
    epoch(),
    offset(0L),
    dstrule(&dstneverrule)
{
}


//
//  Destructor.
//
Platform::~Platform() {
}


//
//  Return the platform frequency as a whole number.
//
ticks_t Platform::frequency() {
    ticks_t numerator;
    ticks_t denominator;
    this->frequency(numerator, denominator);
    return numerator / denominator;
}


//
//  Show this object on the output object.
//
void Platform::show(int level, Output* display, int indent) const {
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char buffer[sizeof(__FILE__)];
    const char* bp = this->component(__FILE__, buffer, sizeof(buffer));
    printf("%s%s(%p)[%lu]:\n", sp, bp, this, sizeof(*this));
    printf("%s singleton=%p\n", sp, this->singleton);
    printf("%s leapseconds=%d\n", sp, this->leapseconds);
    if (lsrule) {
        printf("%s lsrule:\n", sp);
        this->lsrule->show(level, display, indent + 2);
    } else {
        printf("%s lsrule=%p\n", sp, this->lsrule);
    }
    printf("%s epoch:\n", sp);
    this->epoch.show(level, display, indent + 2);
    printf("%s offset=%ld\n", sp, this->offset);
    if (dstrule) {
        printf("%s dstrule:\n", sp);
        this->dstrule->show(level, display, indent + 2);
    } else {
        printf("%s dstrule=%p\n", sp, this->dstrule);
    }
}


} } }
