#ifndef _COM_DIAG_DESPERADO_TARGET_H_
#define _COM_DIAG_DESPERADO_TARGET_H_

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
 *  Determines some qualities of the hardware target and of the C and
 *  C++ compilers used for it and defines standard integral types. Rather
 *  than defining integral types itself, this header file relies as much
 *  as possible on system header files like stdint.h as defined in the
 *  ISO C99 standard.
 *
 *  This header file can included from both C++ and C translation units.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


/* (As required per ISO C99 for limits of integral types to be defined...) */
#define __STDC_LIMIT_MACROS (1)
#include "com/diag/desperado/stdint.h"
/* (... except it does not define these useful ones.) */
#if !defined(UINT8_MIN)
#   define UINT8_MIN   (0)
#endif
#if !defined(UINT16_MIN)
#   define UINT16_MIN  (0)
#endif
#if !defined(UINT32_MIN)
#   define UINT32_MIN  (0)
#endif
#if !defined(UINT64_MIN)
#   define UINT64_MIN  (0)
#endif
#include <sys/types.h>
#if defined(__GNUC__)
#  include <endian.h>
#endif


/*
 *  Desperado expects at least the following integral types to be defined
 *  at this point:
 *
 *      int16_t
 *      int32_t
 *      int64_t
 *      int8_t
 *      intptr_t
 *      size_t
 *      ssize_t
 *      uint16_t
 *      uint32_t
 *      uint64_t
 *      uint8_t
 *      uintptr_t
 */

/*
 * Knowing what floats are available would be good.
 */

#if !defined(DESPERADO_TARGET_FLOAT32)
#   define DESPERADO_TARGET_FLOAT32  float
#endif

typedef DESPERADO_TARGET_FLOAT32 float32_t;

#if !defined(DESPERADO_TARGET_FLOAT64)
#   define DESPERADO_TARGET_FLOAT64  double
#endif

typedef DESPERADO_TARGET_FLOAT64 float64_t;

/*
 *  Establish byte ordering for this target.
 */

#if !defined(DESPERADO_HAS_LITTLE_ENDIAN)
#   if !defined(DESPERADO_HAS_BIG_ENDIAN)
#       if defined(__GNUC__)
#           if (__BYTE_ORDER == __LITTLE_ENDIAN)
#               define DESPERADO_HAS_LITTLE_ENDIAN
#           elif (__BYTE_ORDER == __BIG_ENDIAN)
#               define DESPERADO_HAS_BIG_ENDIAN
#           endif
#       endif
#   endif
#endif

#if !defined(DESPERADO_HAS_LITTLE_ENDIAN)
#   if !defined(DESPERADO_HAS_BIG_ENDIAN)
#       error "desperado: define either DESPERADO_HAS_LITTLE_ENDIAN or DESPERADO_HAS_BIG_ENDIAN but not both!"
#   endif
#endif

#if defined(DESPERADO_HAS_LITTLE_ENDIAN)
#   if defined(DESPERADO_HAS_BIG_ENDIAN)
#       error "desperado: define either DESPERADO_HAS_LITTLE_ENDIAN or DESPERADO_HAS_BIG_ENDIAN but not both!"
#   endif
#endif


/*
 *  Establish bitfield ordering for this target.
 *
 *  It is fully appropriate at this point to curse at the fact
 *  that ANSI C neglects to specify a standard ordering for bitfields,
 *  even though its failure to do so probably means embedded C code I wrote
 *  for PDP-11s more than twenty years ago would still compile and run
 *  today, providing you could find a PDP-11. (I was writing C++ embedded
 *  code for PDP-11s almost as long ago.) The code below makes the very
 *  naive assumption that the bitfield ordering relates to byte order.
 *  This seems to be the convention. A little experimentation has
 *  convinced me that it is almost impossible to write hardware
 *  independent code if this is not the case when dealing with sequences
 *  of bitfields taking up multiple words.
 */

#if !defined(DESPERADO_HAS_BITFIELDS_LOW_TO_HIGH)
#   if !defined(DESPERADO_HAS_BITFIELDS_HIGH_TO_LOW)
#       if defined(__GNUC__)
#           if (__BYTE_ORDER == __LITTLE_ENDIAN)
#               define DESPERADO_HAS_BITFIELDS_LOW_TO_HIGH
#           elif (__BYTE_ORDER == __BIG_ENDIAN)
#               define DESPERADO_HAS_BITFIELDS_HIGH_TO_LOW
#           endif
#       endif
#   endif
#endif

#if !defined(DESPERADO_HAS_BITFIELDS_LOW_TO_HIGH)
#   if !defined(DESPERADO_HAS_BITFIELDS_HIGH_TO_LOW)
#       error "desperado: define either DESPERADO_HAS_BITFIELDS_LOW_TO_HIGH or DESPERADO_HAS_BITFIELDS_HIGH_TO_LOW but not both!"
#   endif
#endif

#if defined(DESPERADO_HAS_BITFIELDS_LOW_TO_HIGH)
#   if defined(DESPERADO_HAS_BITFIELDS_HIGH_TO_LOW)
#       error "desperado: define either DESPERADO_HAS_BITFIELDS_LOW_TO_HIGH or DESPERADO_HAS_BITFIELDS_HIGH_TO_LOW but not both!"
#   endif
#endif


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the target unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittesttarget(void);

/**
 *  Run the target CXXCAPI unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittesttarget2(void);
#endif


#endif
