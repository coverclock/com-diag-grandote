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
 *  Implements the Number unit test.
 *
 *  @see    Number
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Number.h"
#include "com/diag/desperado/Number.h"
#include "com/diag/desperado/int16_Number.h"
#include "com/diag/desperado/int16_Number.h"
#include "com/diag/desperado/int32_Number.h"
#include "com/diag/desperado/int32_Number.h"
#include "com/diag/desperado/int64_Number.h"
#include "com/diag/desperado/int64_Number.h"
#include "com/diag/desperado/int8_Number.h"
#include "com/diag/desperado/int8_Number.h"
#include "com/diag/desperado/uint16_Number.h"
#include "com/diag/desperado/uint16_Number.h"
#include "com/diag/desperado/uint32_Number.h"
#include "com/diag/desperado/uint32_Number.h"
#include "com/diag/desperado/uint64_Number.h"
#include "com/diag/desperado/uint64_Number.h"
#include "com/diag/desperado/uint8_Number.h"
#include "com/diag/desperado/uint8_Number.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

// #define  DEBUG_UNIT_TEST_NUMBER  (65536 / 13)

static int udecode(
    const char* string,
    uint16_t expectedresult,
    bool expectedvalid,
    bool limitsize
) {
    Print errorf(Platform::instance().error());
    int errors = 0;

    bool valid;
    uint16_t result;
    size_t length;
    size_t stringlength = std::strlen(string) + 1;

    valid = limitsize ?
        ::uint16_Number(string, result, length, stringlength - 1) :
        ::uint16_Number(string, result, length);

#if defined(DEBUG_UNIT_TEST_NUMBER)
    Print printf(Platform::instance().output());
    if (0 == ((uint16_t)result % DEBUG_UNIT_TEST_NUMBER)) {
        printf("%s[%d]:%6d %5u 0x%04x 0%06o [%u] \"%s\"[%u] %s\n",
            __FILE__, __LINE__,
            result, result, result, result,
            length,
            string, stringlength,
            valid ? "" : (length == stringlength) ? "overflow" : "syntax" );
    }
#endif

    if (valid != expectedvalid) {
        errorf("%s[%d]: \"%s\" 0x%x %s (%d!=%d)!\n",
            __FILE__, __LINE__, string, result,
            valid ? "" : (stringlength == length) ? "overflow" : "syntax",
            valid, expectedvalid);
        ++errors;
    }

    if (result != expectedresult) {
        errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, string, result, result, expectedresult);
        ++errors;
    }

    if (!expectedvalid) {
        ;
    } else if (limitsize) {
        ;
    } else if (' ' == *(string + length - 1)) {
        ;
    } else if ('\t' == *(string + length - 1)) {
        ;
    } else if ('\0' == *(string + length - 1)) {
        ;
    } else {
        errorf("%s[%d]: \"%s\" 0x%x length %u 0x%x!\n",
            __FILE__, __LINE__, string, result, length, *(string + length - 1));
        ++errors;
    }

    return errors;
}

static int uencode(const char* format, uint16_t value, uint16_t expected,
        bool valid) {
    int errors = 0;
    char string[64];

    std::sprintf(string, format, value);
    errors += udecode(string, expected, valid, false);
    errors += udecode(string, expected, valid, true);

    return errors;
}

