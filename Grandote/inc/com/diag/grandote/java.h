#ifndef _COM_DIAG_DESPERADO_JAVA_H_
#define _COM_DIAG_DESPERADO_JAVA_H_

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
 *  Defines the primitive types used by Java. This header file is not
 *  intended to replace the use of the Java Native Interface (JNI)
 *  header jni.h. It is for the use of C++ applications which may send
 *  and receive data with possibly remote Java applications over, for
 *  example, a socket, and hence may not have a Java environment on
 *  their host system.
 *
 *  This header file can included from both C++ and C translation units.
 *
 *  @see    J. Gosling, et al., <I>The Java Language Specification
 *          Second Edition</I>, Addison-Wesley, 2000
 *
 *  @see    K. Arnold, et al., <I>The Java Programming Language
 *          Third Edition</I>, Addison-Wesley, 2000
 *
 *  @see    S. Liang, <I>The Java Native Interface Programmer's Guide
 *          and Specification</I>, Addison-Wesley, 1999
 *
 *  @see    IEEE, <I>IEEE Standard for Binary Floating-Point Arithmetic</I>,
 *          Std. 754-1985, IEEE, 1985
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"


#if defined(__cplusplus)
extern "C" {
#endif


/*
 *  The Java primitive integral types are pretty simple to define.
 *  Below follows Gosling and Liang.
 */

typedef int8_t j_byte_t;
typedef uint16_t j_char_t;
typedef int16_t j_short_t;
typedef int32_t j_int_t;
typedef int32_t j_size_t;
typedef int64_t j_long_t;


/*
 *  The Java boolean type is primitive but not integral. Liang says
 *  it is an unsigned eight bit type that may only have the values
 *  true and false as defined below.
 */

typedef uint8_t j_boolean_t;

static const j_boolean_t j_false = 0;
static const j_boolean_t j_true = 1;

/**
 *  @def    BOOLEAN_J2C(_JBOOLEAN_)
 *
 *  Generates true if the Java boolean @a _JBOOLEAN_ is true,
 *  otherwise false.
 */
#define BOOLEAN_J2C(_JBOOLEAN_) ((_JBOOLEAN_) != j_false)

/**
 *  @def    BOOLEAN_C2J(_CBOOLEAN_)
 *
 *  Generates Java true if the boolean @a _CBOOLEAN_ is true,
 *  otherwise Java false.
 */
#define BOOLEAN_C2J(_CBOOLEAN_) ((_CBOOLEAN_) ? j_true : j_false)


/*
 *  The C floating point types are problematic because there is no
 *  equivalent to the POSIX stdint.h header file for them AFAIK. They
 *  may differ in size on different targets. Below we just hope that
 *  floats are thirty-two bits and doubles are sixty-four bits, and
 *  try not to think about long doubles. Your mileage may unfortunately
 *  vary. We are also assuming the target uses the standard IEEE 754
 *  floating point standard. These are pretty reasonable assumptions,
 *  but if they do not hold, good luck. (Better ideas welcome.)
 */

typedef float j_float_t;

typedef double j_double_t;


/*
 *  The Java object type is just meant to take up as much space as
 *  a Java object reference. If you really want to manipulate Java
 *  objects, you must resort to using JNI. Note that there is no
 *  guarantee that either Java object references, or Java object
 *  identity hash codes, are actually pointers,
 */

typedef struct j_opaque_t j_opaque_t;

typedef j_opaque_t *j_object_t;


#if defined(__cplusplus)
}
#endif


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the C++ CXXJAPI unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestcxxjapi();
/**
 *  Run the C CXXJAPI unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestcxxjapi2(void);
#endif


#endif
