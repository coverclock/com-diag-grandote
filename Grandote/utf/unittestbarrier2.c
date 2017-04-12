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
 *  Implements the barrier CXXCAPI unit test.
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
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Print.h"

int unittestbarrier2() {
    int errors = 0;
    int dbrc;
    ticks_t dbbefore;
    ticks_t dbafter;

    print_f(platform_output(), "%s[%d]: begin\n", __FILE__, __LINE__);

    dbbefore = platform_time();
    dbrc = desperado_portable_barrier();
    dbafter = platform_time();
    if (0 != dbrc) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, dbrc);
        ++errors;
    }
    print_f(platform_output(),
        "%s[%d]: desperado_portable_barrier(): %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);

    dbbefore = platform_time();
    dbrc = desperado_native_barrier();
    dbafter = platform_time();
    if (0 != dbrc) {
        print_f(platform_error(), "%s[%d]: (%d!=%d)!\n",
            __FILE__, __LINE__, 0, dbrc);
        ++errors;
    }
    print_f(platform_output(),
        "%s[%d]: desperado_native_barrier(): %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);

    dbbefore = platform_time();
    desperado_memory_barrier();
    dbafter = platform_time();
    print_f(platform_output(),
        "%s[%d]: desperado_memory_barrier(): %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);

#if defined(__GNUC__)
#    if defined(__GNUC_MINOR__)
#        if ((((__GNUC__)*1000)+(__GNUC_MINOR__))>=4001)
    dbbefore = platform_time();
    __sync_synchronize();
    dbafter = platform_time();
    print_f(platform_output(),
        "%s[%d]: __sync_synchronize(): %llu ticks\n",
        __FILE__, __LINE__, dbafter - dbbefore);
#        endif
#    endif
#endif

    print_f(platform_output(), "%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