static int utest(uint16_t value) {
    int errors = 0;

    errors += uencode("%d", value, value, true);
    errors += uencode("%u", value, value, true);
    errors += uencode("0x%hx", value, value, true);
    errors += uencode("0x%hX", value, value, true);
    errors += uencode("0%ho", value, value, true);

    errors += uencode("  \t\t  \t\t%d  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t%u  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t0x%hx  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t0x%hX  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t0%ho  \t\t  \t\t", value, value, true);

    errors += uencode("%d 0", value, value, true);
    errors += uencode("%u 0", value, value, true);
    errors += uencode("0x%hx 0", value, value, true);
    errors += uencode("0x%hX 0", value, value, true);
    errors += uencode("0%ho 0", value, value, true);

    errors += uencode("%6d", value, value, true);
    errors += uencode("%6u", value, value, true);
    errors += uencode("0x%04hx", value, value, true);
    errors += uencode("0x%04hX", value, value, true);
    errors += uencode("0%06ho", value, value, true);

    errors += uencode("  \t\t  \t\t%6d  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t%6u  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t0x%04hx  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t0x%04hX  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t0%06ho  \t\t  \t\t", value, value, true);

    errors += uencode("%6d 0", value, value, true);
    errors += uencode("%6u 0", value, value, true);
    errors += uencode("0x%04hx 0", value, value, true);
    errors += uencode("0x%04hX 0", value, value, true);
    errors += uencode("0%06ho 0", value, value, true);

    errors += uencode("-%u", value, -value, true);
    errors += uencode("-0x%hx", value, -value, true);
    errors += uencode("-0x%hX", value, -value, true);
    errors += uencode("-0%ho", value, -value, true);

    errors += uencode("  \t\t  \t\t-%u  \t\t  \t\t", value, -value, true);
    errors += uencode("  \t\t  \t\t-0x%hx  \t\t  \t\t", value, -value, true);
    errors += uencode("  \t\t  \t\t-0x%hX  \t\t  \t\t", value, -value, true);
    errors += uencode("  \t\t  \t\t-0%ho  \t\t  \t\t", value, -value, true);

    errors += uencode("-%u 0", value, -value, true);
    errors += uencode("-0x%hx 0", value, -value, true);
    errors += uencode("-0x%hX 0", value, -value, true);
    errors += uencode("-0%ho 0", value, -value, true);

    errors += uencode("-0x%04hx", value, -value, true);
    errors += uencode("-0x%04hX", value, -value, true);
    errors += uencode("-0%06ho", value, -value, true);

    errors += uencode("  \t\t  \t\t-0x%04hx  \t\t  \t\t", value, -value, true);
    errors += uencode("  \t\t  \t\t-0x%04hX  \t\t  \t\t", value, -value, true);
    errors += uencode("  \t\t  \t\t-0%06ho  \t\t  \t\t", value, -value, true);

    errors += uencode("-0x%04hx 0", value, -value, true);
    errors += uencode("-0x%04hX 0", value, -value, true);
    errors += uencode("-0%06ho 0", value, -value, true);

    errors += uencode("+0x%hx", value, value, true);
    errors += uencode("+0x%hX", value, value, true);
    errors += uencode("+0%ho", value, value, true);

    errors += uencode("  \t\t  \t\t+%u  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t+0x%hx  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t+0x%hX  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t+0%ho  \t\t  \t\t", value, value, true);

    errors += uencode("+0x%hx 0", value, value, true);
    errors += uencode("+0x%hX 0", value, value, true);
    errors += uencode("+0%ho 0", value, value, true);

    errors += uencode("+0x%04hx", value, value, true);
    errors += uencode("+0x%04hX", value, value, true);
    errors += uencode("+0%06ho", value, value, true);

    errors += uencode("  \t\t  \t\t+0x%04hx  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t+0x%04hX  \t\t  \t\t", value, value, true);
    errors += uencode("  \t\t  \t\t+0%06ho  \t\t  \t\t", value, value, true);

    errors += uencode("+0x%04hx 0", value, value, true);
    errors += uencode("+0x%04hX 0", value, value, true);
    errors += uencode("+0%06ho 0", value, value, true);

    errors += uencode("%d.", value, value, false);
    errors += uencode("%u,", value, value, false);
    errors += uencode("0x%hxk", value, value, false);
    errors += uencode("0x%hXK", value, value, false);
    errors += uencode("0%ho8", value, value, false);

    errors += uencode("  \t\t  \t\t%d.  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t%u,  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t0x%hxk  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t0x%hXK  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t0%ho8  \t\t  \t\t", value, value, false);

    errors += uencode("%6d.", value, value, false);
    errors += uencode("%6u,", value, value, false);
    errors += uencode("0x%04hxk", value, value, false);
    errors += uencode("0x%04hXK", value, value, false);
    errors += uencode("0%06ho8", value, value, false);

    errors += uencode("  \t\t  \t\t%6d.  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t%6u,  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t0x%04hxk  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t0x%04hXK  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t0%06ho8  \t\t  \t\t", value, value, false);

    errors += uencode("-%u.", value, -value, false);
    errors += uencode("-0x%hxk", value, -value, false);
    errors += uencode("-0x%hXK", value, -value, false);
    errors += uencode("-0%ho8", value, -value, false);

    errors += uencode("  \t\t  \t\t-%u.  \t\t  \t\t", value, -value, false);
    errors += uencode("  \t\t  \t\t-0x%hxk  \t\t  \t\t", value, -value, false);
    errors += uencode("  \t\t  \t\t-0x%hXK  \t\t  \t\t", value, -value, false);
    errors += uencode("  \t\t  \t\t-0%ho8  \t\t  \t\t", value, -value, false);

    errors += uencode("-0x%04hxk", value, -value, false);
    errors += uencode("-0x%04hXK", value, -value, false);
    errors += uencode("-0%06ho8", value, -value, false);

    errors += uencode("  \t\t  \t\t-0x%04hxk \t\t  \t\t", value, -value, false);
    errors += uencode("  \t\t  \t\t-0x%04hXK \t\t  \t\t", value, -value, false);
    errors += uencode("  \t\t  \t\t-0%06ho8  \t\t  \t\t", value, -value, false);

    errors += uencode("+0x%hxk", value, value, false);
    errors += uencode("+0x%hxK", value, value, false);
    errors += uencode("+0%ho8", value, value, false);

    errors += uencode("  \t\t  \t\t+%u.  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t+0x%hxk  \t\t  \t\t", value, value, false);
    errors += uencode("  \t\t  \t\t+0x%hXK  \t\t  \t\t", value, value, false);

    return errors;
}

