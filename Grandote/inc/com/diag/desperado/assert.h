#ifndef _COM_DIAG_DESPERADO_ASSERT_H_
#define _COM_DIAG_DESPERADO_ASSERT_H_

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
 *  Implements an assert macro that interfaces with the Desperado platform
 *  object. Note that POSIX has some explicit requirements of how multiple
 *  inclusion of the assert.h header file works, which this file endevours
 *  to implement. Hence this header file does not follow the typical pattern.
 *  Also, we jump through some hoops to avoid including other header files,
 *  which vastly simplifies the use of this header file outside of the
 *  Desperado library implementation.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */

#include "com/diag/desperado/cxxcapi.h"

CXXCAPI void platform_fatal(const char*, int, const char*, int, const char*);

#if defined(DESPERADO_HAS_UNITTESTS)
/**
 *  Run the assert unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestassert(void);
#endif

#else

#undef DESPERADO_ASSERT_VOID
#undef DESPERADO_ASSERT_FUNCTION
#undef desperado_assert
#undef assert

#endif


#if defined(__cplusplus)
#   define DESPERADO_ASSERT_VOID        static_cast<void>(0)
#else
#   define DESPERADO_ASSERT_VOID        ((void)0)
#endif


#if defined(__USE_GNU)
#   define DESPERADO_ASSERT_FUNCTION    __PRETTY_FUNCTION__
#elif defined(__cplusplus)
#   define DESPERADO_ASSERT_FUNCTION    __func__
#else
#   define DESPERADO_ASSERT_FUNCTION    ((char *)0)
#endif


/**
 *  @def    desperado_assert(_ASSERTION_)
 *
 *  Generates a call to the platform fatal function if and only if
 *  @a _ASSERTION_ is false.
 */
#define desperado_assert(_ASSERTION_) \
    ((_ASSERTION_) ? DESPERADO_ASSERT_VOID : \
        platform_fatal("Assertion (" #_ASSERTION_ ") failed", \
            0, __FILE__, __LINE__, DESPERADO_ASSERT_FUNCTION))


#if defined(assert)
#   undef assert
#endif


#if defined(NDEBUG)
    /**
     *  @def    assert(_ASSERTION_)
     *
     *  Does nothing regardless of whether @a _ASSERTION_ is true of false.
     */
#   define assert(_ASSERTION_)      DESPERADO_ASSERT_VOID
#else
    /**
     *  @def    assert(_ASSERTION_)
     *
     *  Invokes the assertion failure function if @a _ASSERTION_ is false.
     *  Typically the assert failure function invokes the platform fatal
     *  function. This usually ends badly for the caller.
     */
#   define assert(_ASSERTION_)      desperado_assert(_ASSERTION_)
#endif
