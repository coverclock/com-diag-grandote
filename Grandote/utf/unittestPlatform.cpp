/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2006-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the Platform unit test.
 *
 *  @see    Platform
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/CommonEra.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Ticks.h"
#include "com/diag/desperado/Ticks.h"
#include "com/diag/desperado/Desperado.h"

CXXCAPI int unittestPlatform(void) {
    Print printf;
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    printf("platform=\"%s\"\n", Platform::instance().platform());
    printf("target=\"%s\"\n", Platform::instance().target());
    printf("host=\"%s\"\n", Platform::instance().host());
    printf("identity=0x%llx\n", Platform::instance().identity());

    printf("%s[%d]: show\n", __FILE__, __LINE__);

    Platform::instance().show();

    printf("%s[%d]: show high\n", __FILE__, __LINE__);

    Platform::instance().show(1);

    printf("%s[%d]: errno\n", __FILE__, __LINE__);

    char errnobuffer[80];

    errno = 0;
    printf("errno=%d\n", errno);
    perror("zero");
    printf("%s\n", strerror(errno));
    printf("%s\n", Platform::instance().errormessage(errno, errnobuffer,
        sizeof(errnobuffer)));

    errno = EINVAL;
    printf("errno=%d\n", errno);
    perror("EINVAL");
    printf("%s\n", strerror(errno));
    printf("%s\n", Platform::instance().errormessage(errno, errnobuffer,
        sizeof(errnobuffer)));

    errno = Platform::instance().errormaximum() + 1;
    printf("errno=%d\n", errno);
    perror("Unknown");
    printf("%s\n", strerror(errno));
    printf("%s\n", Platform::instance().errormessage(errno, errnobuffer,
        sizeof(errnobuffer)));

    (*Platform::instance().errornumber()) = 0;
    printf("(*Platform::instance().errornumber())=%d\n",
        (*Platform::instance().errornumber()));
    perror("zero");
    printf("%s\n", strerror((*Platform::instance().errornumber())));
    printf("%s\n", Platform::instance().errormessage(
        *(Platform::instance().errornumber()), errnobuffer,
        sizeof(errnobuffer)));

    (*Platform::instance().errornumber()) = EINVAL;
    printf("(*Platform::instance().errornumber())=%d\n",
        (*Platform::instance().errornumber()));
    perror("EINVAL");
    printf("%s\n", strerror((*Platform::instance().errornumber())));
    printf("%s\n", Platform::instance().errormessage(
        *(Platform::instance().errornumber()), errnobuffer,
        sizeof(errnobuffer)));

    (*Platform::instance().errornumber()) =
        Platform::instance().errormaximum() + 1;
    printf("(*Platform::instance().errornumber())=%d\n",
        (*Platform::instance().errornumber()));
    perror("Unknown");
    printf("%s\n", strerror((*Platform::instance().errornumber())));
    printf("%s\n", Platform::instance().errormessage(
        *(Platform::instance().errornumber()), errnobuffer,
        sizeof(errnobuffer)));

    (*platform_errornumber()) = 0;
    printf("(*platform_errornumber())=%d\n", (*platform_errornumber()));
    perror("zero");
    printf("%s\n", strerror((*platform_errornumber())));
    printf("%s\n", platform_errormessage(
        *platform_errornumber(), errnobuffer,
        sizeof(errnobuffer)));

    (*platform_errornumber()) = EINVAL;
    printf("(*platform_errornumber())=%d\n", (*platform_errornumber()));
    perror("EINVAL");
    printf("%s\n", strerror((*platform_errornumber())));
    printf("%s\n", platform_errormessage(
        *platform_errornumber(), errnobuffer,
        sizeof(errnobuffer)));

    (*platform_errornumber()) = Platform::instance().errormaximum() + 1;
    printf("(*platform_errornumber())=%d\n", (*platform_errornumber()));
    perror("Unknown");
    printf("%s\n", strerror((*platform_errornumber())));
    printf("%s\n", platform_errormessage(
        *platform_errornumber(), errnobuffer,
        sizeof(errnobuffer)));

    for (int ii = 0; ii <= Platform::instance().errormaximum() + 1; ++ii) {
        *Platform::instance().errornumber() = ii;
        int errndx = *Platform::instance().errornumber();
        const char* strmsg = strerror(errndx);
        const char* errmsg = Platform::instance().errormessage(errndx,
            errnobuffer, sizeof(errnobuffer));
        printf("%d=\"%s\"=\"%s\"\n", errndx, strmsg, errmsg);
    }

    printf("%s[%d]: writing to output\n", __FILE__, __LINE__);

    Print outputf(Platform::instance().output());

    outputf.show();

    (outputf.output())('o');
    (outputf.output())('u');
    (outputf.output())('t');
    (outputf.output())('p');
    (outputf.output())('u');
    (outputf.output())('t');
    (outputf.output())('c');
    (outputf.output())('\n');

    (outputf.output())("outputs\n");

    (outputf.output())("outputn\n??????????", sizeof("outputn"));

    char obuffer[] = { 'o', 'u', 't', 'p', 'u', 't', 'b', '\n' };

    (outputf.output())(obuffer, sizeof(obuffer), sizeof(obuffer));

    outputf("%s%c", "outputf", '\n');

    printf("%s[%d]: writing to error\n", __FILE__, __LINE__);

    Print errorf(Platform::instance().error());

    errorf.show(0, &(errorf.output()));

    (errorf.output())('e');
    (errorf.output())('r');
    (errorf.output())('r');
    (errorf.output())('o');
    (errorf.output())('r');
    (errorf.output())('c');
    (errorf.output())('\n');

    (errorf.output())("errors\n");

    (errorf.output())("errorn\n??????????", sizeof("errorn"));

    char ebuffer[] = { 'e', 'r', 'r', 'o', 'r', 'b', '\n' };

    (errorf.output())(ebuffer, sizeof(ebuffer), sizeof(ebuffer));

    errorf("%s%c", "errorf", '\n');

    printf("%s[%d]: writing to log\n", __FILE__, __LINE__);

    Print logf(Platform::instance().log());

    logf.show(0, &(errorf.output()));

    (logf.output())('l');
    (logf.output())('o');
    (logf.output())('g');
    (logf.output())('c');
    (logf.output())('\n');

    (logf.output())("logs\n");

    (logf.output())("logn\n??????????", sizeof("logn"));

    char lbuffer[] = { 'l', 'o', 'g', 'b', '\n' };

    (logf.output())(lbuffer, sizeof(lbuffer), sizeof(lbuffer));

    logf("%s%c", "logf", '\n');

    printf("%s[%d]: dump\n", __FILE__, __LINE__);

    unsigned long data[] = {
        0xdeadbeef,
        0xc0edbabe,
        0xcafefeed,
        0xfacefade
    };
    Platform::instance().dump().bytes(data, sizeof(data));
    Platform::instance().dump().words(data, sizeof(data));

    printf("%s[%d]: print\n", __FILE__, __LINE__);

    (Platform::instance().print())("%s%c", "(print)(error)", '\n');

    printf("%s[%d]: frequency\n", __FILE__, __LINE__);

    ticks_t freq1 = Platform::instance().frequency();

    ticks_t numerator;
    ticks_t denominator;
    Platform::instance().frequency(numerator, denominator);
    ticks_t freq2 = numerator / denominator;

    if (freq1 != freq2) {
        errorf("%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, freq1, freq2);
        ++errors;
    }

    printf("%s[%d]: frequency=(%llu/%llu)=%llu=%lluHz\n",
        __FILE__, __LINE__, numerator, denominator, freq1, freq2);

    ticks_t requested;
    ticks_t now;
    ticks_t then;
    ticks_t claimed;
    ticks_t measured;
    uint64_t seconds;
    uint32_t nanoseconds;
    Ticks ticks;

    printf("%s[%d]: time\n", __FILE__, __LINE__);

    printf("%18s %18s %20s\n",
        "CLAIMED", "MEASURED", "SECONDS");

    for (int ii = 0; ii < 10; ++ii) {

        then = Platform::instance().time();
        claimed = Platform::instance().yield();
        now = Platform::instance().time();
        measured = now - then;
        ticks.seconds(measured, seconds, nanoseconds);

        printf("0x%016llx 0x%016llx %10llu.%09lu\n",
            claimed, measured, seconds, nanoseconds);

    }

    CommonEra ce;
    CommonEra::String cefrom;
    CommonEra::String ceto;

    printf("%s[%d]: CommonEra ticks\n", __FILE__, __LINE__);

    printf("%30s %30s %11s\n",
        "FROM", "TO", "SECONDS");

    for (int ii = 0; ii < 30; ++ii) {

        then = Platform::instance().time();
        claimed = Platform::instance().yield(freq1);
        now = Platform::instance().time();
        measured = now - then;

        ce.fromTicks(then);
        ce.toString(cefrom);

        ce.fromTicks(now);
        ce.toString(ceto);

        ticks.seconds(measured, seconds, nanoseconds);

        printf("%29s %29s %llu.%09lu\n",
            cefrom, ceto, seconds, nanoseconds);

        if (claimed < freq1) {
            errorf("%s[%d]: (%llu<%llu)!\n",
                __FILE__, __LINE__, claimed, freq1);
            ++errors;
        }

        if (claimed > measured) {
            errorf("%s[%d]: (%llu>%llu)!\n",
                __FILE__, __LINE__, claimed, measured);
            ++errors;
        }

    }

    printf("%s[%d]: yield uninterruptable\n", __FILE__, __LINE__);

    then = Platform::instance().time();
    requested = Platform::instance().frequency() * 20;
    claimed = Platform::instance().yield(requested, false);
    now = Platform::instance().time();
    measured = now - then;
    ticks.seconds(measured, seconds, nanoseconds);

    printf("%10s %10s %10s %20s\n",
        "REQUESTED", "CLAIMED", "MEASURED", "SECONDS");
    printf("%10llu %10llu %10llu %10llu.%09lu\n",
        requested, claimed, measured, seconds, nanoseconds);

    printf("%s[%d]: yield interruptable\n", __FILE__, __LINE__);

    then = Platform::instance().time();
    claimed = Platform::instance().yield(requested, true);
    now = Platform::instance().time();
    measured = now - then;
    ticks.seconds(measured, seconds, nanoseconds);

    printf("%10s %10s %10s %20s\n",
        "REQUESTED", "CLAIMED", "MEASURED", "SECONDS");
    printf("%10llu %10llu %10llu %10llu.%09lu\n",
        requested, claimed, measured, seconds, nanoseconds);

    printf("%s[%d]: yield nines\n", __FILE__, __LINE__);

    printf("%10s %10s %10s %20s\n",
        "REQUESTED", "CLAIMED", "MEASURED", "SECONDS");

    requested = 0;

    int limit;

    while (true) {

        seconds = requested / freq1;
        limit = (1 >= seconds) ? 10 : 1;

        for (int ii = 0; limit > ii; ++ii) {

            then = Platform::instance().time();
            claimed = Platform::instance().yield(requested);
            now = Platform::instance().time();
            measured = now - then;
            ticks.seconds(measured, seconds, nanoseconds);

            printf("%10llu %10llu %10llu %10llu.%09lu\n",
                requested, claimed, measured, seconds, nanoseconds);

            if (claimed < requested) {
                errorf("%s[%d]: (%llu<%llu)!\n",
                    __FILE__, __LINE__, claimed, requested);
                ++errors;
            }

            if (claimed > measured) {
                errorf("%s[%d]: (%llu>%llu)!\n",
                    __FILE__, __LINE__, claimed, measured);
                ++errors;
            }

        }

        seconds = measured / freq1;
        if (100  <= seconds) {
            break;
        }

        requested = (0 == requested) ? 9 : (requested * 10) + 9;

    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