static int sdecode(
    const char* string,
    int16_t expectedresult,
    bool expectedvalid,
    bool limitsize
) {
    Print errorf(Platform::instance().error());
    int errors = 0;

    bool valid;
    int16_t result;
    size_t length;
    size_t stringlength = std::strlen(string) + 1;

    valid = limitsize ?
       ::int16_Number(string, result, length, stringlength - 1) :
       ::int16_Number(string, result, length);

#if defined(DEBUG_UNIT_TEST_NUMBER)
    if (0 == ((uint16_t)result % DEBUG_UNIT_TEST_NUMBER)) {
        Print printf(Platform::instance().output());
        printf("%s[%d]:"
            "%6d %5u 0x%04x 0%06o [%u] \"%s\"[%u] %s\n",
            __FILE__, __LINE__,
            result, result, result, result,
            length,
            string, stringlength,
            valid ? "" : (length == stringlength) ? "overflow" : "syntax" );
    }
#endif

    if (valid != expectedvalid) {
        errorf("%s[%d]: \"%s\" 0x%x %s (%d!=%d)!\n",
            __FILE__, __LINE__, string, result,
            valid ? "" : (stringlength == length) ? "overflow" : "syntax",
            valid, expectedvalid);
        ++errors;
    }

    if (result != expectedresult) {
        errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, string, result, result, expectedresult);
        ++errors;
    }

    if (!expectedvalid) {
        ;
    } else if (limitsize) {
        ;
    } else if (' ' == *(string + length - 1)) {
        ;
    } else if ('\t' == *(string + length - 1)) {
        ;
    } else if ('\0' == *(string + length - 1)) {
        ;
    } else {
        errorf("%s[%d]: \"%s\" 0x%x length %u 0x%x!\n",
            __FILE__, __LINE__, string, result, length, *(string + length - 1));
        ++errors;
    }

    return errors;
}

static int sencode(const char* format, int16_t value, int16_t expected,
        bool valid) {
    int errors = 0;
    char string[64];

    std::sprintf(string, format, value);
    errors += sdecode(string, expected, valid, false);
    errors += sdecode(string, expected, valid, true);

    return errors;
}

