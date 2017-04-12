#ifndef _COM_DIAG_DESPERADO_GENERICS_H_
#define _COM_DIAG_DESPERADO_GENERICS_H_

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
 *  Defines some useful preprocessor macros in support of generic
 *  programming, including a replacement for the system-defined
 *  offsetof() macro. It can be included from either
 *  C or C++ translation units. The macro names mimic the all lower case
 *  convention of the standard sizeof() operator. None of these macros
 *  implement rocket science, but they sure are handy to have around.
 *
 *  What's the deal with desperado_offsetof()? Decades ago (no, really)
 *  the offsetof() macro didn't exist. So we wrote our own, But at the
 *  time we were working at place that pretty much had one of everything.
 *  Fun to play, nightmare to write portable code. Epecially, for the
 *  CRAY Y-MP, for which (among other weirdnesses) the base address of a
 *  C structure was not at offset zero. So we had to come up with an
 *  implementation that worked on everything. This is a reimplementation
 *  that works on the Y-MP. Now we would just use the standard offsetof(),
 *  which I would expect to work on any platform.
 *
 *  The use of preprocessor macros here is troublesome. Preprocessor
 *  symbols are like old landmines, hiding just below the surface
 *  waiting to blow something up. I tried implementing some of these
 *  as templatized functions, but that turns out to be remarkably
 *  difficult; I am tempted to say impossible, at least in general, for
 *  all of the macros defined here. The much maligned C preprocessor
 *  is a powerful form of reuse known as code generation. Like all tools
 *  of great power, with its use comes great responsibility. I have not
 *  given up trying to find a good way to do without it.
 *
 *  See integers.h for implementations of some of the more useful generics
 *  as C++ templates. Alas, GNU g++ 4.4.3 doesn't allow you to call functions
 *  from constant expressions, which is exactly where you most want to use
 *  these, and even though they can be resolved at compile time. I believe this
 *  is because static const expressions are actually supported by the C
 *  preprocessor as text substitution, not by the compiler.
 *
 *  Doing without these macros is also difficult. Symbols like INT_MAX
 *  from limits.h are not adequate when doing generic programming, for
 *  example using templates, where the type of a variable cannot be
 *  known in advance.
 *
 *  This is the header file formerly known as of.h.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#if defined(__cplusplus)
#   include <cstddef>
#else
#   include <stddef.h>
#endif
#include "com/diag/desperado/target.h"


#if defined(__cplusplus)
    /**
     *  @def    reinterpretcastto(_TYPE_, _VARIABLE_)
     *
     *  Generates a C++ reinterpret cast of variable @a _VARIABLE_ to
     *  type @a _TYPE_.
     */
#   define reinterpretcastto(_TYPE_, _VARIABLE_) \
        (reinterpret_cast<_TYPE_>(_VARIABLE_))
    /**
     *  @def    staticcastto(_TYPE_, _VARIABLE)
     *
     *  Generates a C++ static cast of variable @a _VARIABLE_ to
     *  type @a _TYPE_.
     */
#   define staticcastto(_TYPE_, _VARIABLE_) \
        (static_cast<_TYPE_>(_VARIABLE_))
#else
    /**
     *  @def    reinterpretcastto(_TYPE_, _VARIABLE_)
     *
     *  Generates a C traditional cast of variable @a _VARIABLE_ to
     *  type @a _TYPE_.
     */
#   define reinterpretcastto(_TYPE_, _VARIABLE_) \
        ((_TYPE_)(_VARIABLE_))
    /**
     *  @def    staticcastto(_TYPE_, _VARIABLE_)
     *
     *  Generates a C traditional cast of variable @a _VARIABLE_ to
     *  type @a _TYPE_, returning the result of the cast.
     */
#   define staticcastto(_TYPE_, _VARIABLE_) \
        ((_TYPE_)(_VARIABLE_))
#endif


/**
 *  @def    nullpointerto(_TYPE_)
 *
 *  Generates a null pointer to an object of type _TYPE_.
 */
#define nullpointerto(_TYPE_) \
    staticcastto(_TYPE_*, 0)


/**
 *  @def    desperado_offsetof(_STRUCT_, _MEMBER_)
 *
 *  Generates the byte offset of the member @a _MEMBER_ within the class or
 *  struct @a _STRUCT_, working, unlike many other implementations of the
 *  standard offsetof(), on unusual architectures like the Cray Y-MP. The
 *  typical implementation assumes that the beginning of the structure
 *  starts at offset zero; this is not the case on some architectures.
 */
#if 0
#define desperado_offsetof(_STRUCT_, _MEMBER_) \
    reinterpretcastto(size_t, staticcastto(char*, &(nullpointerto(_STRUCT_)->_MEMBER_)) - staticcastto(char*, nullpointerto(_STRUCT_)))
