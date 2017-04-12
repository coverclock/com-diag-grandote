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
 *  Implements the Volatile unit test.
 *
 *  @see    Volatile
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Volatile.h"
#include "com/diag/desperado/Volatile.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Desperado.h"

template <typename _TYPE_>
static int unittestVolatileType() {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: sizeof=%zu issignedint=%d\n",
        __FILE__, __LINE__, sizeof(_TYPE_), issignedint(_TYPE_));

    volatile _TYPE_ datum = -1;

    if (Volatile::value(&datum) != datum) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, Volatile::value(&datum), datum);
        ++errors;
    }

    if (Volatile::value(datum) != datum) {
        errorf("%s[%d]: (0x%x!=0x%x)!\n",
            __FILE__, __LINE__, Volatile::value(datum), datum);
        ++errors;
    }

    if (Volatile::address(&datum) != &datum) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, Volatile::address(&datum), &datum);
        ++errors;
    }

    if (Volatile::address(datum) != &datum) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, Volatile::address(datum), &datum);
        ++errors;
    }

    if (&Volatile::reference(&datum) != &datum) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, &Volatile::reference(&datum), &datum);
        ++errors;
    }

    if (&Volatile::reference(datum) != &datum) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, &Volatile::reference(datum), &datum);
        ++errors;
    }

    return errors;
}


CXXCAPI int unittestVolatile(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    errors += unittestVolatileType<uint8_t>();
    errors += unittestVolatileType<uint16_t>();
    errors += unittestVolatileType<uint32_t>();
    errors += unittestVolatileType<uint64_t>();
    errors += unittestVolatileType<int8_t>();
    errors += unittestVolatileType<int16_t>();
    errors += unittestVolatileType<int32_t>();
    errors += unittestVolatileType<int64_t>();

    printf("%s[%d]: end errors=%d\n", __FILE__, __LINE__,
        errors);

    return errors;
}