static int stest(int16_t value) {
    int errors = 0;

    errors += sencode("%hd", value, value, true);
    errors += sencode("0x%hx", value, value, true);
    errors += sencode("0x%hX", value, value, true);
    errors += sencode("0%ho", value, value, true);

    errors += sencode("  \t\t  \t\t%hd  \t\t  \t\t", value, value, true);
    errors += sencode("  \t\t  \t\t0x%hx  \t\t  \t\t", value, value, true);
    errors += sencode("  \t\t  \t\t0x%hX  \t\t  \t\t", value, value, true);
    errors += sencode("  \t\t  \t\t0%ho  \t\t  \t\t", value, value, true);

    errors += sencode("%hd 0", value, value, true);
    errors += sencode("0x%hx 0", value, value, true);
    errors += sencode("0x%hX 0", value, value, true);
    errors += sencode("0%ho 0", value, value, true);

    errors += sencode("%6hd", value, value, true);
    errors += sencode("0x%04hx", value, value, true);
    errors += sencode("0x%04hX", value, value, true);
    errors += sencode("0%06ho", value, value, true);

    errors += sencode("  \t\t  \t\t%6hd  \t\t  \t\t", value, value, true);
    errors += sencode("  \t\t  \t\t0x%04hx  \t\t  \t\t", value, value, true);
    errors += sencode("  \t\t  \t\t0x%04hX  \t\t  \t\t", value, value, true);
    errors += sencode("  \t\t  \t\t0%06ho  \t\t  \t\t", value, value, true);

    errors += sencode("%6hd 0", value, value, true);
    errors += sencode("0x%04hx 0", value, value, true);
    errors += sencode("0x%04hX 0", value, value, true);
    errors += sencode("0%06ho 0", value, value, true);

    errors += sencode("%hd.", value, value, false);
    errors += sencode("0x%hxk", value, value, false);
    errors += sencode("0x%hXK", value, value, false);
    errors += sencode("0%ho8", value, value, false);

    errors += sencode("  \t\t  \t\t%hd.  \t\t  \t\t", value, value, false);
    errors += sencode("  \t\t  \t\t0x%hxk  \t\t  \t\t", value, value, false);
    errors += sencode("  \t\t  \t\t0x%hXK  \t\t  \t\t", value, value, false);
    errors += sencode("  \t\t  \t\t0%ho8  \t\t  \t\t", value, value, false);

    errors += sencode("%6hd.", value, value, false);
    errors += sencode("0x%04hxk", value, value, false);
    errors += sencode("0x%04hXK", value, value, false);
    errors += sencode("0%06ho8", value, value, false);

    errors += sencode("  \t\t  \t\t%6hd.  \t\t  \t\t", value, value, false);
    errors += sencode("  \t\t  \t\t0x%04hxk  \t\t  \t\t", value, value, false);
    errors += sencode("  \t\t  \t\t0x%04hXK  \t\t  \t\t", value, value, false);
    errors += sencode("  \t\t  \t\t0%06ho8  \t\t  \t\t", value, value, false);

    return errors;
}

