#ifndef _COM_DIAG_DESPERADO_BYTEORDER_H_
#define _COM_DIAG_DESPERADO_BYTEORDER_H_

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
 *  Declares the ByteOrder class.
 */


#include "com/diag/desperado/target.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Template defines a generalized method for swapping bytes.
 *  It is templatized on the input type, the output type, and
 *  the control type. For integers, these are all the same.
 *  For floats, the control type is always an integer type the
 *  same sizeof as the float, and the input and output types
 *  depends on the direction, where the network input or output
 *  type is always an integer, and the host input or output type
 *  is always the float, to prevent hardware normalization.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
template <typename _INPUT_, typename _OUTPUT_, typename _CONTROL_>
union ByteOrder {

private:

    _CONTROL_ control;

    _INPUT_ input;

    _OUTPUT_ output;

    unsigned char bytes[sizeof(_CONTROL_)];

public:

    /**
     * Ctor.
     * @param cc is the control word.
     */
    ByteOrder(_CONTROL_ cc = 0) : control(cc) {}

    /**
     * Dtor.
     */
    ~ByteOrder() {}

    /**
     * Returns true if network byte order is not the
     * same as host byte order. Note that a single
     * byte data type will always look little-endian,
     * but the swap method effectively does nothing.
     * @return true if network byte order is not the
     * same as host byte order.
     */
    static bool mustswap() {
        static const ByteOrder swappable(1);
        return swappable.bytes[0] != 0;
    }

    /**
     * Unconditionally swap bytes in a word.
     * @param data is the the input word.
     * @return the output word.
     */
    static _OUTPUT_ swap(_INPUT_ data) {
        ByteOrder in;
        ByteOrder out;
        in.input = data;
        for (size_t ii = 0; ii < sizeof(data); ++ii) {
            out.bytes[ii] = in.bytes[sizeof(data) - 1 - ii];
        }
        return out.output;
    }

    /**
     * Convert the input word to the output word
     * without swapping bytes.
     * @param data is the the input word.
     * @return the output word.
     */
    static _OUTPUT_ noswap(_INPUT_ data) {
        ByteOrder convert;
        convert.input = data;
        return convert.output;
    }

    /**
     * Conditionally swap bytes in a word.
     * @param data is the the input word.
     * @return the output word.
     */
    static _OUTPUT_ swapif(_INPUT_ data) {
        return mustswap() ? swap(data) : noswap(data);
    }

};


#define DESPERADO_BYTEORDER_INTEGER(_ITYPE_) \
    inline _ITYPE_ ntoh(_ITYPE_ data) { \
        return ByteOrder<_ITYPE_, _ITYPE_, _ITYPE_>::swapif(data); \
    } \
    inline _ITYPE_ hton(_ITYPE_ data) { \
        return ByteOrder<_ITYPE_, _ITYPE_, _ITYPE_>::swapif(data); \
    }

#define DESPERADO_BYTEORDER_FLOAT(_FTYPE_, _ITYPE_) \
    inline _FTYPE_ fntoh(_ITYPE_ data) { \
        return ByteOrder<_ITYPE_, _FTYPE_, _ITYPE_>::swapif(data); \
    } \
    inline _ITYPE_ fhton(_FTYPE_ data) { \
        return ByteOrder<_FTYPE_, _ITYPE_, _ITYPE_>::swapif(data); \
    }

/*
 *  I would have much preferred to have used the <stdint.h>
 *  types (uint64_t, etc.) but it turns out doing so introduces
 *  some ambiguity that GCC 4.1.3 apparently isn't prepared to handle.
 *  There is also an issue of whether a int32_t/uint32_t is a
 *  long or an int; this differs on different platforms and
 *  seems to confuse things.
 */

DESPERADO_BYTEORDER_INTEGER(unsigned long long)
DESPERADO_BYTEORDER_INTEGER(unsigned long)
DESPERADO_BYTEORDER_INTEGER(unsigned int)
DESPERADO_BYTEORDER_INTEGER(unsigned short)
DESPERADO_BYTEORDER_INTEGER(unsigned char)

DESPERADO_BYTEORDER_INTEGER(signed long long)
DESPERADO_BYTEORDER_INTEGER(signed long)
DESPERADO_BYTEORDER_INTEGER(signed int)
DESPERADO_BYTEORDER_INTEGER(signed short)
DESPERADO_BYTEORDER_INTEGER(signed char)

/*
 * Here we need to have integer types the same bit width as the
 * floating point types, so we resort to using <stdint.h> and
 * our own floating point types. We don't support long double
 * since there is usually no integer type of its width, and to
 * make matters worse, the width of long double varies widely
 * from platform to platform.
 */

DESPERADO_BYTEORDER_FLOAT(float64_t, uint64_t)
DESPERADO_BYTEORDER_FLOAT(float32_t, uint32_t)

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the ByteOrder unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestByteOrder(void);
#endif


#endif
