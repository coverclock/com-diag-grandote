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
 *  Implements the Linux class.
 *
 *  @see    Linux
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#if defined(DESPERADO_PLATFORM_IS_Linux) || defined(DESPERADO_PLATFORM_IS_Diminuto) || defined(DESPERADO_PLATFORM_IS_Arroyo)


#include "com/diag/desperado/stdarg.h"
#include "com/diag/desperado/stdlib.h"
#include <ctime>
#include <syslog.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Linux.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/Ticks.h"
#include "com/diag/desperado/TimeZone.h"
#include "com/diag/desperado/DstNever.h"
#include "com/diag/desperado/DstUs.h"
#include "com/diag/desperado/LeapSeconds.h"
#include "com/diag/desperado/Constant.h"
#include "com/diag/desperado/generics.h"

extern long timezone;
extern int daylight;

#include "com/diag/desperado/Begin.h"


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
//  This would work for Linux but perhaps not other embedded platforms.
//
Linux::Linux() :
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

    struct rlimit limit;
    if (-1 != ::getrlimit(RLIMIT_CORE, &limit)) {
        limit.rlim_cur = limit.rlim_max;
        ::setrlimit(RLIMIT_CORE, &limit);
    }

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

    //  Set the epoch to the fixed value of the Linux epoch,
    //  expressed in ISO8601 as 1970-01-01T00:00:00.000000000.
    //  The use of atomic seconds here is to prevent the need
    //  of the Common Era object to access the Linux object
    //  while we are still in its constructor. The use of atomic
    //  seconds is possible because the first leap second was
    //  not until after the Linux Epoch.

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
Linux::~Linux() {
}


//
//  Return the name of this platform.
//
const char* Linux::platform() {
    return DESPERADO_PLATFORM_NAME;
}


//
//  Return the name of the target on which this platform is running.
//
const char* Linux::target() {
    return DESPERADO_TARGET_NAME;
}


//
//  Return the name of the host on which this platform is running.
//
const char* Linux::host() {
    return hostnamebuffer;
}


//
//  Return the resolution of the Linux clock in ticks per second as a ratio.
//
void Linux::frequency(ticks_t& numerator, ticks_t& denominator) {
    numerator = Constant::us_per_s;
    denominator = 1;
}


//
//  Return the time of day in ticks since the epoch.
//
ticks_t Linux::time() {
    const ticks_t ticks_per_s = this->frequency();
    struct timeval time;
    ticks_t ticks = 0;

    if (0 <= ::gettimeofday(&time, 0)) {
        ticks = time.tv_sec * ticks_per_s;
        ticks += (time.tv_usec * ticks_per_s) / Constant::us_per_s;
    }

    return ticks;
}


//
//  Return the elapsed time in ticks since construction.
//
ticks_t Linux::elapsed() {
    return this->time() - this->birthdate;
}


//
//  Yield the processor for at least the specified number of ticks.
//  This method is guaranteed to relinquish the processor for at least
//  one context switch. Otherwise, a failure in this function could
//  cause an application to consume the entire processor.
//
ticks_t Linux::yield(ticks_t ticks, bool premature) {
    const ticks_t origin = this->time();
    int rc = 0;

    //  Yield the processor. This method guarantees that we will do at
    //  least one context switch if there is anyone else eligible to run.

    sched_yield();

    //  If we haven't waited long enough, calculate the remaining time
    //  and sleep for that duration.

    ticks_t elapsed = this->time() - origin;

    if (elapsed < ticks) {

        const ticks_t ticks_per_s = this->frequency();
        const ticks_t s_maximum = signedintmaxof(time_t);
        const ticks_t ns_limit = Constant::ns_per_s - 1;
        const ticks_t ns_per_tick = Constant::ns_per_s / ticks_per_s;
        const ticks_t ns_maximum = ns_limit - (ns_limit % ns_per_tick);
        const ticks_t ns_maximum_ticks = ns_maximum / ns_per_tick;
        ticks_t remaining = ticks - elapsed;
        ticks_t delay;
        struct timespec req;
        struct timespec rem;

        //  Repeat if ticks remaining and no error.

        do {

            //  timespec.tv_sec is of type time_t, and time_t is of type
            //  long int, hence cannot encode more than 0x7fffffff seconds
            //  in rem.tv_sec.

            delay = remaining / ticks_per_s;
            if (s_maximum < delay) {
                delay = s_maximum;
            }
            remaining -= delay * ticks_per_s;
            rem.tv_sec = delay;

            //  Cannot encode more than 999,999,999 nanoseconds in
            //  timespec.tv_nsec according to nanosleep(2). In
            //  addition, must keep rem.tv_nsec to an even number
            //  of ticks represented in nanoseconds, so that
            //  the delay represents an integral number of ticks
            //  to subtract from remaining.

            if (ns_maximum_ticks <= remaining) {
                delay = ns_maximum;
                remaining -= ns_maximum_ticks;
            } else {
                delay = remaining * ns_per_tick;
                remaining = 0;
            }
            rem.tv_nsec = delay;

            // Repeat if interrupted with time remaining and not premature.

            do {
                req = rem;
                rc = ::nanosleep(&req, &rem);
            } while ((0 > rc) && (EINTR == errno) && (!premature));

        } while ((0 < remaining) && (0 == rc) && (!premature));

    }
    
    //  Nominally there should be zero ticks remaining unless nanosleep()
    //  failed somehow.

    elapsed = time() - origin;

    if (!premature) {
        while (elapsed < ticks) {
            rc = sched_yield();
            elapsed = time() - origin;
            if (0 != rc) {
                break;
            }
        }
    }

    return elapsed;
}


//
//  Construct and return the identity of the caller.
//
identity_t Linux::identity() {
    pid_t pid = ::getpid();
    pthread_t self = pthread_self();
    identity_t identity = pid;
    return (identity << widthof(self)) | self;
}


//
//  Print some diagnostics and abort producing a core dump.
//  We rely on the logger we are using to emit a timestamp
//  and an identifier.
//
void Linux::fatal(
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
    std::abort();
    std::exit(255); // Should never get here.
}


//
//  Return a reference to the standard input functor.
//
Input& Linux::input() {
    return this->inputs;
}


//
//  Return a reference to the standard output functor.
//
Output& Linux::output() {
    return this->outputs;
}


//
//  Return a reference to the standard error functor.
//
Output& Linux::error() {
    return this->errors;
}


//
//  Return a reference to the log output functor.
//
Output& Linux::log() {
    return this->logs;
}


//
//  Return a reference to the dump object.
//
Dump& Linux::dump() {
    return this->dumps;
}


//
//  Return a reference to the heap object.
//
Heap& Linux::heap() {
    return this->heaps;
}


//
//  Return a reference to the print object.
//
Print& Linux::print() {
    return this->prints;
}


//
//  Return a reference to the logger object.
//
Logger& Linux::logger() {
    return this->loggers;
}


//
//  Extract the component name from a path and store it in a
//  buffer of a specified size.
//
char* Linux::component(const char* path, char* buffer, size_t size) const {
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
int* Linux::errornumber() {
    return __errno_location();
}


//
//  Returns the maximum errno value. Not the most efficient mechanism
//  in the world. Should be called just once upon initialization.
//  Would be simpler just to hard code it; there does not appear to
//  be a portable mechanism to determine this value.
//
int Linux::errormaximum() {
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
const char* Linux::errormessage(int errndx, char* buffer, size_t buflen) {
    return ::strerror_r(errndx, buffer, buflen);
}


//
//  Show this object on the output object.
//
void Linux::show(int level, Output* display, int indent) const {
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


#include "com/diag/desperado/End.h"


#endif
