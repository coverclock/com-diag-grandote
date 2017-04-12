#ifndef _COM_DIAG_DESPERADO_VOLATILE_H_
#define _COM_DIAG_DESPERADO_VOLATILE_H_

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
 *  Defines the Desperado Volatile class that provides helper
 *  functions for volatile accesses. The following C functions
 *  are defined.
 *
 *  uint8_t volatile_value_uint8_t(volatile uint8_t *);
 *  uint16_t volatile_value_uint16_t(volatile uint16_t *);
 *  uint32_t volatile_value_uint32_t(volatile uint32_t *);
 *  uint64_t volatile_value_uint64_t(volatile uint64_t *);
 *
 *  int8_t volatile_value_int8_t(volatile int8_t *);
 *  int16_t volatile_value_int16_t(volatile int16_t *);
 *  int32_t volatile_value_int32_t(volatile int32_t *);
 *  int64_t volatile_value_int64_t(volatile int64_t *);
 *
 *  volatile uint8_t * volatile_address_uint8_t(volatile uint8_t *);
 *  volatile uint16_t * volatile_address_uint16_t(volatile uint16_t *);
 *  volatile uint32_t * volatile_address_uint32_t(volatile uint32_t *);
 *  volatile uint64_t * volatile_address_uint64_t(volatile uint64_t *);
 *
 *  volatile int8_t * volatile_address_int8_t(volatile int8_t *);
 *  volatile int16_t * volatile_address_int16_t(volatile int16_t *);
 *  volatile int32_t * volatile_address_int32_t(volatile int32_t *);
 *  volatile int64_t * volatile_address_int64_t(volatile int64_t *);
 *
 *  @see    E. Eide, J. Regehr, "Volatiles Are Miscompiled, and
 *          What to Do about It", <I>Proceedings of the Eighth
 *          ACM and IEEE International Conference on Embedded
 *          Software</i>, Atlanta GA, Oct 2008
 *
 *  This can be included from either a C or a C++ translation unit.
 * 
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/cxxcapi.h"


#define DESPERADO_VOLATILE_VALUE_C_DECL(_TYPE_) \
CXXCAPI _TYPE_ volatile_value_##_TYPE_(volatile _TYPE_ * vp);

#define DESPERADO_VOLATILE_ADDRESS_C_DECL(_TYPE_) \
CXXCAPI volatile _TYPE_ * volatile_address_##_TYPE_(volatile _TYPE_ * vp);

#define DESPERADO_VOLATILE_VALUE_C_IMPL(_TYPE_) \
CXXCAPI _TYPE_ volatile_value_##_TYPE_(volatile _TYPE_ * vp) { \
    return *vp; \
}

#define DESPERADO_VOLATILE_ADDRESS_C_IMPL(_TYPE_) \
CXXCAPI volatile _TYPE_ * volatile_address_##_TYPE_(volatile _TYPE_ * vp) { \
    return vp; \
}

#define DESPERADO_VOLATILE_VALUE_CXX(_TYPE_) \
static inline _TYPE_ value(volatile _TYPE_ * vp) { \
    return volatile_value_##_TYPE_(vp); \
} \
static inline _TYPE_ value(volatile _TYPE_ & vr) { \
    return volatile_value_##_TYPE_(&vr); \
}

#define DESPERADO_VOLATILE_ADDRESS_CXX(_TYPE_) \
static inline volatile _TYPE_ * address(volatile _TYPE_ * vp) { \
    return volatile_address_##_TYPE_(vp); \
} \
static inline volatile _TYPE_ * address(volatile _TYPE_ & vr) { \
    return volatile_address_##_TYPE_(&vr); \
}

#define DESPERADO_VOLATILE_REFERENCE_CXX(_TYPE_) \
static inline volatile _TYPE_ & reference(volatile _TYPE_ * vp) { \
    return *volatile_address_##_TYPE_(vp); \
} \
static inline volatile _TYPE_ & reference(volatile _TYPE_ & vr) { \
    return *volatile_address_##_TYPE_(&vr); \
}

DESPERADO_VOLATILE_VALUE_C_DECL(int8_t)
DESPERADO_VOLATILE_VALUE_C_DECL(int16_t)
DESPERADO_VOLATILE_VALUE_C_DECL(int32_t)
DESPERADO_VOLATILE_VALUE_C_DECL(int64_t)
DESPERADO_VOLATILE_VALUE_C_DECL(uint8_t)
DESPERADO_VOLATILE_VALUE_C_DECL(uint16_t)
DESPERADO_VOLATILE_VALUE_C_DECL(uint32_t)
DESPERADO_VOLATILE_VALUE_C_DECL(uint64_t)

DESPERADO_VOLATILE_ADDRESS_C_DECL(int8_t)
DESPERADO_VOLATILE_ADDRESS_C_DECL(int16_t)
DESPERADO_VOLATILE_ADDRESS_C_DECL(int32_t)
DESPERADO_VOLATILE_ADDRESS_C_DECL(int64_t)
DESPERADO_VOLATILE_ADDRESS_C_DECL(uint8_t)
DESPERADO_VOLATILE_ADDRESS_C_DECL(uint16_t)
DESPERADO_VOLATILE_ADDRESS_C_DECL(uint32_t)
DESPERADO_VOLATILE_ADDRESS_C_DECL(uint64_t)


#if defined(__cplusplus)

/**
 * The Volatile class defines polymorphic inline static C++ class methods
 * to the C-callable value and address helper functions. Note that although
 * these methods are inlined, the C-callable functions are not. This is
 * crucial to implement Eide and Regher's volatile helper functions.
 */
class Volatile {

public:

    DESPERADO_VOLATILE_VALUE_CXX(int8_t)
    DESPERADO_VOLATILE_VALUE_CXX(int16_t)
    DESPERADO_VOLATILE_VALUE_CXX(int32_t)
    DESPERADO_VOLATILE_VALUE_CXX(int64_t)
    DESPERADO_VOLATILE_VALUE_CXX(uint8_t)
    DESPERADO_VOLATILE_VALUE_CXX(uint16_t)
    DESPERADO_VOLATILE_VALUE_CXX(uint32_t)
    DESPERADO_VOLATILE_VALUE_CXX(uint64_t)

    DESPERADO_VOLATILE_ADDRESS_CXX(int8_t)
    DESPERADO_VOLATILE_ADDRESS_CXX(int16_t)
    DESPERADO_VOLATILE_ADDRESS_CXX(int32_t)
    DESPERADO_VOLATILE_ADDRESS_CXX(int64_t)
    DESPERADO_VOLATILE_ADDRESS_CXX(uint8_t)
    DESPERADO_VOLATILE_ADDRESS_CXX(uint16_t)
    DESPERADO_VOLATILE_ADDRESS_CXX(uint32_t)
    DESPERADO_VOLATILE_ADDRESS_CXX(uint64_t)

    DESPERADO_VOLATILE_REFERENCE_CXX(int8_t)
    DESPERADO_VOLATILE_REFERENCE_CXX(int16_t)
    DESPERADO_VOLATILE_REFERENCE_CXX(int32_t)
    DESPERADO_VOLATILE_REFERENCE_CXX(int64_t)
    DESPERADO_VOLATILE_REFERENCE_CXX(uint8_t)
    DESPERADO_VOLATILE_REFERENCE_CXX(uint16_t)
    DESPERADO_VOLATILE_REFERENCE_CXX(uint32_t)
    DESPERADO_VOLATILE_REFERENCE_CXX(uint64_t)

};

#endif


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Volatile unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestVolatile(void);
/**
 *  Run the Volatile2 unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestVolatile2(void);
#endif


#endif
