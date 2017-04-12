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
 *  Implements the Ram unit test.
 *
 *  @see    Ram
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Ram.h"
#include "com/diag/desperado/Ram.h"
#include "com/diag/desperado/uint8_Ram.h"
#include "com/diag/desperado/uint8_Ram.h"
#include "com/diag/desperado/uint16_Ram.h"
#include "com/diag/desperado/uint16_Ram.h"
#include "com/diag/desperado/uint32_Ram.h"
#include "com/diag/desperado/uint32_Ram.h"
#include "com/diag/desperado/uint64_Ram.h"
#include "com/diag/desperado/uint64_Ram.h"
#include "com/diag/desperado/Heap.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/Desperado.h"

template class Ram<int>;

static Ram<int> staticRam;

CXXCAPI int unittestRam(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    Dump dump;
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticRam.show();

    uint8_Ram.show();

    uint16_Ram.show();

    uint32_Ram.show();

    uint64_Ram.show();

    Ram<uint8_t> ram2 = uint8_Ram;
    ram2.show();

    Ram<uint8_t> ram3;
    ram3 = uint8_Ram;
    ram3.show();

    void* pointer = Platform::instance().heap().malloc(4096);
    size_t bytes = Platform::instance().heap().size(pointer);

    printf("%s[%d]: 8 bits\n", __FILE__, __LINE__);

    uint8_t* pointer8 = static_cast<uint8_t*>(pointer);

    printf("%s[%d]: data\n", __FILE__, __LINE__);
    volatile uint8_t data8 = uint8_Ram.data(pointer8);
    if (0 != data8) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, data8);
    }
    dump.words(pointer8, bytes);
    printf("%s[%d]: address\n", __FILE__, __LINE__);
    volatile uint8_t* address8 = uint8_Ram.address(pointer8, bytes);
    if (0 != address8) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(address8));
    }
    dump.words(pointer8, bytes);
    printf("%s[%d]: device\n", __FILE__, __LINE__);
    volatile uint8_t* device8 = uint8_Ram.device(pointer8, bytes);
    if (0 != device8) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(device8));
    }
    dump.words(pointer8, bytes);

    printf("%s[%d]: 16 bits\n", __FILE__, __LINE__);

    uint16_t* pointer16 = static_cast<uint16_t*>(pointer);

    printf("%s[%d]: data\n", __FILE__, __LINE__);
    volatile uint16_t data16 = uint16_Ram.data(pointer16);
    if (0 != data16) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, data16);
    }
    dump.words(pointer16, bytes);
    printf("%s[%d]: address\n", __FILE__, __LINE__);
    volatile uint16_t* address16 = uint16_Ram.address(pointer16, bytes);
    if (0 != address16) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(address16));
    }
    dump.words(pointer16, bytes);
    printf("%s[%d]: device\n", __FILE__, __LINE__);
    volatile uint16_t* device16 = uint16_Ram.device(pointer16, bytes);
    if (0 != device16) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(device16));
    }
    dump.words(pointer16, bytes);

    printf("%s[%d]: 32 bits\n", __FILE__, __LINE__);

    uint32_t* pointer32 = static_cast<uint32_t*>(pointer);

    printf("%s[%d]: data\n", __FILE__, __LINE__);
    volatile uint32_t data32 = uint32_Ram.data(pointer32);
    if (0 != data32) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, data32);
    }
    dump.words(pointer32, bytes);
    printf("%s[%d]: address\n", __FILE__, __LINE__);
    volatile uint32_t* address32 = uint32_Ram.address(pointer32, bytes);
    if (0 != address32) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(address32));
    }
    dump.words(pointer32, bytes);
    printf("%s[%d]: device\n", __FILE__, __LINE__);
    volatile uint32_t* device32 = uint32_Ram.device(pointer32, bytes);
    if (0 != device32) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(device32));
    }
    dump.words(pointer32, bytes);

    printf("%s[%d]: 64 bits\n", __FILE__, __LINE__);

    uint64_t* pointer64 = static_cast<uint64_t*>(pointer);

    printf("%s[%d]: data\n", __FILE__, __LINE__);
    volatile uint64_t data64 = uint64_Ram.data(pointer64);
    if (0 != data64) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, data64);
    }
    dump.words(pointer64, bytes);
    printf("%s[%d]: address\n", __FILE__, __LINE__);
    volatile uint64_t* address64 = uint64_Ram.address(pointer64, bytes);
    if (0 != address64) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(address64));
    }
    dump.words(pointer64, bytes);
    printf("%s[%d]: device\n", __FILE__, __LINE__);
    volatile uint64_t* device64 = uint64_Ram.device(pointer64, bytes);
    if (0 != device64) {
        errorf("%s[%d]: (0x%08x!=0x%08x)!\n",
            __FILE__, __LINE__, 0, reinterpret_cast<uintptr_t>(device64));
    }
    dump.words(pointer64, bytes);

    Platform::instance().heap().free(pointer);

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
