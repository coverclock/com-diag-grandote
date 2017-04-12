/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2009-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the ByteOrder unit test.
 *
 *  @see    ByteOrder
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/ByteOrder.h"
#include "com/diag/desperado/ByteOrder.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Desperado.h"


template <typename _TYPE_>
static int testMustSwap(const char * file, int line) {
    Print printf(Platform::instance().output());
    if (ByteOrder<_TYPE_, _TYPE_, _TYPE_>::mustswap()) {
        printf("%s[%d]: little endian\n", file, line);
    } else {
        printf("%s[%d]: big endian\n", file, line);
    }
    return 0;
}

template <typename _TYPE_>
static int testIntegerSanity(const char * file, int line, _TYPE_ input) {
    Print errorf(Platform::instance().error());
    Dump dump(Platform::instance().error());
    int errors = 0;
    _TYPE_ temp = hton(input);
    _TYPE_ output = ntoh(temp);
    if (input != output) {
        errorf("%s[%d]: failed!\n", file, line);
        dump.bytes(&input, sizeof(input));
        dump.bytes(&temp, sizeof(temp));
        dump.bytes(&output, sizeof(output));
        ++errors;
    }
    return errors;
}

template <typename _HOST_, typename _NETWORK_>
static int testFloatSanity(const char * file, int line, _HOST_ input) {
    Print errorf(Platform::instance().error());
    Dump dump(Platform::instance().error());
    int errors = 0;
    _NETWORK_ temp = fhton(input);
    _HOST_ output = fntoh(temp);
    if (input != output) {
        errorf("%s[%d]: failed!\n", file, line);
        dump.bytes(&input, sizeof(input));
        dump.bytes(&temp, sizeof(temp));
        dump.bytes(&output, sizeof(output));
        ++errors;
    }
    return errors;
}

template <typename _TYPE_>
static int testIntegerSwap(const char * file, int line, _TYPE_ from, _TYPE_ to) {
    Print errorf(Platform::instance().error());
    Dump dump(Platform::instance().error());
    int errors = 0;
    _TYPE_ temp = ByteOrder<_TYPE_, _TYPE_, _TYPE_>::swap(from);
    if (temp != to) {
        errorf("%s[%d]: failed!\n", file, line);
        dump.bytes(&from, sizeof(from));
        dump.bytes(&temp, sizeof(temp));
        dump.bytes(&to, sizeof(to));
        ++errors;
    }
    return errors;
}

template <typename _INPUT_, typename _OUTPUT_, typename _CONTROL_>
static int testFloatSwap(const char * file, int line, _INPUT_ from, _OUTPUT_ to) {
    Print errorf(Platform::instance().error());
    Dump dump(Platform::instance().error());
    int errors = 0;
    _OUTPUT_ temp = ByteOrder<_INPUT_, _OUTPUT_, _CONTROL_>::swap(from);
    if (temp != to) {
        errorf("%s[%d]: failed!\n", file, line);
        dump.bytes(&from, sizeof(from));
        dump.bytes(&temp, sizeof(temp));
        dump.bytes(&to, sizeof(to));
        ++errors;
    }
    return errors;
}

