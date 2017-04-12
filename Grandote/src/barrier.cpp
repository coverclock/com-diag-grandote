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
 *  Implements the Desperado portable memory barrier function.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include <pthread.h>
#include "com/diag/desperado/barrier.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

//
//  Implement a memory barrier. Yes, we really are creating a mutex
//  on the stack. We're only going to use it for its side effect of
//  implementing a memory barrier. Allocating it on the stack insures
//  that two threads will never see the same mutex (since it isn't
//  necessary in this context) hence there's never a chance of actually
//  blocking, and if one of the mutex operations fails, the mutex isn't
//  left in an ambiguous state but rather all cleaned up regardless.
//  The down side is that this is extremely inefficient compared to using
//  an assembler memory barrier instruction.
//
CXXCAPI int desperado_portable_barrier() {
    Logger& logger = Platform::instance().logger();
    int value = 0;
    int rc;
    int state;
    pthread_mutex_t mutex;

    rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
    if (0 != rc) {
        logger.error("%s[%d]: pthread_setcancelstate()=%d\n",
            __FILE__, __LINE__, rc);
        value = -1;
    } else {
        rc = pthread_mutex_init(&mutex, 0);
        if (0 != rc) {
            logger.error("%s[%d]: pthread_mutex_init(%p)=%d\n",
                __FILE__, __LINE__, &mutex, rc);
            value = -2;
        } else {
            rc = pthread_mutex_lock(&mutex);
            if (0 != rc) {
                logger.error("%s[%d]: pthread_mutex_lock(%p)=%d\n",
                    __FILE__, __LINE__, &mutex, rc);
                value = -3;
            } else {
                rc = pthread_mutex_unlock(&mutex);
                if (0 != rc) {
                    logger.error("%s[%d]: pthread_mutex_unlock(%p)=%d\n",
                        __FILE__, __LINE__, &mutex, rc);
                    value = -4;
                }
            }
        }
        rc = pthread_setcancelstate(state, 0);
        if (0 != rc) {
            logger.error("%s[%d]: pthread_setcancelstate(%d)=%d\n",
                __FILE__, __LINE__, state, rc);
            value = -5;
        }
    }

    return value;
}
