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
 *  Implements the Counters unit test.
 *
 *  @see    Counters
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Counters.h"
#include "com/diag/desperado/Counters.h"
#include "com/diag/desperado/NewCounters.h"
#include "com/diag/desperado/NewCounters.h"
#include "com/diag/desperado/CountersType.h"
#include "com/diag/desperado/CountersType.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Desperado.h"

enum {
    COUNTER0 = 0,
    COUNTER1,
    COUNTER2,
    COUNTER3,
    COUNTER4,
    COUNTER5,
    COUNTER6,
    COUNTER7,
    COUNTER8,
    COUNTER9,
    COUNTERS
};

static const char* labels[] = {
    "COUNTER0",
    "COUNTER1",
    0,
    "COUNTER3",
    0,
    "COUNTER5",
    "COUNTER6",
    "COUNTER7",
    0,
    "OTHER"
};

template class CountersType<COUNTERS>;

class UT_Counters_Functor : public Counters::Functor {
public:
    UT_Counters_Functor(bool a = true) : all(a) { }
    virtual ~UT_Counters_Functor() {}
    virtual bool operator() (
        unsigned int /* id */,
        const char* label,
        int value
    ) {
        Print printf(Platform::instance().output());
        printf("%s=%d\n", label, value);
        return all;
    }
    bool all;
};

typedef Counters::Counter Counter;

static Counters staticCounters;
static NewCounters staticNewCounters;
static CountersType<COUNTERS> staticCountersType;

