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
 *  Implements the generics CXXCAPI unit test.
 *
 *  N.B.
 *
 *  Some architectures double word align 64-bit integers.
 *  Some don't. IA32 appears not to. GCC allows the alignment
 *  to be forced for a particular variable through the use
 *  of an attribute.
 *
 *      uint64_t dd __attribute__ ((aligned (sizeof(uint64_t))));
 *
 *  All double word types may be forced to double word alignment
 *  via the use of a command line option.
 *
 *      -malign-double
 *
 *  This has the risk that it will alter the layout of existing
 *  structure definitions (including in system header files) and
 *  possibly cause breakage. Alignment may be tested for using a
 *  gcc built-in.
 *
 *      (__alignof__(badorder.dd) == sizeof(uint64_t))
 *
 *  @see    generics.h
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/bool.h"
#include "com/diag/desperado/bool.h"

int unittestgenerics2() {
    int errors = 0;
    typedef struct Type {
        uint8_t cc;
        uint16_t ss;
        int32_t ll;
        int64_t dd;
    } Type;
    Type object;
    size_t offset;
    size_t size;
    typedef uint8_t (uint8a)[11];
    typedef uint16_t (uint16a)[7];
    typedef uint32_t (uint32a)[5];
    typedef uint64_t (uint64a)[3];
    typedef union Align {
        uint64_t dd[2];
        uint32_t ll[4];
        uint16_t ss[8];
        uint8_t cc[16];
    } Align;
    Align align;
    uint8_t* cpointer = &(align.cc[7]);
    uint8_t* rpointer;
    uint8_t* xpointer;
    typedef struct GoodOrder {
        uint64_t dd;    /* 0..7 */
        uint32_t ll;    /* 8..11 */
        uint16_t ss;    /* 12..13 */
        uint8_t cc;     /* 14..14 */
    } GoodOrder;
    GoodOrder goodorder __attribute__ ((aligned (sizeof(uint64_t))));
    typedef struct BadOrder {
        uint64_t slack0;    /* 0..7 */
        uint8_t slack1[1];  /* 8..8 */
        uint8_t cc;         /* 9..9 */
        uint8_t slack2[3];  /* 10..12 (13..13) */
        uint16_t ss;        /* 14..15 */
        uint8_t slack3[3];  /* 16..18 (19..19) */
        uint32_t ll;        /* 20..23 */
        uint8_t slack4[1];  /* 24 (25..31) */
        uint64_t dd;        /* 32..40 */
    } BadOrder;
    BadOrder badorder __attribute__ ((aligned (sizeof(uint64_t))));
    int aligned;
    Type* pointer;
    size_t width;
    Type array[10];
    size_t index;
    uint64_t u64;
    uint32_t u32;
    uint16_t u16;
    uint8_t u8;
    int64_t s64;
    int32_t s32;
    int16_t s16;
    int8_t s8;
    size_t ii;

    print_f(platform_output(), "%s[%d]: begin\n",
        __FILE__, __LINE__);

    print_f(platform_output(), "%s[%d]: sizeof\n", __FILE__, __LINE__);

#define psizeof(_TYPE_) \
    print_f(platform_output(), "%s[%d]: sizeof(" #_TYPE_ ")=%u\n", __FILE__, __LINE__, sizeof(_TYPE_))

    psizeof(char);
    psizeof(int8_t);
    psizeof(uint8_t);
    psizeof(short);
    psizeof(int16_t);
    psizeof(uint16_t);
    psizeof(int);
    psizeof(unsigned);
    psizeof(unsigned int);
    psizeof(int32_t);
    psizeof(uint32_t);
    psizeof(void*);
    psizeof(intptr_t);
    psizeof(size_t);
    psizeof(ssize_t);
    psizeof(float);
    psizeof(long);
    psizeof(unsigned long);
    psizeof(double);
    psizeof(long long);
    psizeof(unsigned long long);
    psizeof(int64_t);
    psizeof(uint64_t);

    print_f(platform_output(), "%s[%d]: desperado_offsetof\n",
        __FILE__, __LINE__);

#define poffsetof(_FIELD_) \
    print_f(platform_output(), "%s[%d]: desperado_offsetof(" #_FIELD_ ")=%u\n", __FILE__, __LINE__, desperado_offsetof(Type, _FIELD_))

    poffsetof(cc);
    poffsetof(ss);
    poffsetof(ll);
    poffsetof(dd);

    offset = desperado_offsetof(Type, dd);
    if (8 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, offset);
        ++errors;
    }

    offset = desperado_offsetof(Type, ll);
    if (4 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 4, offset);
        ++errors;
    }

    offset = desperado_offsetof(Type, ss);
    if (2 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 2, offset);
        ++errors;
    }

    offset = desperado_offsetof(Type, cc);
    if (0 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 0, offset);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: offsetof\n",
        __FILE__, __LINE__);

    offset = offsetof(Type, dd);
    if (8 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, offset);
        ++errors;
    }

    offset = offsetof(Type, ll);
    if (4 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 4, offset);
        ++errors;
    }

    offset = offsetof(Type, ss);
    if (2 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 2, offset);
        ++errors;
    }

    offset = offsetof(Type, cc);
    if (0 != offset) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 0, offset);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: membersizeof\n",
        __FILE__, __LINE__);

    size = membersizeof(Type, dd);
    if (sizeof(object.dd) != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, sizeof(object.dd), size);
        ++errors;
    }

    size = membersizeof(Type, ll);
    if (sizeof(object.ll) != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, sizeof(object.ll), size);
        ++errors;
    }

    size = membersizeof(Type, ss);
    if (sizeof(object.ss) != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, sizeof(object.ss), size);
        ++errors;
    }

    size = membersizeof(Type, cc);
    if (sizeof(object.cc) != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, sizeof(object.cc), size);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: sizedownto\n",
        __FILE__, __LINE__);

    size = sizedownto(sizeof(uint8a), uint64_t);
    if (8 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, size);
        ++errors;
    }

    size = sizedownto(sizeof(uint16a), uint64_t);
    if (8 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, size);
        ++errors;
    }

    size = sizedownto(sizeof(uint32a), uint64_t);
    if (16 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 16, size);
        ++errors;
    }

    size = sizedownto(sizeof(uint64a), uint64_t);
    if (24 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 24, size);
        ++errors;
    }

    size = sizedownto(sizeof(uint8a), uint32_t);
    if (8 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, size);
        ++errors;
    }

    size = sizedownto(sizeof(uint16a), uint32_t);
    if (12 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 12, size);
        ++errors;
    }

    size = sizedownto(sizeof(uint32a), uint32_t);
    if (20 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 20, size);
        ++errors;
    }

    size = sizedownto(sizeof(uint64a), uint32_t);
    if (24 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 24, size);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: sizeupto\n",
        __FILE__, __LINE__);

    size = sizeupto(sizeof(uint8a), uint64_t);
    if (16 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 16, size);
        ++errors;
    }

    size = sizeupto(sizeof(uint16a), uint64_t);
    if (16 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 16, size);
        ++errors;
    }

    size = sizeupto(sizeof(uint32a), uint64_t);
    if (24 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 24, size);
        ++errors;
    }

    size = sizeupto(sizeof(uint64a), uint64_t);
    if (24 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 24, size);
        ++errors;
    }

    size = sizeupto(sizeof(uint8a), uint32_t);
    if (12 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 12, size);
        ++errors;
    }

    size = sizeupto(sizeof(uint16a), uint32_t);
    if (16 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 16, size);
        ++errors;
    }

    size = sizeupto(sizeof(uint32a), uint32_t);
    if (20 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 20, size);
        ++errors;
    }

    size = sizeupto(sizeof(uint64a), uint32_t);
    if (24 != size) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 24, size);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: aligndownto\n",
        __FILE__, __LINE__);

    rpointer = (uint8_t*)aligndownto(cpointer, uint64_t);
    xpointer = (uint8_t*)&(align.dd[0]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)! (nominal)\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        if ((((uintptr_t)rpointer) & 0x7) != 0) {
            print_f(platform_error(), "%s[%d]: (0x%08x)!\n",
                __FILE__, __LINE__, (uintptr_t)rpointer);
            ++errors;
        }
    }

    rpointer = (uint8_t*)aligndownto(cpointer, uint32_t);
    xpointer = (uint8_t*)&(align.ll[1]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        ++errors;
    }

    rpointer = (uint8_t*)aligndownto(cpointer, uint16_t);
    xpointer = (uint8_t*)&(align.ss[3]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        ++errors;
    }

    rpointer = (uint8_t*)aligndownto(cpointer, uint8_t);
    xpointer = (uint8_t*)&(align.cc[7]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: alignupto\n",
        __FILE__, __LINE__);

    rpointer = (uint8_t*)alignupto(cpointer, uint64_t);
    xpointer = (uint8_t*)&(align.dd[1]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)! (nominal)\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        if ((((uintptr_t)rpointer) & 0x7) != 0) {
            print_f(platform_error(), "%s[%d]: (0x%08x)!\n",
                __FILE__, __LINE__, (uintptr_t)rpointer);
            ++errors;
        }
    }

    rpointer = (uint8_t*)alignupto(cpointer, uint32_t);
    xpointer = (uint8_t*)&(align.ll[2]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        ++errors;
    }

    rpointer = (uint8_t*)alignupto(cpointer, uint16_t);
    xpointer = (uint8_t*)&(align.ss[4]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        ++errors;
    }

    rpointer = (uint8_t*)alignupto(cpointer, uint8_t);
    xpointer = (uint8_t*)&(align.cc[7]);
    if (xpointer != rpointer) {
        print_f(platform_error(), "%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, (uintptr_t)xpointer, (uintptr_t)rpointer);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: isalignedto\n",
        __FILE__, __LINE__);

    print_f(platform_output(),
        "%s[%d]: &goodorder=%p &dd=%p &ll=%p &ss=%p &cc=%p\n",
        __FILE__, __LINE__,
        &goodorder, &goodorder.dd, &goodorder.ll, &goodorder.ss, &goodorder.cc);

    aligned = isalignedto(&goodorder.dd, uint64_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)! (nominal)\n",
            __FILE__, __LINE__, aligned, 1);
        aligned = isalignedto(&goodorder.dd, uint32_t);
        if (!aligned) {
            print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, aligned, 1);
            ++errors;
        }
    }

    aligned = isalignedto(&goodorder.dd, uint32_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.dd, uint16_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.dd, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.ll, uint64_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)! (nominal)\n",
            __FILE__, __LINE__, aligned, 1);
        aligned = isalignedto(&goodorder.ll, uint32_t);
        if (!aligned) {
            print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, aligned, 1);
            ++errors;
        }
    }

    aligned = isalignedto(&goodorder.ll, uint32_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.ll, uint16_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.ll, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.ss, uint64_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)! (nominal)\n",
            __FILE__, __LINE__, aligned, 0);
        aligned = isalignedto(&goodorder.ss, uint32_t);
        if (!aligned) {
            print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, aligned, 1);
            ++errors;
        }
    }

    aligned = isalignedto(&goodorder.ss, uint32_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.ss, uint16_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.ss, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.cc, uint64_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)! (nominal)\n",
            __FILE__, __LINE__, aligned, 0);
        aligned = isalignedto(&goodorder.cc, uint32_t);
        if (!aligned) {
            print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, aligned, 1);
            ++errors;
        }
    }

    aligned = isalignedto(&goodorder.cc, uint32_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 0);
        ++errors;
    }

    aligned = isalignedto(&goodorder.cc, uint16_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&goodorder.cc, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: &badorder=%p &cc=%p &ss=%p &ll=%p &dd=%p\n",
        __FILE__, __LINE__,
        &badorder, &badorder.cc, &badorder.ss, &badorder.ll, &badorder.dd);

    aligned = isalignedto(&badorder.dd, uint64_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)! (nominal)\n",
            __FILE__, __LINE__, aligned, 1);
        aligned = isalignedto(&badorder.dd, uint32_t);
        if (!aligned) {
            print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
                __FILE__, __LINE__, aligned, 1);
            ++errors;
        }
    }

    aligned = isalignedto(&badorder.dd, uint32_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.dd, uint16_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.dd, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.ll, uint64_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 0);
        ++errors;
    }

    aligned = isalignedto(&badorder.ll, uint32_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.ll, uint16_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.ll, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.ss, uint64_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 0);
        ++errors;
    }

    aligned = isalignedto(&badorder.ss, uint32_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 0);
        ++errors;
    }

    aligned = isalignedto(&badorder.ss, uint16_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.ss, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    aligned = isalignedto(&badorder.cc, uint64_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 0);
        ++errors;
    }

    aligned = isalignedto(&badorder.cc, uint32_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 0);
        ++errors;
    }

    aligned = isalignedto(&badorder.cc, uint16_t);
    if (aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 0);
        ++errors;
    }

    aligned = isalignedto(&badorder.cc, uint8_t);
    if (!aligned) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, aligned, 1);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: originof\n", __FILE__, __LINE__);

    pointer = originof(Type, dd, &object.dd);
    if (pointer != &object) {
        print_f(platform_error(), "%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, pointer, &object);
        ++errors;
    }

    pointer = originof(Type, ll, &object.ll);
    if (pointer != &object) {
        print_f(platform_error(), "%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, pointer, &object);
        ++errors;
    }

    pointer = originof(Type, ss, &object.ss);
    if (pointer != &object) {
        print_f(platform_error(), "%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, pointer, &object);
        ++errors;
    }

    pointer = originof(Type, cc, &object.cc);
    if (pointer != &object) {
        print_f(platform_error(), "%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, pointer, &object);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: widthof\n", __FILE__, __LINE__);

    width = widthof(object.dd);
    if (64 != width) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 64, width);
        ++errors;
    }

    width = widthof(object.ll);
    if (32 != width) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 32, width);
        ++errors;
    }

    width = widthof(object.ss);
    if (16 != width) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 16, width);
        ++errors;
    }

    width = widthof(object.cc);
    if (8 != width) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 8, width);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: indexof\n", __FILE__, __LINE__);

    for (ii = 0; ii < countof(array); ++ii) {
        pointer = &(array[ii]);
        index = indexof(pointer, array);
        if (index != ii) {
            print_f(platform_error(), "%s[%d]: (%lu!=%lu)!\n",
                __FILE__, __LINE__, index, ii);
            ++errors;
        }
    }

    print_f(platform_output(),
        "%s[%d]: unsignedintminof\n", __FILE__, __LINE__);

    u64 = unsignedintminof(uint64_t);
    if (UINT64_MIN != u64) {
        print_f(platform_error(), "%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, UINT64_MIN, u64);
        ++errors;
    }

    u32 = unsignedintminof(uint32_t);
    if (UINT32_MIN != u32) {
        print_f(platform_error(), "%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, UINT32_MIN, u32);
        ++errors;
    }

    u16 = unsignedintminof(uint16_t);
    if (UINT16_MIN != u16) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT16_MIN, u16);
        ++errors;
    }

    u8 = unsignedintminof(uint8_t);
    if (UINT8_MIN != u8) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT8_MIN, u8);
        ++errors;
    }

    u64 = unsignedintminof(int64_t);
    if (UINT64_MIN != u64) {
        print_f(platform_error(), "%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, UINT64_MIN, u64);
        ++errors;
    }

    u32 = unsignedintminof(int32_t);
    if (UINT32_MIN != u32) {
        print_f(platform_error(), "%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, UINT32_MIN, u32);
        ++errors;
    }

    u16 = unsignedintminof(int16_t);
    if (UINT16_MIN != u16) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT16_MIN, u16);
        ++errors;
    }

    u8 = unsignedintminof(int8_t);
    if (UINT8_MIN != u8) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT8_MIN, u8);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: unsignedintmaxof\n", __FILE__, __LINE__);

    u64 = unsignedintmaxof(uint64_t);
    if (UINT64_MAX != u64) {
        print_f(platform_error(), "%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, UINT64_MAX, u64);
        ++errors;
    }

    u32 = unsignedintmaxof(uint32_t);
    if (UINT32_MAX != u32) {
        print_f(platform_error(), "%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, UINT32_MAX, u32);
        ++errors;
    }

    u16 = unsignedintmaxof(uint16_t);
    if (UINT16_MAX != u16) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT16_MAX, u16);
        ++errors;
    }

    u8 = unsignedintmaxof(uint8_t);
    if (UINT8_MAX != u8) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT8_MAX, u8);
        ++errors;
    }

    u64 = unsignedintmaxof(int64_t);
    if (UINT64_MAX != u64) {
        print_f(platform_error(), "%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, UINT64_MAX, u64);
        ++errors;
    }

    u32 = unsignedintmaxof(int32_t);
    if (UINT32_MAX != u32) {
        print_f(platform_error(), "%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, UINT32_MAX, u32);
        ++errors;
    }

    u16 = unsignedintmaxof(int16_t);
    if (UINT16_MAX != u16) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT16_MAX, u16);
        ++errors;
    }

    u8 = unsignedintmaxof(int8_t);
    if (UINT8_MAX != u8) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT8_MAX, u8);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: signedintminof\n", __FILE__, __LINE__);

    s64 = signedintminof(uint64_t);
    if (INT64_MIN != s64) {
        print_f(platform_error(), "%s[%d]: (%lld!=%lld)!\n",
            __FILE__, __LINE__, INT64_MIN, s64);
        ++errors;
    }

    s32 = signedintminof(uint32_t);
    if (INT32_MIN != s32) {
        print_f(platform_error(), "%s[%d]: (%ld!=%ld)!\n",
            __FILE__, __LINE__, INT32_MIN, s32);
        ++errors;
    }

    s16 = signedintminof(uint16_t);
    if (INT16_MIN != s16) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
        __FILE__, __LINE__, INT16_MIN, s16);
        ++errors;
    }

    s8 = signedintminof(uint8_t);
    if (INT8_MIN != s8) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT8_MIN, s8);
        ++errors;
    }

    s64 = signedintminof(int64_t);
    if (INT64_MIN != s64) {
        print_f(platform_error(), "%s[%d]: (%lld!=%lld)!\n",
            __FILE__, __LINE__, INT64_MIN, s64);
        ++errors;
    }

    s32 = signedintminof(int32_t);
    if (INT32_MIN != s32) {
        print_f(platform_error(), "%s[%d]: (%ld!=%ld)!\n",
            __FILE__, __LINE__, INT32_MIN, s32);
        ++errors;
    }

    s16 = signedintminof(int16_t);
    if (INT16_MIN != s16) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
        __FILE__, __LINE__, INT16_MIN, s16);
        ++errors;
    }

    s8 = signedintminof(int8_t);
    if (INT8_MIN != s8) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT8_MIN, s8);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: signedintmaxof\n", __FILE__, __LINE__);

    s64 = signedintmaxof(int64_t);
    if (INT64_MAX != s64) {
        print_f(platform_error(), "%s[%d]: (%lld!=%lld)!\n",
            __FILE__, __LINE__, INT64_MAX, s64);
        ++errors;
    }

    s32 = signedintmaxof(int32_t);
    if (INT32_MAX != s32) {
        print_f(platform_error(), "%s[%d]: (%ld!=%ld)!\n",
            __FILE__, __LINE__, INT32_MAX, s32);
        ++errors;
    }

    s16 = signedintmaxof(int16_t);
    if (INT16_MAX != s16) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT16_MAX, s16);
        ++errors;
    }

    s8 = signedintmaxof(int8_t);
    if (INT8_MAX != s8) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT8_MAX, s8);
        ++errors;
    }

    s64 = signedintmaxof(uint64_t);
    if (INT64_MAX != s64) {
        print_f(platform_error(), "%s[%d]: (%lld!=%lld)!\n",
            __FILE__, __LINE__, INT64_MAX, s64);
        ++errors;
    }

    s32 = signedintmaxof(uint32_t);
    if (INT32_MAX != s32) {
        print_f(platform_error(), "%s[%d]: (%ld!=%ld)!\n",
            __FILE__, __LINE__, INT32_MAX, s32);
        ++errors;
    }

    s16 = signedintmaxof(uint16_t);
    if (INT16_MAX != s16) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT16_MAX, s16);
        ++errors;
    }

    s8 = signedintmaxof(uint8_t);
    if (INT8_MAX != s8) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT8_MAX, s8);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: issignedint\n", __FILE__, __LINE__);

    if (issignedint(uint8_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d))!\n",
            __FILE__, __LINE__, false, issignedint(uint8_t));
        ++errors;
    }

    if (issignedint(uint16_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, false, issignedint(uint16_t));
        ++errors;
    }

    if (issignedint(uint32_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, false, issignedint(uint32_t));
        ++errors;
    }

    if (issignedint(uint64_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, false, issignedint(uint64_t));
        ++errors;
    }

    if (!issignedint(int8_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, issignedint(int8_t));
        ++errors;
    }

    if (!issignedint(int16_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, issignedint(int16_t));
        ++errors;
    }

    if (!issignedint(int32_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, issignedint(int32_t));
        ++errors;
    }

    if (!issignedint(int64_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, issignedint(int64_t));
        ++errors;
    }

    if (issignedint(size_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, false, issignedint(size_t));
        ++errors;
    }

    if (!issignedint(ssize_t)) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, true, issignedint(ssize_t));
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: intminof\n", __FILE__, __LINE__);

    u64 = intminof(uint64_t);
    if (UINT64_MIN != u64) {
        print_f(platform_error(), "%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, UINT64_MIN, u64);
        ++errors;
    }

    u32 = intminof(uint32_t);
    if (UINT32_MIN != u32) {
        print_f(platform_error(), "%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, UINT32_MIN, u32);
        ++errors;
    }

    u16 = intminof(uint16_t);
    if (UINT16_MIN != u16) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT16_MIN, u16);
        ++errors;
    }

    u8 = intminof(uint8_t);
    if (UINT8_MIN != u8) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT8_MIN, u8);
        ++errors;
    }

    s64 = intminof(int64_t);
    if (INT64_MIN != s64) {
        print_f(platform_error(), "%s[%d]: (%lld!=%lld)!\n",
            __FILE__, __LINE__, INT64_MIN, s64);
        ++errors;
    }

    s32 = intminof(int32_t);
    if (INT32_MIN != s32) {
        print_f(platform_error(), "%s[%d]: (%ld!=%ld)!\n",
            __FILE__, __LINE__, INT32_MIN, s32);
        ++errors;
    }

    s16 = intminof(int16_t);
    if (INT16_MIN != s16) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
        __FILE__, __LINE__, INT16_MIN, s16);
        ++errors;
    }

    s8 = intminof(int8_t);
    if (INT8_MIN != s8) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT8_MIN, s8);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: intmaxof\n", __FILE__, __LINE__);

    u64 = intmaxof(uint64_t);
    if (UINT64_MAX != u64) {
        print_f(platform_error(), "%s[%d]: (%llu!=%llu)!\n",
            __FILE__, __LINE__, UINT64_MAX, u64);
        ++errors;
    }

    u32 = intmaxof(uint32_t);
    if (UINT32_MAX != u32) {
        print_f(platform_error(), "%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, UINT32_MAX, u32);
        ++errors;
    }

    u16 = intmaxof(uint16_t);
    if (UINT16_MAX != u16) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT16_MAX, u16);
        ++errors;
    }

    u8 = intmaxof(uint8_t);
    if (UINT8_MAX != u8) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, UINT8_MAX, u8);
        ++errors;
    }

    s64 = intmaxof(int64_t);
    if (INT64_MAX != s64) {
        print_f(platform_error(), "%s[%d]: (%lld!=%lld)!\n",
            __FILE__, __LINE__, INT64_MAX, s64);
        ++errors;
    }

    s32 = intmaxof(int32_t);
    if (INT32_MAX != s32) {
        print_f(platform_error(), "%s[%d]: (%ld!=%ld)!\n",
            __FILE__, __LINE__, INT32_MAX, s32);
        ++errors;
    }

    s16 = intmaxof(int16_t);
    if (INT16_MAX != s16) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT16_MAX, s16);
        ++errors;
    }

    s8 = intmaxof(int8_t);
    if (INT8_MAX != s8) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, INT8_MAX, s8);
        ++errors;
    }

    print_f(platform_output(), "%s[%d]: rangeof\n", __FILE__, __LINE__);

    u16 = rangeof(uint8_t, uint16_t);
    if (0x100U != u16) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 0x100U, u16);
        ++errors;
    }

    u32 = rangeof(uint16_t, uint32_t);
    if (0x10000UL  != u32) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 0x10000UL, u32);
        ++errors;
    }

    u64 = rangeof(uint32_t, uint64_t);
    if (0x100000000ULL != u64) {
        print_f(platform_error(), "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 0x100000000ULL, u64);
        ++errors;
    }

    print_f(platform_output(),
        "%s[%d]: values\n", __FILE__, __LINE__);

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "unsignedintminof(uint64_t)",
        unsignedintminof(uint64_t),
        unsignedintminof(uint64_t),
        unsignedintminof(uint64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "unsignedintminof(uint32_t)",
        unsignedintminof(uint32_t),
        unsignedintminof(uint32_t),
        unsignedintminof(uint32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "unsignedintminof(uint16_t)",
        unsignedintminof(uint16_t),
        unsignedintminof(uint16_t),
        unsignedintminof(uint16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "unsignedintminof(uint8_t)",
        unsignedintminof(uint8_t),
        unsignedintminof(uint8_t),
        unsignedintminof(uint8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(uint64_t)",
        unsignedintmaxof(uint64_t),
        unsignedintmaxof(uint64_t),
        unsignedintmaxof(uint64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(uint32_t)",
        unsignedintmaxof(uint32_t),
        unsignedintmaxof(uint32_t),
        unsignedintmaxof(uint32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(uint16_t)",
        unsignedintmaxof(uint16_t),
        unsignedintmaxof(uint16_t),
        unsignedintmaxof(uint16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(uint8_t)",
        unsignedintmaxof(uint8_t),
        unsignedintmaxof(uint8_t),
        unsignedintmaxof(uint8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "unsignedintminof(int64_t)",
        unsignedintminof(int64_t),
        unsignedintminof(int64_t),
        unsignedintminof(int64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "unsignedintminof(int32_t)",
        unsignedintminof(int32_t),
        unsignedintminof(int32_t),
        unsignedintminof(int32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "unsignedintminof(int16_t)",
        unsignedintminof(int16_t),
        unsignedintminof(int16_t),
        unsignedintminof(int16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "unsignedintminof(int8_t)",
        unsignedintminof(int8_t),
        unsignedintminof(int8_t),
        unsignedintminof(int8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(int64_t)",
        unsignedintmaxof(int64_t),
        unsignedintmaxof(int64_t),
        unsignedintmaxof(int64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(int32_t)",
        unsignedintmaxof(int32_t),
        unsignedintmaxof(int32_t),
        unsignedintmaxof(int32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(int16_t)",
        unsignedintmaxof(int16_t),
        unsignedintmaxof(int16_t),
        unsignedintmaxof(int16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "unsignedintmaxof(int8_t)",
        unsignedintmaxof(int8_t),
        unsignedintmaxof(int8_t),
        unsignedintmaxof(int8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "signedintminof(int64_t)",
        signedintminof(int64_t),
        signedintminof(int64_t),
        signedintminof(int64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "signedintminof(int32_t)",
        signedintminof(int32_t),
        signedintminof(int32_t),
        signedintminof(int32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "signedintminof(int16_t)",
        signedintminof(int16_t),
        signedintminof(int16_t),
        signedintminof(int16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "signedintminof(int8_t)",
        signedintminof(int8_t),
        signedintminof(int8_t),
        signedintminof(int8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "signedintmaxof(int64_t)",
        signedintmaxof(int64_t),
        signedintmaxof(int64_t),
        signedintmaxof(int64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "signedintmaxof(int32_t)",
        signedintmaxof(int32_t),
        signedintmaxof(int32_t),
        signedintmaxof(int32_t),
        signedintmaxof(int32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "signedintmaxof(int16_t)",
        signedintmaxof(int16_t),
        signedintmaxof(int16_t),
        signedintmaxof(int16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "signedintmaxof(int8_t)",
        signedintmaxof(int8_t),
        signedintmaxof(int8_t),
        signedintmaxof(int8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "signedintminof(uint64_t)",
        signedintminof(uint64_t),
        signedintminof(uint64_t),
        signedintminof(uint64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "signedintminof(uint32_t)",
        signedintminof(uint32_t),
        signedintminof(uint32_t),
        signedintminof(uint32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "signedintminof(uint16_t)",
        signedintminof(uint16_t),
        signedintminof(uint16_t),
        signedintminof(uint16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "signedintminof(uint8_t)",
        signedintminof(uint8_t),
        signedintminof(uint8_t),
        signedintminof(uint8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "signedintmaxof(uint64_t)",
        signedintmaxof(uint64_t),
        signedintmaxof(uint64_t),
        signedintmaxof(uint64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "signedintmaxof(uint32_t)",
        signedintmaxof(uint32_t),
        signedintmaxof(uint32_t),
        signedintmaxof(uint32_t),
        signedintmaxof(uint32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "signedintmaxof(uint16_t)",
        signedintmaxof(uint16_t),
        signedintmaxof(uint16_t),
        signedintmaxof(uint16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "signedintmaxof(uint8_t)",
        signedintmaxof(uint8_t),
        signedintmaxof(uint8_t),
        signedintmaxof(uint8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "intminof(uint64_t)",
        intminof(uint64_t),
        intminof(uint64_t),
        intminof(uint64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "intminof(uint32_t)",
        intminof(uint32_t),
        intminof(uint32_t),
        intminof(uint32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "intminof(uint16_t)",
        intminof(uint16_t),
        intminof(uint16_t),
        intminof(uint16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "intminof(uint8_t)",
        intminof(uint8_t),
        intminof(uint8_t),
        intminof(uint8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "intmaxof(uint64_t)",
        intmaxof(uint64_t),
        intmaxof(uint64_t),
        intmaxof(uint64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "intmaxof(uint32_t)",
        intmaxof(uint32_t),
        intmaxof(uint32_t),
        intmaxof(uint32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "intmaxof(uint16_t)",
        intmaxof(uint16_t),
        intmaxof(uint16_t),
        intmaxof(uint16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "intmaxof(uint8_t)",
        intmaxof(uint8_t),
        intmaxof(uint8_t),
        intmaxof(uint8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "intminof(int64_t)",
        intminof(int64_t),
        intminof(int64_t),
        intminof(int64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "intminof(int32_t)",
        intminof(int32_t),
        intminof(int32_t),
        intminof(int32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "intminof(int16_t)",
        intminof(int16_t),
        intminof(int16_t),
        intminof(int16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "intminof(int8_t)",
        intminof(int8_t),
        intminof(int8_t),
        intminof(int8_t));

    print_f(platform_output(), "%s[%d]: %s=0x%016llx=%lld=%llu\n",
        __FILE__, __LINE__,
        "intmaxof(int64_t)",
        intmaxof(int64_t),
        intmaxof(int64_t),
        intmaxof(int64_t));

    print_f(platform_output(), "%s[%d]: %s=0x%08lx=%ld=%lu\n",
        __FILE__, __LINE__,
        "intmaxof(int32_t)",
        intmaxof(int32_t),
        intmaxof(int32_t),
        intmaxof(int32_t));

    print_f(platform_output(), "%s[%d]: %s=0x%04hx=%hd=%u\n",
        __FILE__, __LINE__,
        "intmaxof(int16_t)",
        intmaxof(int16_t),
        intmaxof(int16_t),
        intmaxof(int16_t));

    print_f(platform_output(), "%s[%d]: %s=0x%02hhx=%hhd=%u\n",
        __FILE__, __LINE__,
        "intmaxof(int8_t)",
        intmaxof(int8_t),
        intmaxof(int8_t),
        intmaxof(int8_t));

    print_f(platform_output(), "%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}

