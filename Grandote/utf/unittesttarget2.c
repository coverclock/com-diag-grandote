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
 *  Implements the target2 unit test.
 *
 *  @see    target.h
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/littleendian.h"
#include "com/diag/desperado/littleendian.h"
#include "com/diag/desperado/lowtohigh.h"
#include "com/diag/desperado/lowtohigh.h"
#include "com/diag/desperado/bool.h"
#include "com/diag/desperado/bool.h"
#undef DESPERADO_HAS_DEBUGGING
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/debug.h"
#define DESPERADO_HAS_DEBUGGING
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/debug.h"
#undef DESPERADO_HAS_DEBUGGING
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/debug.h"
#include "com/diag/desperado/reads.h"
#include "com/diag/desperado/reads.h"
#include "com/diag/desperado/writes.h"
#include "com/diag/desperado/writes.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/assert.h"
#include "com/diag/desperado/assert.h"

int unittesttarget2() {
    int errors = 0;
    const unsigned char ones[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    const uint8_t  cu8  = ~(uint8_t)(0);
    const uint16_t cu16 = ~(uint16_t)(0);
    const uint32_t cu32 = ~(uint32_t)(0);
    const uint64_t cu64 = ~(uint64_t)(0);
    const int8_t  cs8  = -(int8_t)(1);
    const int16_t cs16 = -(int16_t)(1);
    const int32_t cs32 = -(int32_t)(1);
    const int64_t cs64 = -(int64_t)(1);
    const uintptr_t cptr = (uintptr_t)(0);
    int little;
    int low;
    bool truth = true;
    bool fiction = false;
    bool nottruth = !true;
    bool notfiction = !false;

    print_f(platform_output(), "%s[%d]: begin\n",
        __FILE__, __LINE__);

    print_f(platform_output(), "%s[%d]: byte order\n", __FILE__, __LINE__);

    little = littleendian();
    print_f(platform_output(), "%s endian\n", little ? "little" : "big");

#if defined(DESPERADO_HAS_LITTLE_ENDIAN)
    if (!little) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)\n",
            __FILE__, __LINE__, little, 1);
        ++errors;
    }
#elif defined(DESPERADO_HAS_BIG_ENDIAN)
    if (little) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)\n",
            __FILE__, __LINE__, little, 0);
        ++errors;
    }
#else
    print_f(platform_error(), "%s[%d]: byte order unknown\n"
        __FILE__, __LINE__);
    ++errors;
#endif

    print_f(platform_output(), "%s[%d]: bitfield order\n", __FILE__, __LINE__);

    low = lowtohigh();
    print_f(platform_output(), "%s\n", low ? "low to high" : "high to low");

#if defined(DESPERADO_HAS_BITFIELDS_LOW_TO_HIGH)
    if (!low) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)\n",
            __FILE__, __LINE__, low, 1);
        ++errors;
    }
#elif defined(DESPERADO_HAS_BITFIELDS_HIGH_TO_LOW)
    if (low) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)\n",
            __FILE__, __LINE__, low, 0);
        ++errors;
    }
#else
    print_f(platform_error(), "%s[%d]: bitfield order unknown\n"
        __FILE__, __LINE__);
    ++errors;
