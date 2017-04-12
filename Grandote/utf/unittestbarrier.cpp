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
 *  Implements the barrier unit test.
 *
 *  @see    barrier.h
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/types.h"
#include "com/diag/desperado/barrier.h"
#include "com/diag/desperado/barrier.h"
#include "com/diag/desperado/MemoryBarrier.h"
#include "com/diag/desperado/MemoryBarrier.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Desperado.h"

CXXCAPI int unittestbarrier(void) {
    Print printf;
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    Platform& dbplatform = Platform::instance();
    int dbrc;
    ticks_t dbbefore;
    ticks_t dbafter;

    //  The first call below is much more expensive than
    //  subsequent calls. it is not the function itself; 
    //  if you reorder the calls, the first (now different)
    //  call is still much more expensive (21us vs 3us).

    dbbefore = dbplatform.time();
    dbrc = desperado_portable_barrier();
    dbafter = dbplatform.time();
    if (0 != dbrc) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, dbrc);
        ++errors;
    }
    printf("%s[%d]: desperado_portable_barrier(): %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);

    dbbefore = dbplatform.time();
    dbrc = desperado_portable_barrier();
    dbafter = dbplatform.time();
    if (0 != dbrc) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, dbrc);
        ++errors;
    }
    printf("%s[%d]: desperado_portable_barrier(): %llu ticks (second try)\n",
        __FILE__, __LINE__, dbafter - dbbefore);

    dbbefore = dbplatform.time();
    dbrc = desperado_native_barrier();
    dbafter = dbplatform.time();
    if (0 != dbrc) {
        errorf("%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, dbrc);
        ++errors;
    }
    printf("%s[%d]: desperado_native_barrier(): %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);

    dbbefore = dbplatform.time();
    desperado_memory_barrier();
    dbafter = dbplatform.time();
    printf("%s[%d]: desperado_memory_barrier(): %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);

    dbbefore = dbplatform.time();
    {
        MemoryBarrier memorybarrier;
    }
    dbafter = dbplatform.time();
    printf("%s[%d]: MemoryBarrier: %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