#else
#define desperado_offsetof(_STRUCT_, _MEMBER_) \
    staticcastto(size_t, reinterpretcastto(uintptr_t, &(nullpointerto(_STRUCT_)->_MEMBER_)) - reinterpretcastto(uintptr_t, nullpointerto(_STRUCT_)))
#endif


/**
 *  @def    originof(_STRUCT_, _MEMBER_, _POINTER_)
 *
 *  Generates the address of an object of the class or struct @a _STRUCT_
 *  given the pointer @a _POINTER_ of a member of that type @a _MEMBER_.
 */
#define originof(_STRUCT_, _MEMBER_, _POINTER_) \
     reinterpretcastto(_STRUCT_*, reinterpretcastto(char*, (_POINTER_)) - desperado_offsetof(_STRUCT_, _MEMBER_))


/**
 *  @def    countof(_ARRAY_)
 *
 *  Generates the number of elements in the array @a _ARRAY_ whose single
 *  dimension is known at compile time.
 */
#define countof(_ARRAY_) \
    staticcastto(size_t, sizeof(_ARRAY_) / sizeof(_ARRAY_[0]))


/**
 *  @def    widthof(_TYPE_)
 *
 *  Generates the width in bits of the type or variable @a _TYPE_.
 */
#define widthof(_TYPE_) \
    staticcastto(size_t, sizeof(_TYPE_) * 8)


/**
 *  @def    membersizeof(_STRUCT_, _MEMBER_)
 *
 *  Generates the sizeof a member @a _MEMBER_ of a class or struct
 *  @a _STRUCT_ without needing an instance of that type.
 */
#define membersizeof(_STRUCT_, _MEMBER_) \
    sizeof(nullpointerto(_STRUCT_)->_MEMBER_)


/**
 *  @def    sizedownby(_VALUE_, _SIZEOF_)
 *
 *  Generates value that is the variable or value @a _VALUE_ sized
 *  down to an integral multiple of the value @a _SIZEOF_.
 */
#define sizedownby(_VALUE_, _SIZEOF_) \
    staticcastto(size_t, staticcastto(size_t, _VALUE_) & (~((_SIZEOF_) - 1)))


/**
 *  @def    sizedownto(_VALUE_, _TYPE_)
 *
 *  Generates a value that is the variable or value @a _VALUE_ sized
 *  down to an integral multiple of the sizeof the type @a _TYPE_.
 */
#define sizedownto(_VALUE_, _TYPE_) \
    sizedownby((_VALUE_), sizeof(_TYPE_))


/**
 *  @def    sizeupby(_VALUE_, _SIZEOF_)
 *
 *  Generates a value that is the variable or value @a _VALUE_
 *  sized up to an integral multiple of the value @a _SIZEOF_.
 */
#define sizeupby(_VALUE_, _SIZEOF_) \
     staticcastto(size_t, (staticcastto(size_t, (_VALUE_)) + ((_SIZEOF_) - 1)) & (~((_SIZEOF_) - 1)))


/**
 *  @def    sizeupto(_VALUE_, _TYPE_)
 *
 *  Generates a value that is the variable or value @a _VALUE_
 *  sized up to an integral multiple of the sizeof the type @a _TYPE_.
 */
#define sizeupto(_VALUE_, _TYPE_) \
    sizeupby((_VALUE_), sizeof(_TYPE_))


/**
 *  @def    aligndownby(_POINTER_, _SIZEOF_)
 *
 *  Generates an address that is the value of the pointer or value
 *  @a _POINTER_ aligned down to an integral multiple of the value
 *  @a _SIZEOF_.
 */
#define aligndownby(_POINTER_, _SIZEOF_) \
    reinterpretcastto(void*, reinterpretcastto(uintptr_t, _POINTER_) & (~((_SIZEOF_) - 1)))


/**
 *  @def    aligndownto(_POINTER_, _TYPE_)
 *
 *  Generates an address that is the value of the pointer or value
 *  @a _POINTER_ aligned down to an integral multiple of the sizeof
 *  the type @a _TYPE_. N.B. if _TYPE_ is a 64-bit types, the resulting
 *  pointer will be double-word aligned even though many architectures
 *  align 64-bit types on 32-bit boundaries.
 */
#define aligndownto(_POINTER_, _TYPE_) \
    aligndownby((_POINTER_), sizeof(_TYPE_))


/**
 *  @def    alignupby(_POINTER_, _SIZEOF_)
 *
 *  Generates an address that is the value of the pointer or value
 *  @a _POINTER_ aligned up to an integral multiple of the value
 *  @a _SIZEOF_.
 */
#define alignupby(_POINTER_, _SIZEOF_) \
     reinterpretcastto(void*, (reinterpretcastto(uintptr_t, (_POINTER_)) + ((_SIZEOF_) - 1)) & (~((_SIZEOF_) - 1)))