#endif

    print_f(platform_output(), "%s[%d]: types\n",
        __FILE__, __LINE__);

    if (8 != (sizeof(cu8) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, (sizeof(cu8) * 8));
        ++errors;
    }

    if (16 != (sizeof(cu16) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 16, (sizeof(cu16) * 8));
        ++errors;
    }

    if (32 != (sizeof(cu32) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 32, (sizeof(cu32) * 8));
        ++errors;
    }

    if (64 != (sizeof(cu64) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 64, (sizeof(cu64) * 8));
        ++errors;
    }

    if (8 != (sizeof(cs8) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, (sizeof(cs8) * 8));
        ++errors;
    }

    if (16 != (sizeof(cs16) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 16, (sizeof(cs16) * 8));
        ++errors;
    }

    if (32 != (sizeof(cs32) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 32, (sizeof(cs32) * 8));
        ++errors;
    }

    if (64 != (sizeof(cs64) * 8)) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 64, (sizeof(cs64) * 8));
        ++errors;
    }

    if (sizeof(void*) < (sizeof(cptr))) {
        print_f(platform_error(), "%s[%d]: (%u<%u)!\n",
            __FILE__, __LINE__, sizeof(void*), sizeof(cptr));
        ++errors;
    }

    if (0 != memcmp(&cu8, ones, sizeof(cu8))) {
        print_f(platform_error(), "%s[%d]: 0x%x!\n",
            __FILE__, __LINE__, cu8);
        dump_bytes(platform_dump(), &cu8, sizeof(cu8));
        ++errors;
    }

    if (0 != memcmp(&cu16, ones, sizeof(cu16))) {
        print_f(platform_error(), "%s[%d]: 0x%x!\n",
            __FILE__, __LINE__, cu16);
        dump_bytes(platform_dump(), &cu16, sizeof(cu16));
        ++errors;
    }

    if (0 != memcmp(&cu32, ones, sizeof(cu32))) {
        print_f(platform_error(), "%s[%d]: 0x%lx!\n",
            __FILE__, __LINE__, cu32);
        dump_bytes(platform_dump(), &cu32, sizeof(cu32));
        ++errors;
    }

    if (0 != memcmp(&cu64, ones, sizeof(cu64))) {
        print_f(platform_error(), "%s[%d]: 0x%llx!\n",
            __FILE__, __LINE__, cu64);
        dump_bytes(platform_dump(), &cu64, sizeof(cu64));
        ++errors;
    }

    if (0 != memcmp(&cs8, ones, sizeof(cs8))) {
        print_f(platform_error(), "%s[%d]: 0x%x!\n",
            __FILE__, __LINE__, cs8);
        dump_bytes(platform_dump(), &cs8, sizeof(cs8));
        ++errors;
    }

    if (0 != memcmp(&cs16, ones, sizeof(cs16))) {
        print_f(platform_error(), "%s[%d]: 0x%x!\n",
            __FILE__, __LINE__, cs16);
        dump_bytes(platform_dump(), &cs16, sizeof(cs16));
        ++errors;
    }

    if (0 != memcmp(&cs32, ones, sizeof(cs32))) {
        print_f(platform_error(), "%s[%d]: 0x%lx!\n",
            __FILE__, __LINE__, cs32);
        dump_bytes(platform_dump(), &cs32, sizeof(cs32));
        ++errors;
    }

    if (0 != memcmp(&cs64, ones, sizeof(cs64))) {
        print_f(platform_error(), "%s[%d]: 0x%llx!\n",
            __FILE__, __LINE__, cs64);
        dump_bytes(platform_dump(), &cs64, sizeof(cs64));
        ++errors;
    }

    if (0xffULL != cu8) {
        print_f(platform_error(), "%s[%d]: (%llu!=%u)!\n",
            __FILE__, __LINE__, 0xffULL, cu8);
        ++errors;
    }

    if (0xffffULL != cu16) {
        print_f(platform_error(), "%s[%d]: (%llu!=%u)!\n",
            __FILE__, __LINE__, 0xffffULL, cu16);
        ++errors;
    }

    if (0xffffffffULL != cu32) {
        print_f(platform_error(), "%s[%d]: (%llu!=%lu)!\n",
            __FILE__, __LINE__, 0xffffffffULL, cu32);
        ++errors;
    }

    if (0xffffffffffffffffULL != cu64) {
        print_f(platform_error(), "%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, 0xffffffffffffffffULL, cu64);
        ++errors;
    }

    if (-1LL != cs8) {
        print_f(platform_error(), "%s[%d]: (%lld!=%d)!\n",
            __FILE__, __LINE__, -1LL, cs8);
        ++errors;
    }

    if (-1LL != cs16) {
        print_f(platform_error(), "%s[%d]: (%lld!=%d)!\n",
            __FILE__, __LINE__, -1LL, cs16);
        ++errors;
    }

    if (-1LL != cs32) {
        print_f(platform_error(), "%s[%d]: (%lld!=%ld)!\n",
            __FILE__, __LINE__, -1LL, cs32);
        ++errors;
    }

    if (-1LL != cs64) {
        print_f(platform_error(), "%s[%d]: (%lld!=%lld)!\n",
            __FILE__, __LINE__, -1LL, cs64);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: bool\n", __FILE__, __LINE__);

    if (!truth) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, truth, true);
        ++errors;
    }

    if (fiction) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, fiction, false);
        ++errors;
    }

    if (!truth) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, truth, true);
        ++errors;
    }

    if (fiction) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, fiction, false);
        ++errors;
    }

    if (!notfiction) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, notfiction, true);
        ++errors;
    }

    if (nottruth) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, nottruth, false);
        ++errors;
    }

    if (truth != notfiction) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, truth, notfiction);
        ++errors;
    }

    if (fiction != nottruth) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, fiction, nottruth);
        ++errors;
    }

    if (truth != (1 == 1)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, truth, (1 == 1));
        ++errors;
    }

    if (fiction != (1 == 0)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, fiction, (1 == 0));
        ++errors;
    }

    if (truth != (!!2)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, truth, (!!2));
        ++errors;
    }

    if (fiction != (!!!2)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, fiction, (!!!2));
        ++errors;
    }

    if (truth != (!!(-2))) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, truth, (!!(-2)));
        ++errors;
    }

    if (fiction != (!!!(-2))) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, fiction, (!!!(-2)));
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
