#ifndef _COM_DIAG_DESPERADO_ERRNO_H_
#define _COM_DIAG_DESPERADO_ERRNO_H_

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
 *  Provides a place for applications to extend the errno range
 *  by adding new error numbers.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#if defined(__cplusplus)
#   include <cerrno>
#else
#   include <errno.h>
#endif


#include "com/diag/desperado/cxxcapi.h"


CXXCAPI int* platform_errornumber(void);


#if !defined(errno)
    /**
     *  @def    errno
     *
     *  Dereferences the pointer returned by the platform error
     *  number function which points to the appropriate errno
     *  variable for the calling context. This does not override
     *  any similar platform specific errno implementation.
     */
#   define errno (*platform_errornumber())
#endif


#endif
