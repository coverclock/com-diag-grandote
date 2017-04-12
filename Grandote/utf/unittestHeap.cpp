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
 *  Implements the Heap unit test.
 *
 *  @see    Heap
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdlib.h"
#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Desperado.h"

static inline bool null(void* ptr) {
    return 0 != ptr;
}

static Heap staticHeap;

CXXCAPI int unittestHeap(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticHeap.show();

    Heap heap;
    heap.show();

    printf("%s[%d]: semantics\n", __FILE__, __LINE__);

    bool was = heap.trace(true);
    if (was) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, false, was);
        ++errors;
    }

    const size_t BYTES = 256;
    const size_t MEMBERS = 10;

    void* baseline0[4];
    void* test0[countof(baseline0)];

    for (size_t ii = 0; countof(baseline0) > ii; ++ii) {
        baseline0[ii] = std::malloc(BYTES);
        test0[ii] = heap.malloc(BYTES);
        if (0 == test0[ii]) {
            errorf("%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, test0[ii], ii);
            ++errors;
        }
        if (null(baseline0[ii]) != null(test0[ii])) {
            errorf("%s[%d]: (%p?%p[%lu])!\n",
                __FILE__, __LINE__, baseline0[ii], test0[ii], ii);
            ++errors;
        }
        if (BYTES != heap.size(test0[ii])) {
            errorf("%s[%d]: (%lu!=%lu[%lu])!\n",
                __FILE__, __LINE__, BYTES, heap.size(test0[ii]), ii);
            ++errors;
        }
    }

    void* baseline[13];
    size_t expected[countof(baseline)];
    void* test[countof(baseline)];
    size_t sizes[countof(baseline)];
    size_t current = 0;
    int jj = 0;
    int kk = 0;

    expected[jj] = 0;
    baseline[jj] = 0;
    test[jj] = 0;
    sizes[jj] = 0;
    ++jj;

    expected[jj] = 0;
    baseline[jj] = std::malloc(expected[jj]);
    test[jj] = heap.malloc(expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

    expected[jj] = BYTES;
    baseline[jj] = std::malloc(expected[jj]);
    test[jj] = heap.malloc(expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = std::realloc(0, expected[jj]);
    test[jj] = heap.realloc(0, expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

#if defined(DESPERADO_HAS_DEBUGGING)
    //
    //  There appears to be some ambiguity as to what it means
    //  if both arguments to realloc() are zero. If the second
    //  argument is zero, it is supposed to be equivalent to
    //  calling free(), and calling free() with a null pointer
    //  is specifically allowed by POSIX. If the first argument
    //  is null, it is supposed to be equivalent to calling
    //  malloc(), and calling malloc() with a zero size is
    //  also allowed by POSIX. Both Linux and Cygwin seem to
    //  take the latter interpretation, so Desperado does too.
    //
    errorf("%s[%d]: std::realloc(0x%x,0x%x)=0x%x\n",
        __FILE__, __LINE__, 0, expected[jj-1], baseline[jj-1]);
    errorf("%s[%d]: heap.realloc(0x%x,0x%x)=0x%x\n",
        __FILE__, __LINE__, 0, expected[jj-1], test[jj-1]);
    errorf("%s[%d]: heap.size(0x%x)=%lu\n",
        __FILE__, __LINE__, test[jj-1], sizes[jj-1]);
#endif

    expected[jj] = BYTES;
    baseline[jj] = std::realloc(0, expected[jj]);
    test[jj] = heap.realloc(0, expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = std::realloc(baseline0[kk], expected[jj]);
    test[jj] = heap.realloc(test0[kk], expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;
    ++kk;

#if defined(DESPERADO_HAS_DEBUGGING)
    //
    //  Cygwin doesn't seem to implement realloc() the way both
    //  POSIX and its own man page describes.
    //
    errorf("%s[%d]: std::realloc(0x%x,0x%x)=0x%x\n",
        __FILE__, __LINE__, baseline0[kk-1], expected[jj-1], baseline[jj-1]);
    errorf("%s[%d]: heap.realloc(0x%x,0x%x)=0x%x\n",
        __FILE__, __LINE__, test0[kk-1], expected[jj-1], test[jj-1]);
    errorf("%s[%d]: heap.size(0x%x)=%lu\n",
        __FILE__, __LINE__, test[jj-1], sizes[jj-1]);
#endif

    expected[jj] = BYTES;
    baseline[jj] = std::realloc(baseline0[kk], expected[jj]);
    test[jj] = heap.realloc(test0[kk], expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;
    ++kk;

    expected[jj] = BYTES * 2;
    baseline[jj] = std::realloc(baseline0[kk], expected[jj]);
    test[jj] = heap.realloc(test0[kk], expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;
    ++kk;

    expected[jj] = BYTES / 4;
    baseline[jj] = std::realloc(baseline0[kk], expected[jj]);
    test[jj] = heap.realloc(test0[kk], expected[jj]);
    sizes[jj] = heap.size(test[jj]);
    ++jj;
    ++kk;

    expected[jj] = 0;
    baseline[jj] = std::calloc(0, 0);
    test[jj] = heap.calloc(0, 0);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = std::calloc(0, BYTES);
    test[jj] = heap.calloc(0, BYTES);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

    expected[jj] = 0;
    baseline[jj] = std::calloc(MEMBERS, 0);
    test[jj] = heap.calloc(MEMBERS, 0);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

    expected[jj] = MEMBERS * BYTES;
    baseline[jj] = std::calloc(MEMBERS, BYTES);
    test[jj] = heap.calloc(MEMBERS, BYTES);
    sizes[jj] = heap.size(test[jj]);
    ++jj;

    printf("%3s %10s %10s %10s\n",
        "II", "STDLIB", "HEAP", "SIZE");

    for (size_t ii = 0; countof(test) > ii; ++ii) {
        printf("%3d 0x%08x 0x%08x %10u\n",
            ii, reinterpretcastto(uintptr_t,baseline[ii]),
            reinterpretcastto(uintptr_t,test[ii]),
            sizes[ii]);
    }

    for (size_t ii = 0; countof(test) > ii; ++ii) {
        if (null(baseline[ii]) != null(test[ii])) {
            errorf("%s[%d]: (%p?%p[%lu])!\n",
                __FILE__, __LINE__, baseline[ii], test[ii], ii);
            ++errors;
        }
        if (expected[ii] != sizes[ii]) {
            errorf("%s[%d]: (%lu!=%lu[%lu])!\n",
                __FILE__, __LINE__, expected[ii], sizes[ii], ii);
            ++errors;
        }
        free(baseline[ii]);
        heap.free(test[ii]);
    }

    current = heap.getCurrent();
    if (0 != current) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, 0, current);
        ++errors;
    }

    heap.show();

    was = heap.trace(false);
    if (!was) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, was);
        ++errors;
    }

    printf("%s[%d]: malloc\n", __FILE__, __LINE__);

    void* block[14];
    size_t size = 0;
    size_t actual = 0;
    size_t total = 0;
    void* vp;
    uint8_t* cp;

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    for (size_t ii = 0; countof(block) > ii; ++ii) {
        block[ii] = heap.malloc(size);
        if (0 == block[ii]) {
            errorf("%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        actual = heap.size(block[ii]);
        if (size != actual) {
            errorf("%s[%d]: (%lu!=%lu[%lu])!\n",
                __FILE__, __LINE__, size, actual, ii);
            ++errors;
        }
        total += actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    heap.show();

    printf("%s[%d]: free\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    size = 0;
    for (size_t ii = 0; countof(block) > ii; ++ii) {
        actual = heap.size(block[ii]);
        heap.free(block[ii]);
        total -= actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    current = heap.getCurrent();
    if (0 != current) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, 0, current);
        ++errors;
    }

    heap.show();

    printf("%s[%d]: malloc via realloc\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    size = 0;
    for (size_t ii = 0; countof(block) > ii; ++ii) {
        block[ii] = heap.realloc(0, size);
        if (0 == block[ii]) {
            errorf("%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        actual = heap.size(block[ii]);
        if (size != actual) {
            errorf("%s[%d]: (%lu!=%lu[%lu])!\n",
                __FILE__, __LINE__, size, actual, ii);
            ++errors;
        }
        total += actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    heap.show();

    printf("%s[%d]: free via realloc\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    size = 0;
    for (size_t ii = 0; countof(block) > ii; ++ii) {
        actual = heap.size(block[ii]);
        vp = heap.realloc(block[ii], 0);
        if (0 != vp) {
            errorf("%s[%d]: (%p!=%p[%lu])!\n",
                __FILE__, __LINE__, 0, vp, ii);
            ++errors;
        }
        total -= actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    current = heap.getCurrent();
    if (0 != current) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, 0, current);
        ++errors;
    }

    heap.show();

    printf("%s[%d]: malloc via calloc 1\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    size = 0;
    for (size_t ii = 0; countof(block) > ii; ++ii) {
        block[ii] = heap.calloc(1, size);
        if (0 == block[ii]) {
            errorf("%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        actual = heap.size(block[ii]);
        if (size != actual) {
            errorf("%s[%d]: (%lu!=%lu[%lu])!\n",
                __FILE__, __LINE__, size, actual, ii);
            ++errors;
        }
        cp = staticcastto(uint8_t*,block[ii]);
        for (int jj = 0; staticcastto(int,size) > jj; ++jj) {
            if (0 != cp[jj]) {
                errorf("%s[%d]: (%p!=%p[%lu][%lu])!\n",
                    __FILE__, __LINE__, 0, cp[jj], ii, jj);
                ++errors;
            }
        }
        total += actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    heap.show();

    printf("%s[%d]: free\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    size = 0;
    for (size_t ii = 0; countof(block) > ii; ++ii) {
        actual = heap.size(block[ii]);
        heap.free(block[ii]);
        total -= actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    current = heap.getCurrent();
    if (0 != current) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, 0, current);
        ++errors;
    }

    heap.show();

    printf("%s[%d]: malloc via calloc 2\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    size = 0;
    for (size_t ii = 0; countof(block) > ii; ++ii) {
        block[ii] = heap.calloc(size, 1);
        if (0 == block[ii]) {
            errorf("%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[ii], ii);
            ++errors;
        }
        actual = heap.size(block[ii]);
        if (size != actual) {
            errorf("%s[%d]: (%lu!=%lu[%lu])!\n",
                __FILE__, __LINE__, size, actual, ii);
            ++errors;
        }
        cp = staticcastto(uint8_t*,block[ii]);
        for (size_t jj = 0; size > jj; ++jj) {
            if (0 != cp[jj]) {
                errorf("%s[%d]: (%p!=%p[%lu][%lu])!\n",
                    __FILE__, __LINE__, 0, cp[jj], ii, jj);
                ++errors;
            }
        }
        total += actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    heap.show();

    printf("%s[%d]: free\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s %10s %4s %4s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "SUM",
        "TOTAL", "CURRENT","SUCC", "FAIL");

    size = 0;
    for (size_t ii = 0; countof(block) > ii; ++ii) {
        actual = heap.size(block[ii]);
        heap.free(block[ii]);
        total -= actual;
        printf("%3d 0x%08x %10u %10u %10u %10u %10u %4u %4u\n",
            ii, block[ii], size, actual, total,
            heap.getTotal(), heap.getCurrent(),
            heap.getSuccesses(), heap.getFailures());
        size = size ? (size * 2) : 1;
    }

    current = heap.getCurrent();
    if (0 != current) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, 0, current);
        ++errors;
    }

    heap.show();

    printf("%s[%d]: realloc\n", __FILE__, __LINE__);

    printf("%3s %10s %10s %10s %10s %10s\n",
        "II", "ADDRESS", "SIZE", "ACTUAL", "GET", "SET");

    size_t set = 0;
    size_t get = 0;
    block[0] = 0;
    for (size = 0; 256 >= size; ++size) {
        block[0] = heap.realloc(block[0], size);
        if (0 == block[0]) {
            errorf("%s[%d]: (%p==%p[%lu])!\n",
                __FILE__, __LINE__, 0, block[0], size);
            ++errors;
        }
        actual = heap.size(block[0]);
        if (size != actual) {
            errorf("%s[%d]: (%u!=%u[%u])!\n",
                __FILE__, __LINE__, size, actual, size);
            ++errors;
        }
        cp = staticcastto(uint8_t*,block[0]);
        for (int ii = 0; staticcastto(int,set) > ii; ++ii) {
            if (get != cp[ii]) {
                errorf("%s[%d]: (%lu!=%lu[%lu])!\n",
                    __FILE__, __LINE__, get, cp[ii], ii);
                ++errors;
            }
        }
        get = set;
        for (int ii = 0; staticcastto(int,size) > ii; ++ii) {
            cp[ii] = set;
        }
        set = size;
        total += actual;
        printf("%3d 0x%08x %10u %10u %10u %10u\n",
            size, block[0], size, actual, get, set);
    }

    heap.free(block[0]);

    heap.show();

    current = heap.getCurrent();
    if (0 != current) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, 0, current);
        ++errors;
    }


    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