CXXCAPI int unittestCounters(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;
    Output* errorp = &Platform::instance().error();

    Counter alternates[COUNTERS] = { 0 };
    Counters alternate(COUNTERS, alternates, labels);
    Counter counters[COUNTERS] = { 0 };
    Counters counter(COUNTERS, counters, labels);

    const char* biglabels[25] = { 0 };
    Counters big0;
    Counters big00(0, counters);
    Counters big000(1);
    Counter big1s[20];
    Counters big1(countof(big1s), big1s, biglabels);
    Counter big2s[21];
    Counters big2(countof(big2s), big2s, biglabels);
    Counter big3s[22];
    Counters big3(countof(big3s), big3s, biglabels);
    Counter big4s[23];
    Counters big4(countof(big4s), big4s, biglabels);
    Counter big5s[24];
    Counters big5(countof(big5s), big5s, biglabels);
    Counter big6s[25];
    Counters big6(countof(big6s), big6s, biglabels);
    const char* big7labels[256] = { 0 };

    NewCounters big7(256, big7labels);
    big7.reset();

    NewCounters big8(256);
    big8.reset();

    CountersType<COUNTERS> type9(labels);

    UT_Counters_Functor all;
    UT_Counters_Functor partial(false);

    int value;
    int expected;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticCounters.show();
    ::staticNewCounters.show();
    ::staticCountersType.show();

    counter.show();
    alternate.show(0, errorp);

    printf("%s[%d]: initial\n", __FILE__, __LINE__);

    counter.show(1);
    alternate.show(1, errorp);

    printf("%s[%d]: increment\n", __FILE__, __LINE__);

    for (int ii = 0; ii < COUNTERS; ++ii) {
        for (int jj = 0; jj < ii; ++jj) {
            value = counter.get(ii);
            expected = jj;
            if (value != expected) {
                errorf("%s[%d]: (%d!=%d)!\n",
                    __FILE__, __LINE__, value, expected);
                ++errors;
            }
            value = counter.increment(ii);
            expected = jj;
            if (value != expected) {
                errorf("%s[%d]: (%d!=%d)!\n",
                    __FILE__, __LINE__, value, expected);
                ++errors;
            }
            value = counter.get(ii);
            expected = jj + 1;
            if (value != expected) {
                errorf("%s[%d]: (%d!=%d)!\n",
                    __FILE__, __LINE__, value, expected);
                ++errors;
            }
        }
    }

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    for (int ii = 0; ii < COUNTERS; ++ii) {
        for (int jj = 0; jj < ii; ++jj) {
            value = counter.get(ii);
            expected = ii;
            if (value != expected) {
                errorf("%s[%d]: (%d!=%d)!\n",
                    __FILE__, __LINE__, value, expected);
                ++errors;
            }
        }
    }

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    printf("%s[%d]: decrement\n", __FILE__, __LINE__);

    for (int ii = 0; ii < COUNTERS; ++ii) {
        for (int jj = 0; jj < ii; ++jj) {
            value = counter.get(ii);
            expected = ii - jj;
            if (value != expected) {
                errorf("%s[%d]: (%d!=%d)!\n",
                    __FILE__, __LINE__, value, expected);
                ++errors;
            }
            value = counter.decrement(ii);
            expected = ii - jj;
            if (value != expected) {
                errorf("%s[%d]: (%d!=%d)!\n",
                    __FILE__, __LINE__, value, expected);
                ++errors;
            }
            value = counter.get(ii);
            expected = ii - jj - 1;
            if (value != expected) {
                errorf("%s[%d]: (%d!=%d)!\n",
                    __FILE__, __LINE__, value, expected);
                ++errors;
            }
        }
    }

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = 0;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    printf("%s[%d]: increment(n)\n", __FILE__, __LINE__);

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = 0;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
        value = counter.increment(ii, ii);
        expected = 0;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
        value = counter.get(ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    printf("%s[%d]: decrement(n)\n", __FILE__, __LINE__);

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
        value = counter.decrement(ii, ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
        value = counter.get(ii);
        expected = 0;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = 0;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    printf("%s[%d]: set()\n", __FILE__, __LINE__);

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.set(ii, ii);
        expected = 0;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
        value = counter.get(ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = ii;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    printf("%s[%d]: reset()\n", __FILE__, __LINE__);

    counter.reset();

    for (int ii = 0; ii < COUNTERS; ++ii) {
        value = counter.get(ii);
        expected = 0;
        if (value != expected) {
            errorf("%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, value, expected);
            ++errors;
        }
    }

    printf("%s[%d]: overflow\n", __FILE__, __LINE__);

    counter.set(COUNTERS, signedintmaxof(int) - 1);
    expected = 0;
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS);
    expected = signedintmaxof(int) - 1;
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS - 1);
    expected = signedintmaxof(int) - 1;
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.increment(COUNTERS);
    expected = signedintmaxof(int) - 1;
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS);
    expected = signedintmaxof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS - 1);
    expected = signedintmaxof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.increment(COUNTERS);
    expected = signedintmaxof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS);
    expected = signedintminof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS - 1);
    expected = signedintminof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    printf("%s[%d]: underflow\n", __FILE__, __LINE__);

    counter.set(COUNTERS, signedintminof(int) + 1);
    expected = signedintminof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS);
    expected = signedintminof(int) + 1;
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS - 1);
    expected = signedintminof(int) + 1;
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.decrement(COUNTERS);
    expected = signedintminof(int) + 1;
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS);
    expected = signedintminof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS - 1);
    expected = signedintminof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.decrement(COUNTERS);
    expected = signedintminof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS);
    expected = signedintmaxof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    value = counter.get(COUNTERS - 1);
    expected = signedintmaxof(int);
    if (value != expected) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, value, expected);
        ++errors;
    }

    printf("%s[%d]: mib alternate all\n", __FILE__, __LINE__);

    alternate.apply(all);

    printf("%s[%d]: mib alternate partial\n",
        __FILE__, __LINE__);

    alternate.apply(partial);

    printf("%s[%d]: mib counter all\n", __FILE__, __LINE__);

    counter.apply(all);

    printf("%s[%d]: mib counter partial\n",
        __FILE__, __LINE__);

    counter.apply(partial);

    printf("%s[%d]: final\n", __FILE__, __LINE__);

    counter.show(1);
    alternate.show(1, errorp);

    printf("%s[%d]: big0 output\n", __FILE__, __LINE__);
    big0.show(1);
    errorf("%s[%d]: big0 error\n", __FILE__, __LINE__);
    big0.show(1, errorp);

    printf("%s[%d]: big00 output\n", __FILE__, __LINE__);
    big00.show(1);
    errorf("%s[%d]: big00 error\n", __FILE__, __LINE__);
    big00.show(1, errorp);

    printf("%s[%d]: big000 output\n", __FILE__, __LINE__);
    big000.show(1);
    errorf("%s[%d]: big000 error\n", __FILE__, __LINE__);
    big000.show(1, errorp);

    printf("%s[%d]: big1 output\n", __FILE__, __LINE__);
    big1.show(1);
    errorf("%s[%d]: big1 error\n", __FILE__, __LINE__);
    big1.show(1, errorp);

    printf("%s[%d]: big2 output\n", __FILE__, __LINE__);
    big2.show(1);
    errorf("%s[%d]: big2 error\n", __FILE__, __LINE__);
    big2.show(1, errorp);

    printf("%s[%d]: big3 output\n", __FILE__, __LINE__);
    big3.show(1);
    errorf("%s[%d]: big3 error\n", __FILE__, __LINE__);
    big3.show(1, errorp);

    for (unsigned int ii = 0; big4.cardinal() > ii; ++ii) {
        big4.set(ii, signedintminof(int));
    }
    printf("%s[%d]: big4 output\n", __FILE__, __LINE__);
    big4.show(1);
    errorf("%s[%d]: big4 error\n", __FILE__, __LINE__);
    big4.show(1, errorp);

    for (unsigned int ii = 0; big5.cardinal() > ii; ++ii) {
        big5.set(ii, signedintmaxof(int));
    }
    printf("%s[%d]: big5 output\n", __FILE__, __LINE__);
    big5.show(1);
    errorf("%s[%d]: big5 error\n", __FILE__, __LINE__);
    big5.show(1, errorp);

    printf("%s[%d]: big6 output\n", __FILE__, __LINE__);
    big6.show(1);
    errorf("%s[%d]: big6 error\n", __FILE__, __LINE__);
    big6.show(1, errorp);

    printf("%s[%d]: big7 output\n", __FILE__, __LINE__);
    big7.show(1);
    errorf("%s[%d]: big7 error\n", __FILE__, __LINE__);
    big7.show(1, errorp);

    printf("%s[%d]: big8 output\n", __FILE__, __LINE__);
    big8.show();
    big8.show(1);
    errorf("%s[%d]: big8 error\n", __FILE__, __LINE__);
    big8.show(1, errorp);

    for (unsigned int ii = 0; type9.cardinal() > ii; ++ii) {
        type9.set(ii, ii);
    }
    printf("%s[%d]: type9 output\n", __FILE__, __LINE__);
    type9.show(1);
    errorf("%s[%d]: type9 error\n", __FILE__, __LINE__);
    type9.show(1, errorp);

    printf("%s[%d]: errors=%d\n", __FILE__, __LINE__, errors);

    return errors;
}
