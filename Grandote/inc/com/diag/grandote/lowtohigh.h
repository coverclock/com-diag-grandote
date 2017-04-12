#ifndef _COM_DIAG_DESPERADO_LOWTOHIGH_H_
#define _COM_DIAG_DESPERADO_LOWTOHIGH_H_

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
 *  Implements the lowtohigh inline function. This can be included
 *  from either a C or a C++ translation unit.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/generics.h"


/**
 *  Returns non-zero if the target orders bitfields from low-order to
 *  high, zero if high-order to low. There is an assumption made in
 *  the code below that arithmetic is typical two's complement.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 *
 *  @return non-zero if bitfields ordered low-order to high, zero if
 *          high-order to low.
 */
CXXCINLINE int lowtohigh() {
    union {
        signed int value;
        struct {
            unsigned int sign : 1;
        } field;
    } bitfields;

    bitfields.value = 0;
    bitfields.field.sign = 1;

    return (0 > bitfields.value) ? 0 : 1;
}


#endif
