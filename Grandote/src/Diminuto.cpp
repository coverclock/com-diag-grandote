/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2017 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Diminuto class.
 *
 *  @see    Diminuto
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "platform.h"
#include "target.h"


#if defined(GRANDOTE_PLATFORM_IS_Diminuto)


#include "com/diag/grandote/cxxcapi.h"


CXXCAPI {
#include "com/diag/diminuto/diminuto_core.h"
#include "com/diag/diminuto/diminuto_frequency.h"
#include "com/diag/diminuto/diminuto_time.h"
#include "com/diag/diminuto/diminuto_delay.h"
}

#include "com/diag/grandote/stdarg.h"
#include "com/diag/grandote/stdlib.h"
#include <ctime>
#include <syslog.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include "com/diag/grandote/errno.h"
#include "com/diag/grandote/types.h"
#include "com/diag/grandote/string.h"
#include "com/diag/grandote/Diminuto.h"
#include "com/diag/grandote/CommonEra.h"
#include "com/diag/grandote/Ticks.h"
#include "com/diag/grandote/TimeZone.h"
#include "com/diag/grandote/DstNever.h"
#include "com/diag/grandote/DstUs.h"
#include "com/diag/grandote/LeapSeconds.h"
#include "com/diag/grandote/Constant.h"
#include "com/diag/grandote/generics.h"

extern long timezone;
extern int daylight;

#include "com/diag/grandote/Begin.h"


static DstNever dstnever;           	// Used if no Daylight Saving Time.
static DstUs dstus;                 	// Used assuming U.S. rules.
static LeapSeconds leapsecondsrule;     // Used as the leap second rule.
static char hostnamebuffer[64];     	// Used for gethostname().


static char* hostname() {
    int rc = ::gethostname(hostnamebuffer, sizeof(hostnamebuffer));
    if (0 != rc) {
        hostnamebuffer[0] = '\0';
    }
    hostnamebuffer[sizeof(hostnamebuffer) - 1] = '\0';
    return hostnamebuffer;
}


//
//  Constructor.
//
//  You might consider passing argc and argv to the constructor of a
//  derived class from the main and have it parse command line parameters.
//  This would work for Diminuto but perhaps not other embedded platforms.
//
Diminuto::Diminuto() :
    Platform(),
    inputs(stdin),
    outputs(stdout),
    errors(stderr),
    logs(hostname()),
    dumps(this->errors),
    heaps(this->errors),
    prints(this->errors),
    loggers(this->logs)
{
    //  Allow unlimited core dump sizes. This isn't a good idea
    //  for an embedded application, but it is useful for debugging
    //  on a simulated target.

    diminuto_core_enable();

    //  Unbuffer standard error. This is typically the default and is useful
    //  for debugging. If you are having problems debugging, especially with
    //  correlating error messages with output in time, first try redirecting
    //  standard output to standard error on the command line.

    std::setbuf(stderr, 0);

    //  Indicate that system ticks do not incorporate leap seconds,
    //  as per time(2) and POSIX.1 Annex B 2.2.2, and that the built-in
    //  leap seconds rule is used.

    this->setLeapSecondTicks(false);
    this->setLeapSeconds(leapsecondsrule);

    //  Set the zone to match that of the underlying system configuration.
    //  This is just a guess, useful to facilitate testing. Applications
    //  using this package should set the time zone and DST rule explicitly.

    tzset();
    this->setOffset(-timezone);
    DaylightSavingTime* dst = &dstnever;
    if (daylight) { dst = &dstus; }
    this->setDaylightSavingTime(*dst);

    //  Set the epoch to the fixed value of the Diminuto epoch,
    //  expressed in ISO8601 as 1970-01-01T00:00:00.000000000.
    //  The use of atomic seconds here is to prevent the need
    //  of the Common Era object to access the Diminuto object
    //  while we are still in its constructor. The use of atomic
    //  seconds is possible because the first leap second was
    //  not until after the Diminuto Epoch.

    CommonEra ce(1970, 1, 1, 0, 0, 0, 0);
    seconds_t seconds = ce.toAtomicSeconds();
    Epoch epoch(seconds, 0);
    this->setEpoch(epoch);

    //  Record the time of construction.

    this->birthdate = this->time();
}


//
//  Destructor.
//
Diminuto::~Diminuto() {
}


//
//  Return the name of this platform.
//
const char* Diminuto::platform() {
    return GRANDOTE_PLATFORM_NAME;
}


//
//  Return the name of the target on which this platform is running.
//
const char* Diminuto::target() {
    return GRANDOTE_TARGET_NAME;
}


//
//  Return the name of the host on which this platform is running.
//
const char* Diminuto::host() {
    return hostnamebuffer;
}


//
//  Return the resolution of the Diminuto clock in ticks per second as a ratio.
//
void Diminuto::frequency(ticks_t& numerator, ticks_t& denominator) {
    numerator = diminuto_frequency();
    denominator = 1;
}


//
//  Return the time of day in ticks since the epoch.
//
ticks_t Diminuto::time() {
    return diminuto_time_clock();
}


//
//  Return the elapsed time in ticks since construction.
//
ticks_t Diminuto::elapsed() {
    return this->time() - this->birthdate;
}


