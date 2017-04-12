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
 *  Implements the Crc unit test.
 *
 *  @see    Crc
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Crc.h"
#include "com/diag/desperado/Crc.h"
#include "com/diag/desperado/atm_Crc.h"
#include "com/diag/desperado/atm_Crc.h"
#include "com/diag/desperado/crc16_Crc.h"
#include "com/diag/desperado/crc16_Crc.h"
#include "com/diag/desperado/crc32_Crc.h"
#include "com/diag/desperado/crc32_Crc.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

static Crc<uint32_t> staticCrc;

CXXCAPI int unittestCrc(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    const char* string = "123456789";
    size_t size = std::strlen(string);

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticCrc.show();

    printf("%s[%d]: CRC16\n", __FILE__, __LINE__);

    Crc<uint16_t> crc16table(0x8005, true, 0, 0);
    crc16table.show();
    crc16table.show(1);

    uint16_t state16;
    uint16_t crc16;
    uint16_t expected16 = 0xbb3d; // [Williams1993]
    crc16table(state16);
    for (unsigned int ii = 0; size > ii; ii++) {
        crc16table(state16, &(string[ii]), 1);
    }
    crc16 = crc16table(state16, 0, 0);
    printf("crc16(\"%s\"[%u])=0x%x\n", string, size, crc16);
    if (expected16 != crc16) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, expected16, crc16);
        ++errors;
    }

    printf("%s[%d]: CRC32: PKZIP, Ethernet, FDDI\n", __FILE__, __LINE__);

    Crc<uint32_t> crc32table(0x04c11db7, true, 0xffffffff, 0xffffffff);
    crc32table.show();
    crc32table.show(1);

    uint32_t state32;
    uint32_t crc32;
    uint32_t expected32 = 0xcbf43926; // [Williams1993]
    crc32table(state32);
    crc32 = crc32table(state32, string, size);
    printf("crc32(\"%s\"[%u])=0x%x\n", string, size, crc32);
    if (expected32 != crc32) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, expected32, crc16);
        ++errors;
    }

    printf("%s[%d]: CCMS CRC\n", __FILE__, __LINE__);

    Crc<uint16_t> ccmstable(0x1021, true, 0xffff, 0xffff);
    ccmstable.show();
    ccmstable.show(1);

    printf("%s[%d]: ATM CRC\n", __FILE__, __LINE__);

    Crc<int32_t> atmtable(0x04c11db7, false, 0xffffffff, 0xffffffff);
    atmtable.show();
    atmtable.show(1);

    printf("%s[%d]: predefined generators\n", __FILE__, __LINE__);

    ::atm_Crc.show();
    ::crc16_Crc.show();
    ::crc32_Crc.show();

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__, errors);

    return errors;
}