CXXCAPI int unittestByteOrder(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    errors += testMustSwap<int64_t>
        (__FILE__, __LINE__);
    errors += testMustSwap<int32_t>
        (__FILE__, __LINE__);
    errors += testMustSwap<int16_t>
        (__FILE__, __LINE__);
    errors += testMustSwap<int8_t>
        (__FILE__, __LINE__);

    errors += testMustSwap<uint64_t>
        (__FILE__, __LINE__);
    errors += testMustSwap<uint32_t>
        (__FILE__, __LINE__);
    errors += testMustSwap<uint16_t>
        (__FILE__, __LINE__);
    errors += testMustSwap<uint8_t>
        (__FILE__, __LINE__);

    errors += testIntegerSanity<int64_t>
        (__FILE__, __LINE__, 0x0123456789abcdefLL);
    errors += testIntegerSanity<int32_t>
        (__FILE__, __LINE__, 0x01234567L);
    errors += testIntegerSanity<int16_t>
        (__FILE__, __LINE__, 0x0123);
    errors += testIntegerSanity<int8_t>
        (__FILE__, __LINE__, 0x01);

    errors += testIntegerSanity<uint64_t>
        (__FILE__, __LINE__, 0x0123456789abcdefULL);
    errors += testIntegerSanity<uint32_t>
        (__FILE__, __LINE__, 0x01234567UL);
    errors += testIntegerSanity<uint16_t>
        (__FILE__, __LINE__, 0x0123U);
    errors += testIntegerSanity<uint8_t>
        (__FILE__, __LINE__, 0x01U);

    errors += testFloatSanity<float64_t, uint64_t>
        (__FILE__, __LINE__, 2.7182818284590452354);
    errors += testFloatSanity<float32_t, uint32_t>
        (__FILE__, __LINE__, 3.14159265358979323846);

    errors += testIntegerSanity<signed long long>
		(__FILE__, __LINE__, 0x0123456789abcdefLL);
    errors += testIntegerSanity<signed long>
		(__FILE__, __LINE__, 0x01234567L);
    errors += testIntegerSanity<signed int>
		(__FILE__, __LINE__, 0x01234567L);
    errors += testIntegerSanity<signed short>
		(__FILE__, __LINE__, 0x0123);
    errors += testIntegerSanity<signed char>
		(__FILE__, __LINE__, 0x01);

    errors += testIntegerSanity<unsigned long long>
		(__FILE__, __LINE__, 0x0123456789abcdefULL);
    errors += testIntegerSanity<unsigned long>
		(__FILE__, __LINE__, 0x01234567UL);
    errors += testIntegerSanity<unsigned int>
		(__FILE__, __LINE__, 0x01234567UL);
    errors += testIntegerSanity<unsigned short>
		(__FILE__, __LINE__, 0x0123U);
    errors += testIntegerSanity<unsigned char>
		(__FILE__, __LINE__, 0x01U);

    errors += testFloatSanity<double, uint64_t>
        (__FILE__, __LINE__, 2.7182818284590452354);
    errors += testFloatSanity<float, uint32_t>
        (__FILE__, __LINE__, 3.14159265358979323846);

    errors += testIntegerSwap<uint64_t>
        (__FILE__, __LINE__, 0x0123456789abcdefULL, 0xefcdab8967452301ULL);
    errors += testIntegerSwap<uint32_t>
        (__FILE__, __LINE__, 0x01234567UL, 0x67452301UL);
    errors += testIntegerSwap<uint16_t>
        (__FILE__, __LINE__, 0x0123U, 0x2301U);
    errors += testIntegerSwap<uint8_t>
        (__FILE__, __LINE__, 0x01U, 0x01U);

    errors += testIntegerSwap<int64_t>
        (__FILE__, __LINE__, 0x0123456789abcdefLL, 0xefcdab8967452301LL);
    errors += testIntegerSwap<int32_t>
        (__FILE__, __LINE__, 0x01234567L, 0x67452301L);
    errors += testIntegerSwap<int16_t>
        (__FILE__, __LINE__, 0x0123, 0x2301);
    errors += testIntegerSwap<int8_t>
        (__FILE__, __LINE__, 0x01, 0x01);

    // I thought these should work as long as the target implements
    // IEEE 754 Floating Point. If it doesn't, there is not much
    // point in pushing floats across the network unless it is to
    // exactly the same architecture. And if that's the case, there
    // is probably no point in swapping the bytes. Wikipedia:Endianess
    // suggests that the situation is even more complicated than this,
    // so disable this code if it doesn't fly on your target.

#if !0
    errors += testFloatSwap<float64_t, uint64_t, uint64_t>
        (__FILE__, __LINE__, 2.7182818284590452354, 0x6957148b0abf0540ULL);
    errors += testFloatSwap<float32_t, uint32_t, uint32_t>
        (__FILE__, __LINE__, 3.14159265358979323846, 0xdb0f4940UL);
    errors += testFloatSwap<uint64_t, float64_t, uint64_t>
        (__FILE__, __LINE__, 0x6957148b0abf0540ULL, 2.7182818284590452354);
    errors += testFloatSwap<uint32_t, float32_t, uint32_t>
        (__FILE__, __LINE__, 0xdb0f4940UL, 3.14159265358979323846);
#endif

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__,
        errors);

    return errors;
}
