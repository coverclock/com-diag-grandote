#ifndef _COM_DIAG_DESPERADO_LITTLEENDIAN_H_
#define _COM_DIAG_DESPERADO_LITTLEENDIAN_H_

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
 *  Implements the littleendian inline function. This can be included
 *  from either a C or a C++ translation unit.
 *
 *  For years I used the first algorithm shown below, which is based on
 *  a little trick I stole shamelessly from the original X11 distribution.
 *  But interestingly enough, that trick doesn't work with many modern
 *  processors and their optimizing C and C++ compilers because of
 *  assumptions made about "aliasing" of pointers. So I had to invent
 *  a new approach which used a mechanism C/C++ could understand.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/cxxcapi.h"

#if 0

#include "com/diag/desperado/generics.h"

/**
 *  Returns non-zero if the target is little endian, zero if big endian.
 *  Based on code in the original X11 distribution from MIT. This is
 *  callable from C. Very likely optimized away to be a simple value.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 *
 *  @see X11
 *
 *  @return non-zero if little endian, zero if big endian.
 */
CXXCINLINE int littleendian(void) {
    unsigned long word=1;
    return ((*(staticcastto(char*, &word))) != 0) ? 1 : 0;
}

#else

#include "com/diag/desperado/target.h"

/**
 *  Returns non-zero if the target is little endian, zero if big endian.
 *  Based on C++ code I wrote for a client for an embedded projet. This is
 *  callable from C. Very likely optimized away to be a simple value.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 *
 *  @return non-zero if little endian, zero if big endian.
 */
CXXCINLINE int littleendian(void) {
    static union { uint32_t w; uint8_t b[sizeof(uint32_t)]; } d = { 1 };
    return d.b[0];
}

#endif


#endif