CXXCAPI int unittestNumber(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    printf("%s[%d]: uint8_t\n", __FILE__, __LINE__);
    ::uint8_Number.show();

    printf("%s[%d]: int8_t\n", __FILE__, __LINE__);
    ::int8_Number.show();

    printf("%s[%d]: uint16_t\n", __FILE__, __LINE__);
    ::uint16_Number.show();

    printf("%s[%d]: int6_t\n", __FILE__, __LINE__);
    ::int16_Number.show();

    printf("%s[%d]: uint32_t\n", __FILE__, __LINE__);
    ::uint32_Number.show();

    printf("%s[%d]: int32_t\n", __FILE__, __LINE__);
    ::int32_Number.show();

    printf("%s[%d]: uint64_t\n", __FILE__, __LINE__);
    ::uint64_Number.show();

    printf("%s[%d]: int64_t\n", __FILE__, __LINE__);
    ::int64_Number.show();

    //  Do some basic sanity tests of all the stdint-based parsers.

    printf("%s[%d]: sanity\n", __FILE__, __LINE__);

    {
        const char* string ="0xfe";
        uint8_t expectedresult = 0xfeU;
        uint8_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = uint8_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%x validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%x length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    {
        const char* string ="-128";
        int8_t expectedresult = -128;
        int8_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = int8_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%x validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%x length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    {
        const char* string ="0XFEDC";
        uint16_t expectedresult = 0XFEDCU;
        uint16_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = uint16_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%x validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%x length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    {
        const char* string ="-32768";
        int16_t expectedresult = -32768;
        int16_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = int16_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%x validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%x length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    {
        const char* string ="0xfedcba98";
        uint32_t expectedresult = 0xfedcba98UL;
        uint32_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = uint32_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%lx validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf(
                "%s[%d]: \"%s\" 0x%lx result (0x%lx!=0x%lx)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%lx length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    {
#if 0
        const char* string ="-2147483648";     // Uexpected compiler warning
        int32_t expectedresult = -2147483648L; // occurred for this value.
#else
        const char* string ="-2147483647";
        int32_t expectedresult = -2147483647L;
#endif
        int32_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = int32_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%lx validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf(
                "%s[%d]: \"%s\" 0x%lx result (0x%lx!=0x%lx)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%lx length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    {
        const char* string ="0xFEDCBA9876543210";
        uint64_t expectedresult = 0xFEDCBA9876543210ULL;
        uint64_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = uint64_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%llx validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf(
                "%s[%d]: \"%s\" 0x%llx result (0x%llx!=0x%llx)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%llx length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    {
        const char* string ="-9223372036854775808";
        int64_t expectedresult = -9223372036854775808ULL;
        int64_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = int64_Number(string, result, length);
        if (!valid) {
            errorf("%s[%d]: \"%s\" 0x%llx validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, true);
            ++errors;
        }
        if (result != expectedresult) {
            errorf(
                "%s[%d]: \"%s\" 0x%llx result (0x%llx!=0x%llx)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%llx length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    //  Test basic unsigned overflow detection.

    printf("%s[%d]: unsigned overflow\n", __FILE__, __LINE__);

    {
        const char* string ="0x01c5";
        uint8_t expectedresult = 0xc5;
        uint8_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = uint8_Number(string, result, length);
        if (valid) {
            errorf("%s[%d]: \"%s\" 0x%x validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, false);
            ++errors;
        }
        if (result != expectedresult) {
            errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%x length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    //  Test basic signed overflow detection.

    printf("%s[%d]: signed overflow\n", __FILE__, __LINE__);

    {
        const char* string ="128";
        int8_t expectedresult = -128;
        int8_t result;
        size_t length;
        size_t expectedlength = std::strlen(string) + 1;
        bool valid;

        valid = int8_Number(string, result, length);
        if (valid) {
            errorf("%s[%d]: \"%s\" 0x%x validity (%d!=%d)!\n",
                __FILE__, __LINE__, string, result, valid, false);
            ++errors;
        }
        if (result != expectedresult) {
            errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
                __FILE__, __LINE__, string, result, result, expectedresult);
            ++errors;
        }
        if (length != expectedlength) {
            errorf("%s[%d]: \"%s\" 0x%x length (%u!=%u)!\n",
                __FILE__, __LINE__, string, result, length, expectedlength);
            ++errors;
        }
    }

    //  Test pre-defined parsers.

    printf("%s[%d]: predefined parsers\n", __FILE__, __LINE__);

    ::int8_Number.show();
    ::int16_Number.show();
    ::int32_Number.show();
    ::int64_Number.show();

    ::uint8_Number.show();
    ::uint16_Number.show();
    ::uint32_Number.show();
    ::uint64_Number.show();

    //  Test string length.

    printf("%s[%d]: string length\n", __FILE__, __LINE__);

    {

        const char* string = "-123456789";
        int32_t result;
        size_t length;
        int32_t expectedresult[] = {
            0,
            0,
            -1,
            -12,
            -123,
            -1234,
            -12345,
            -123456,
            -1234567,
            -12345678,
            -123456789,
            -123456789,
            -123456789,
            -123456789,
            -123456789
        };
        bool valid;

        for (size_t ii = 0; countof(expectedresult) > ii; ++ii) {
            valid = int32_Number(string, result, length, ii);
            if (valid != (expectedresult[ii] != 0)) {
                errorf("%s[%d]: \"%s\" 0x%x validity (%d!=%d)!\n",
                    __FILE__, __LINE__, string, result,
                    valid, (expectedresult[ii] != 0));
                ++errors;
            }
            if (result != expectedresult[ii]) {
                errorf("%s[%d]: \"%s\" 0x%x result (0x%x!=0x%x)!\n",
                    __FILE__, __LINE__, string, result,
                    result, expectedresult[ii]);
                ++errors;
            }
            size_t stringlength = sizeof("-123456789");
            if ((length != ii) && (length != stringlength)) {
                errorf("%s[%d]: \"%s\" 0x%x length (%u!=%u)!\n",
                    __FILE__, __LINE__, string, result,
                    length, ii);
                ++errors;
            }
        }

    }

    //  We're cheating here by exhaustively testing only the
    //  16-bit parser. We test both the signed and the unsigned
    //  version; and all eight parsers (really, four) use the exact
    //  same template code, differing only in the word length; and
    //  it takes too long to test the 32-bit parser; and forget
    //  exhaustively testing the 64-bit parser in your lifetime.

    uint16_t uvalue;
    uint16_t uminimum = intminof(uint16_t);
    uint16_t umaximum = intmaxof(uint16_t);
    printf("%s[%d]: uint16_t [0x%04hx..0x%04hx]\n",
        __FILE__, __LINE__, uminimum, umaximum);

    for (uint32_t uindex = uminimum; uindex <= umaximum; ++uindex) {
        uvalue = uindex;
        errors += utest(uvalue);
    }

    int16_t svalue;
    int16_t sminimum = intminof(int16_t);
    int16_t smaximum = intmaxof(int16_t);
    printf("%s[%d]: int16_t [0x%04hx..0x%04hx]\n",
        __FILE__, __LINE__, sminimum, smaximum);

    for (int32_t sindex = sminimum; sindex <= smaximum; ++sindex) {
        svalue = sindex;
        errors += stest(svalue++);
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
