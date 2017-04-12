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
 *  Implements the second Heap CXXCAPI unit test.
 *
 *  This unit test is a little confusing: the calls to
 *  malloc() etc. are mapped via inline functions to
 *  the Desperado CXXCAPI heap calls. There is no way
 *  (and this is the intent) for the original malloc() etc.
 *  calls to be invoked. So this unit test compares the
 *  mapped malloc() against the native Desperado CXXCAPI
 *  calls, not against the actual underlying malloc() calls.
 *
 *  You will probably get some warnings from the compiler since
 *  the Desperado malloc.h header file prototypes malloc() etc. as
 *  inline after it is likely someone has included a system
 *  header file that prototypes the same functions as extern.
 *
 *  @see    Heap
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/malloc.h"
#include "com/diag/desperado/malloc.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"

#define MALLOC(b)       heap_malloc(platform_heap(), b)
#define REALLOC(p, b)   heap_realloc(platform_heap(), p, b)
#define CALLOC(m, b)    heap_calloc(platform_heap(), m, b)
#define FREE(p)         heap_free(platform_heap(), p)

static int null(void* ptr) {
    return 0 != ptr;
}

int unittestHeap3() {
    int errors = 0;
    const size_t BYTES = 256;
    const size_t MEMBERS = 10;
    void* baseline0[4];
    void* baseline[13];
    void* test0[countof(baseline0)];
    void* test[countof(baseline)];
    size_t sizes[countof(baseline)];
    size_t expected[countof(baseline)];
    size_t ii = 0;
    size_t jj = 0;
    size_t kk = 0;
    void* block[14];
    size_t size = 0;
    size_t total = 0;
    void* vp;
    uint8_t* cp;
    size_t set = 0;
    size_t get = 0;
    int nn;

    print_f(platform_output(), "%s[%d]: begin\n", __FILE__, __LINE__);

    print_f(platform_output(), "%s[%d]: semantics\n",
        __FILE__, __LINE__);

    for (ii = 0; countof(baseline0) > ii; ++ii) {
        baseline0[ii] = malloc(BYTES);
        test0[ii] = MALLOC(BYTES);
        if (0 == test0[ii]) {
            print_f(platform_output(), "%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, test0[ii], ii);
            ++errors;
        }
        if (null(baseline0[ii]) != null(test0[ii])) {
            print_f(platform_output(), "%s[%d]: (%p?%p[%lu])!\n",
                __FILE__, __LINE__, baseline0[ii], test0[ii], ii);
            ++errors;
        }
    }

    expected[jj] = 0;
    baseline[jj] = 0;
    test[jj] = 0;
    sizes[jj] = 0;
    ++jj;

    expected[jj] = 0;
    baseline[jj] = malloc(expected[jj]);
    test[jj] = MALLOC(expected[jj]);
    ++jj;

    expected[jj] = BYTES;
    baseline[jj] = malloc(expected[jj]);
    test[jj] = MALLOC(expected[jj]);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = realloc(0, expected[jj]);
    test[jj] = REALLOC(0, expected[jj]);
    ++jj;

    expected[jj] = BYTES;
    baseline[jj] = realloc(0, expected[jj]);
    test[jj] = REALLOC(0, expected[jj]);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = realloc(baseline0[kk], expected[jj]);
    test[jj] = REALLOC(test0[kk], expected[jj]);
    ++jj;
    ++kk;

    expected[jj] = BYTES;
    baseline[jj] = realloc(baseline0[kk], expected[jj]);
    test[jj] = REALLOC(test0[kk], expected[jj]);
    ++jj;
    ++kk;

    expected[jj] = BYTES * 2;
    baseline[jj] = realloc(baseline0[kk], expected[jj]);
    test[jj] = REALLOC(test0[kk], expected[jj]);
    ++jj;
    ++kk;

    expected[jj] = BYTES / 4;
    baseline[jj] = realloc(baseline0[kk], expected[jj]);
    test[jj] = REALLOC(test0[kk], expected[jj]);
    ++jj;
    ++kk;

    expected[jj] = 0;
    baseline[jj] = calloc(0, 0);
    test[jj] = CALLOC(0, 0);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = calloc(0, BYTES);
    test[jj] = CALLOC(0, BYTES);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = calloc(MEMBERS, 0);
    test[jj] = CALLOC(MEMBERS, 0);
    ++jj;

    expected[jj] = MEMBERS * BYTES;
    baseline[jj] = calloc(MEMBERS, BYTES);
    test[jj] = CALLOC(MEMBERS, BYTES);
    ++jj;

    print_f(platform_output(), "%3s %10s %10s %10s\n",
        "II", "STDLIB", "HEAP", "SIZE");

    for (ii = 0; countof(test) > ii; ++ii) {
        print_f(platform_output(), "%3d 0x%08x 0x%08x %10u\n",
            ii, staticcastto(uintptr_t,baseline[ii]),
            staticcastto(uintptr_t,test[ii]),
            sizes[ii]);
    }

    for (ii = 0; countof(test) > ii; ++ii) {
        if (null(baseline[ii]) != null(test[ii])) {
            print_f(platform_output(), "%s[%d]: (%p?%p[%lu])!\n",
                __FILE__, __LINE__, baseline[ii], test[ii], ii);
            ++errors;
        }
        free(baseline[ii]);
        FREE(test[ii]);
    }

    print_f(platform_output(), "%s[%d]: malloc\n", __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    for (ii = 0; countof(block) > ii; ++ii) {
        block[ii] = MALLOC(size);
        if (0 == block[ii]) {
            print_f(platform_output(), "%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        total += size;
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: free\n", __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    size = 0;
    for (ii = 0; countof(block) > ii; ++ii) {
        FREE(block[ii]);
        total -= size;
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: malloc via realloc\n",
        __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    size = 0;
    for (ii = 0; countof(block) > ii; ++ii) {
        block[ii] = REALLOC(0, size);
        if (0 == block[ii]) {
            print_f(platform_output(), "%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        total += size;
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: free via realloc\n",
        __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    size = 0;
    for (ii = 0; countof(block) > ii; ++ii) {
        vp = REALLOC(block[ii], 0);
        if (0 != vp) {
            print_f(platform_output(), "%s[%d]: (%p!=%p[%lu])!\n",
                __FILE__, __LINE__, 0, vp, ii);
            ++errors;
        }
        total -= size;
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: malloc via calloc 1\n",
        __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    size = 0;
    for (ii = 0; countof(block) > ii; ++ii) {
        block[ii] = CALLOC(1, size);
        if (0 == block[ii]) {
            print_f(platform_output(), "%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        cp = staticcastto(uint8_t*,block[ii]);
        for (jj = 0; size > jj; ++jj) {
            if (0 != cp[jj]) {
                print_f(platform_output(),
                    "%s[%d]: (%p!=%p[%lu][%lu])!\n",
                    __FILE__, __LINE__, 0, cp[jj], ii, jj);
                ++errors;
            }
        }
        total += size;
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: free\n", __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    size = 0;
    for (ii = 0; countof(block) > ii; ++ii) {
        FREE(block[ii]);
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: malloc via calloc 2\n",
        __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    size = 0;
    for (ii = 0; countof(block) > ii; ++ii) {
        block[ii] = CALLOC(size, 1);
        if (0 == block[ii]) {
            print_f(platform_output(), "%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        cp = staticcastto(uint8_t*,block[ii]);
        for (jj = 0; size > jj; ++jj) {
            if (0 != cp[jj]) {
                print_f(platform_output(),
                    "%s[%d]: (%p!=%p[%lu][%lu])!\n",
                    __FILE__, __LINE__, 0, cp[jj], ii, jj);
                ++errors;
            }
        }
        total += size;
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: free\n", __FILE__, __LINE__);

    print_f(platform_output(),
        "%3s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "SUM");

    size = 0;
    for (ii = 0; countof(block) > ii; ++ii) {
        FREE(block[ii]);
        print_f(platform_output(),
            "%3d 0x%08x %10u %10u\n",
            ii, block[ii], size, total);
        size = size ? (size * 2) : 1;
    }

    print_f(platform_output(), "%s[%d]: realloc\n",
        __FILE__, __LINE__);

    print_f(platform_output(), "%3s %10s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "GET", "SET");

    block[0] = 0;
    for (size = 0; 256 >= size; ++size) {
        block[0] = REALLOC(block[0], size);
        if (0 == block[0]) {
            print_f(platform_output(), "%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[0], size);
            ++errors;
        }
        cp = staticcastto(uint8_t*,block[0]);
        for (nn = 0; staticcastto(int,set) > nn; ++nn) {
            if (get != cp[nn]) {
                print_f(platform_output(), "%s[%d]: (%lu!=%lu[%d])!\n",
                    __FILE__, __LINE__, get, cp[nn], nn);
                ++errors;
            }
        }
        get = set;
        for (nn = 0; staticcastto(int,size) > nn; ++nn) {
            cp[nn] = set;
        }
        set = size;
        total += size;
        print_f(platform_output(), "%3d 0x%08x %10u %10u %10u\n",
            size, block[0], size, get, set);
    }

    FREE(block[0]);

    print_f(platform_output(), "%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