//
//  Yield the processor for at least the specified number of ticks.
//  This method is guaranteed to relinquish the processor for at least
//  one context switch. Otherwise, a failure in this function could
//  cause an application to consume the entire processor.
//
ticks_t Diminuto::yield(ticks_t ticks, bool premature) {
    ticks_t was = diminuto_time_elapsed();
    if (ticks == 0) {
        diminuto_yield();
    } else if (premature) {
        diminuto_delay_interruptible(ticks);
    } else {
        diminuto_delay_uninterruptible(ticks);
    }
    ticks_t now = diminuto_time_elapsed();
    return now - was;
}


//
//  Construct and return the identity of the caller.
//
identity_t Diminuto::identity() {
    return pthread_self();
}


//
//  Print some diagnostics and abort producing a core dump.
//  We rely on the logger we are using to emit a timestamp
//  and an identifier.
//
void Diminuto::fatal(
    const char* event,
    int error,
    const char* file,
    int line,
    const char* function
) {
    static const size_t maximum = 255;
    int index = (0 <= error) ? error : errno;
    Logger& log = this->logger();
    if (0 != event) {
        log.fatal("EVENT: \"%.*s\"!\n",
            ::strnlen(event, maximum), event);
    }
    if (0 != file) {
        log.fatal("LOCATION: %.*s[%d]!\n",
            ::strnlen(file, maximum), file, line);
    }
    if (0 != function) {
        log.fatal("FUNCTION: %.*s!\n",
            ::strnlen(function, maximum), function);
    }
    if (0 < index) {
        const char* error = std::strerror(index);
        log.fatal("ERROR: \"%.*s\"[%d]!\n",
            ::strnlen(error, maximum), error, index);
    }
    (log.output())();
    diminuto_core_fatal();
}


//
//  Return a reference to the standard input functor.
//
Input& Diminuto::input() {
    return this->inputs;
}


//
//  Return a reference to the standard output functor.
//
Output& Diminuto::output() {
    return this->outputs;
}


//
//  Return a reference to the standard error functor.
//
Output& Diminuto::error() {
    return this->errors;
}


//
//  Return a reference to the log output functor.
//
Output& Diminuto::log() {
    return this->logs;
}


//
//  Return a reference to the dump object.
//
Dump& Diminuto::dump() {
    return this->dumps;
}


//
//  Return a reference to the heap object.
//
Heap& Diminuto::heap() {
    return this->heaps;
}


//
//  Return a reference to the print object.
//
Print& Diminuto::print() {
    return this->prints;
}


//
//  Return a reference to the logger object.
//
Logger& Diminuto::logger() {
    return this->loggers;
}


//
//  Extract the component name from a path and store it in a
//  buffer of a specified size.
//
char* Diminuto::component(const char* path, char* buffer, size_t size) const {
    if (path && buffer && size) {
        const char* here = path;
        const char* beginning;
        for (beginning = here; *here; ++here) {
            if ('/' == (*here)) {
                beginning = here + 1;
            }
        }
        const char* ending;
        for (ending = here; here != path; --here) {
            if (('.' == (*here)) && ('.' != (*(here + 1))) && (*(here + 1))) {
                ending = here;
                break;
            }
        }
        char* there = buffer;
        for (here = beginning; (size > 1) && (here < ending); --size) {
            *(there++) = *(here++);
        }
        *there = '\0';
    }
    return buffer;
}


//
//  Returns a pointer to the errno variable.
//
int* Diminuto::errornumber() {
    return __errno_location();
}


//
//  Returns the maximum errno value. Not the most efficient mechanism
//  in the world. Should be called just once upon initialization.
//  Would be simpler just to hard code it; there does not appear to
//  be a portable mechanism to determine this value.
//
int Diminuto::errormaximum() {
    int errmax = 1000; // Really arbitrary value.
    char buffer[80];
    char* string;
    int rc;
    const char* comparison = "Unknown error ";
    for (int errndx = errmax; errndx > 0; --errndx) {
        string = ::strerror_r(errndx, buffer, sizeof(buffer));
        rc = std::strncmp(string, comparison, sizeof(comparison) - 1);
        if (0 != rc) {
            errmax = errndx;
            break;
        }
    }
    return errmax;
}


//
//  Returns an error message.
//
const char* Diminuto::errormessage(int errndx, char* buffer, size_t buflen) {
    return ::strerror_r(errndx, buffer, buflen);
}


//
//  Show this object on the output object.
//
void Diminuto::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Platform::show(level, display, indent + 1);
    printf("%s birthdate=%llu\n", sp, this->birthdate);
    printf("%s inputs:\n", sp);
    this->inputs.show(level, display, indent + 2);
    printf("%s outputs:\n", sp);
    this->outputs.show(level, display, indent + 2);
    printf("%s errors:\n", sp);
    this->errors.show(level, display, indent + 2);
    printf("%s logs:\n", sp);
    this->logs.show(level, display, indent + 2);
    printf("%s dumps:\n", sp);
    this->dumps.show(level, display, indent + 2);
    printf("%s heaps:\n", sp);
    this->heaps.show(level, display, indent + 2);
    printf("%s prints:\n", sp);
    this->prints.show(level, display, indent + 2);
    printf("%s loggers:\n", sp);
    this->loggers.show(level, display, indent + 2);
}


#include "com/diag/grandote/End.h"


#endif
