#ifndef _COM_DIAG_DESPERADO_ROOT_H_
#define _COM_DIAG_DESPERADO_ROOT_H_

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
 *  Defines the root function.
 */


#include "com/diag/desperado/Begin.h"

/**
 *  This template generates a function which provides a very rough
 *  estimate of the square root of an integer of the desired type.
 *  Using negative numbers with signed types has no meaning, but
 *  no checks are made in this regard.
 *  @author coverclock@diag.com
 */
template <typename _TYPE_>
inline _TYPE_ root(_TYPE_ datum)
{
    _TYPE_ result = 0;

    while (datum > 0) {
        result = (result << 1) | 0x1;
        datum = datum >> 2;
    }

    return result;
}

/**
 *  This template generates a simple absolute value function.
 *  @author coverclock@diag.com
 */
template <typename _TYPE_>
inline _TYPE_ absolute(_TYPE_ datum)
{
    return datum < 0 ? -datum : datum;
}


/**
 *  This template generates a simple minimum function.
 *  @author coverclock@diag.com
 */
template <typename _TYPE_>
inline _TYPE_ minimum(_TYPE_ datum1, _TYPE_ datum2)
{
    return datum1 < datum2 ? datum1 : datum2;
}

/**
 *  This template generates a simple maximum function.
 *  @author coverclock@diag.com
 */
template <typename _TYPE_>
inline _TYPE_ maximum(_TYPE_ datum1, _TYPE_ datum2)
{
    return datum1 > datum2 ? datum1 : datum2;
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Root unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestRoot(void);
#endif


#endif