/**
 *  @def    alignupto(_POINTER_, _TYPE_)
 *
 *  Generates an address that is the value of the pointer or value
 *  @a _POINTER_ aligned up to an integral multiple of the sizeof
 *  the type @a _TYPE_. N.B. if _TYPE_ is a 64-bit types, the resulting
 *  pointer will be double-word aligned even though many architectures
 *  align 64-bit types on 32-bit boundaries.
 */
#define alignupto(_POINTER_, _TYPE_) \
    alignupby((_POINTER_), sizeof(_TYPE_))


/**
 *  @def    isalignedby(_POINTER_, _SIZEOF_)
 *
 *  Generates true if and only if the pointer or value @a _POINTER_ is
 *  aligned to an integral multiple of the value @a _SIZEOF_.
 */
#define isalignedby(_POINTER_, _SIZEOF_) \
    ((reinterpretcastto(uintptr_t, (_POINTER_)) & ((_SIZEOF_) - 1)) == 0)


/**
 *  @def    isalignedto(_POINTER_, _TYPE_)
 *
 *  Generates true if and only if the pointer or value @a _POINTER_ is
 *  aligned to an integral multiple of the sizeof the type @a _TYPE_.
 *  N.B. if _TYPE_ is a 64-bit type, it will be assumed to be double word
 *  aligned even though many archtitectures align 64-bit types on 32-bit
 *  boundaries.
 */
#define isalignedto(_POINTER_, _TYPE_) \
    isalignedby((_POINTER_), sizeof(_TYPE_))


/**
 *  @def    indexof(_POINTER_, _ARRAY_)
 *
 *  Generates an array index given a pointer @a _POINTER_ to an
 *  element in the array @a _ARRAY_.
 */
#define indexof(_POINTER_, _ARRAY_) \
    ((_POINTER_) - (_ARRAY_))


/**
 *  @def    unsignedintminof(_TYPE_)
 *
 *  Generates the minimum unsigned integer value which can be
 *  contained in the integral type @a _TYPE_.
 */
#define unsignedintminof(_TYPE_) \
    staticcastto(_TYPE_, 0)


/**
 *  @def    unsignedintmaxof(_TYPE_)
 *
 *  Generates the maximum unsigned integer value which can be
 *  contained in the integral type @a _TYPE_.
 */
#define unsignedintmaxof(_TYPE_) \
    staticcastto(_TYPE_, (~unsignedintminof(_TYPE_)))


/**
 *  @def    signedintminof(_TYPE_)
 *
 *  Generates the minimum signed integer value which can be
 *  contained in the integral type @a _TYPE_.
 */
#define signedintminof(_TYPE_) \
    staticcastto(_TYPE_, (staticcastto(_TYPE_, 1) << (widthof(_TYPE_) - 1)))


/**
 *  @def    signedintmaxof(_TYPE_)
 *
 *  Generates the maximum signed integer value which can be
 *  contained in the integral type @a _TYPE_.
 */
#define signedintmaxof(_TYPE_) \
    staticcastto(_TYPE_, (~signedintminof(_TYPE_)))


/**
 *  @def    issignedint(_TYPE_)
 *
 *  Generates true if and only if the integral type @a _TYPE_ is
 *  signed.
 */
#define issignedint(_TYPE_) \
    (!(0 < signedintminof(_TYPE_)))


/**
 *  @def    intminof(_TYPE_)
 *
 *  Generates the minimum integer value, signed or unsigned as appropriate,
 *  which can be contained in the integral type @a _TYPE_.
 */
#define intminof(_TYPE_) \
    (issignedint(_TYPE_) ? signedintminof(_TYPE_) : unsignedintminof(_TYPE_))


/**
 *  @def    intmaxof(_TYPE_)
 *
 *  Generates the maximum integer value, signed or unsigned as appropriate,
 *  which can be contained in the integral type @a _TYPE_.
 */
#define intmaxof(_TYPE_) \
    (issignedint(_TYPE_) ? signedintmaxof(_TYPE_) : unsignedintmaxof(_TYPE_))


/**
 *  @def    rangeof(_TYPE_, _RESULT_)
 *
 *  Generates the number of unique values which can be contained in the
 *  integral type @a _TYPE_ and returns the result as a value of type
 *  @a _RESULT_. For example, the rangeof(uint8_t, size_t) is 256.
 *  The rangeof _TYPE_ by definition cannot be held a value of type
 *  _TYPE_. For example, the rangeof size_t cannot be held in a variable
 *  of type _size_t. Hence, the type of the result must be explicitly
 *  specified, for example rangeof(uint32_t, uint64_t).
 */
#define rangeof(_TYPE_, _RESULT_) \
    staticcastto(_RESULT_, staticcastto(_RESULT_, unsignedintmaxof(_TYPE_)) \
        + staticcastto(_RESULT_, 1))


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the generics unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestgenerics(void);
/**
 *  Run the generics CXXCAPI unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestgenerics2(void);
#endif


#endif
