/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2007-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the GeometricThrottle unit test.
 *
 *  @see    GeometricThrottle
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/GeometricThrottle.h"
#include "com/diag/desperado/GeometricThrottle.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Ticks.h"
#include "com/diag/desperado/Desperado.h"

static GeometricThrottle staticGeometricThrottle;

CXXCAPI int unittestGeometricThrottle(void*, int limit) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    ticks_t now;
    ticks_t then;
    uint64_t seconds;
    uint32_t nanoseconds;
    GeometricThrottle throttle;
    GeometricThrottle throttle1;
    GeometricThrottle save1 = throttle;
    GeometricThrottle save2;
    unsigned long ii, jj, kk;
    unsigned int expected;
    unsigned int attempted = 0;
    unsigned int admitted = 0;
    bool alarmed;
    Ticks ticks;
    ticks_t delay;
    int limit2 = limit;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticGeometricThrottle.show();

    if (limit2 < 0) {
        expected = 32;
    } else {
        expected = 0;
        for (unsigned int ii = limit2; ii > 0; ii>>=1) {
            ++expected;
        }
    }

    printf("%s[%d]: constructors\n", __FILE__, __LINE__);

    throttle.show();
    save1.show();
    save2 = throttle;
    save2.show();

    if ((delay = throttle.frequency()) != 0) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, delay, 0);
        throttle.show();
        ++errors;
    }

    printf("%s[%d]: admissions\n", __FILE__, __LINE__);

    then = Platform::instance().time();
    now = then;
    ticks.seconds(now - then, seconds, nanoseconds);
    printf("%s[%d]: initial %llu.%09lu %u/%u\n",
        __FILE__, __LINE__, seconds, nanoseconds, admitted, attempted);
    throttle.show();

    for (ii = 0; (ii < 3) && (0 != limit2); ii++) { // 0x..(3*0xffffffff)
        for (jj = 0; (jj < 0x10000) && (0 != limit2); ++jj) { // 0..0xffffffff
            for (kk = 0; (kk < 0x10000) && (0 != limit2); ++kk) { // 0..0xffff
                ++attempted;
                if ((delay = throttle.admissible()) == 0) {
                    now = Platform::instance().time();
                    ticks.seconds(now - then, seconds, nanoseconds);
                    ++admitted;
                    printf("%s[%d]: admit %llu.%09lu %u/%u\n",
                        __FILE__, __LINE__,
                        seconds, nanoseconds, admitted, attempted);
                    throttle.commit(true);
                    alarmed = throttle.isAlarmed();
                    if (alarmed) {
                        errorf("%s[%d]: (%d!=%d)!\n",
                            __FILE__, __LINE__, alarmed, false);
                        throttle.show();
                        ++errors;
                    }
                } else if (delay == intmaxof(ticks_t)) {
                    throttle.rollback();
                    alarmed = throttle.isAlarmed();
                    if (alarmed) {
                        errorf("%s[%d]: (%d!=%d)!\n",
                            __FILE__, __LINE__, alarmed, false);
                        throttle.show();
                        ++errors;
                    }
                } else {
                    errorf("%s[%d]: (%llu)!\n",
                        __FILE__, __LINE__, delay);
                    throttle.show();
                    ++errors;
                }
                if ((delay = throttle1.admissible()) == 0) {
                    throttle1.commit();
                    alarmed = throttle1.isAlarmed();
                    if (alarmed) {
                        errorf("%s[%d]: (%d!=%d)!\n",
                            __FILE__, __LINE__, alarmed, false);
                        throttle1.show();
                        ++errors;
                    }
                } else if (delay == intmaxof(ticks_t)) {
                    throttle1.commit();
                    alarmed = throttle1.isAlarmed();
                    if (!alarmed) {
                        errorf("%s[%d]: (%d!=%d)!\n",
                            __FILE__, __LINE__, alarmed, true);
                        throttle1.show();
                        ++errors;
                    }
                } else {
                    errorf("%s[%d]: (%llu)!\n",
                        __FILE__, __LINE__, delay);
                    throttle.show();
                    ++errors;
                }
                if (limit2 > 0) {
                    --limit2;
                }
            }
        }
    }

    now = Platform::instance().time();
    ticks.seconds(now - then, seconds, nanoseconds);
    printf("%s[%d]: final %llu.%09lu %u/%u\n",
        __FILE__, __LINE__, seconds, nanoseconds, admitted, attempted);
    throttle.show();

    if (admitted != expected) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__,
            admitted, expected);
        throttle.show();
        ++errors;
    }

    printf("%s[%d]: reset\n", __FILE__, __LINE__);

    throttle.reset();
    throttle.show();

    limit2 = limit;
    admitted = 0;
    attempted = 0;

    then = Platform::instance().time();
    now = then;
    ticks.seconds(now - then, seconds, nanoseconds);
    printf("%s[%d]: initial %llu.%09lu %u/%u\n",
        __FILE__, __LINE__, seconds, nanoseconds, admitted, attempted);
    throttle.show();

    for (ii = 0; (ii < 3) && (0 != limit2); ii++) { // 0x..(3*0xffffffff)
        for (jj = 0; (jj < 0x10000) && (0 != limit2); ++jj) { // 0..0xffffffff
            for (kk = 0; (kk < 0x10000) && (0 != limit2); ++kk) { // 0..0xffff
                ++attempted;
                if ((delay = throttle.admissible()) == 0) {
                    now = Platform::instance().time();
                    ticks.seconds(now - then, seconds, nanoseconds);
                    ++admitted;
                    printf("%s[%d]: admit %llu.%09lu %u/%u\n",
                        __FILE__, __LINE__,
                        seconds, nanoseconds, admitted, attempted);
                    throttle.commit();
                    alarmed = throttle.isAlarmed();
                    if (alarmed) {
                        errorf("%s[%d]: (%d!=%d)!\n",
                            __FILE__, __LINE__, alarmed, false);
                        throttle.show();
                        ++errors;
                    }
                } else if (delay == intmaxof(ticks_t)) {
                    throttle.rollback();
                    alarmed = throttle.isAlarmed();
                    if (alarmed) {
                        errorf("%s[%d]: (%d!=%d)!\n",
                            __FILE__, __LINE__, alarmed, false);
                        throttle.show();
                        ++errors;
                    }
                } else {
                    errorf("%s[%d]: (%llu)!\n",
                        __FILE__, __LINE__, delay);
                    throttle.show();
                    ++errors;
                }
                if (limit2 > 0) {
                    --limit2;
                }
            }
        }
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
