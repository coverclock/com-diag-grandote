#ifndef _COM_DIAG_DESPERADO_TYPES_H_
#define _COM_DIAG_DESPERADO_TYPES_H_

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
 *  Defines the integral types used by Desperado.
 *
 *  This header file can included from both C++ and C translation units.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/target.h"


#include "com/diag/desperado/Begin.h"


/**
 *  This type defines the storage necessary to hold absolute ticks.
 *  Ticks are the units in which the host operating system keeps
 *  time. The length of a single tick is platform dependent. It may
 *  be as small as a nanosecond, as large as a second, or even some
 *  really difficult to use fraction based on the frequency of the
 *  processor clock. Time in the platform is kept relative to an
 *  epoch, the time of day and date which represents zero ticks.
 *  Different platforms use different epochs. The same platform may
 *  not have a fixed epoch.
*/
typedef uint64_t ticks_t;


/**
 *  This type defines the storage necessary to hold absolute seconds.
 *  Absolute seconds always measures the number of seconds since the Common
 *  Era epoch.
 */
typedef uint64_t seconds_t;


/**
 *  This type identifies the running task/thread/process in some
 *  unique manner. Typically, since Desperado requires POSIX
 *  including POSIX Threads, this is the process ID from getpid(2)
 *  combined with the thread ID from pthread_self(3). However, some
 *  platforms may implement some other form of identity.
 */
typedef uint64_t identity_t;


#include "com/diag/desperado/End.h"


#endif
