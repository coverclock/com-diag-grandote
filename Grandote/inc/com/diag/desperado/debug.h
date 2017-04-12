#ifndef _COM_DIAG_DESPERADO_DEBUG_H_
#define _COM_DIAG_DESPERADO_DEBUG_H_

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
 *  Defines some useful debugging macros. Similar to assert.h, this
 *  header file can be included more than once, and may generate
 *  different code depending on whether or not DESPERADO_HAS_DEBUGGING
 *  is defined.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/errno.h"

#if defined(DESPERADO_HAS_DEBUGGING)
#   include "Platform.h"
#   include "Dump.h"
#endif


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Debug unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestDebug(void);
#endif


#else


#   undef DEBUG_PRINTF
#   undef DEBUG_PRINTF_IF
#   undef DEBUG_SHOW
#   undef DEBUG_SHOW_IF
#   undef DEBUG_DUMP
#   undef DEBUG_DUMP_IF
#   undef DESPERADO_FATAL_FUNCTION
#   undef DEBUG_FATAL
#   undef DEBUG_FATAL_IF
#   undef DEBUG_TRACE


#endif


#if defined(DESPERADO_HAS_DEBUGGING)


/**
 *  @def    DEBUG_PRINTF(_PARENLIST_)
 *
 *  Generates a call to the platform error object using the
 *  parenthesized printf-style argument list @a _PARENLIST_
 *  if debugging is enabled.
 */
#define DEBUG_PRINTF(_PARENLIST_) \
    staticcastto(void, (platform_errorf _PARENLIST_))


/**
 *  @def    DEBUG_PRINTF_IF(_CONDITION_, _PARENLIST_)
 *
 *  Generates a call to the platform error object using the
 *  parenthesized printf-style argument list @a _PARENLIST_
 *  if the condition @a _CONDITION_ is met and debugging is enabled.
 */
#define DEBUG_PRINTF_IF(_CONDITION_, _PARENLIST_) \
    staticcastto(void, ((_CONDITION_) ? (platform_errorf _PARENLIST_) : 0))


#if defined(__cplusplus)

/**
 *  @def    DEBUG_SHOW(_REFERENCE_)
 *
 *  Generates a call to the show() method of a @a _REFERENCE_ if
 *  debugging is enabled. Only useful for C++ translation units.
 */
#define DEBUG_SHOW(_REFERENCE_) \
    ((_REFERENCE_).show())


/**
 *  @def    DEBUG_SHOW_IF(_CONDITION_, _REFERENCE_)
 *
 *  Generates a call to the show() method of a @a _REFERENCE_ if
 *  the condition @a _CONDITION_ is met and debugging is enabled.
 *  Only useful for C++ translation units.
 */
#define DEBUG_SHOW_IF(_CONDITION_, _REFERENCE_) \
    ((_CONDITION_) ? ((_REFERENCE_).show()) : staticcastto(void, 0))

#endif


/**
 *  @def    DEBUG_DUMP(_POINTER_, _SIZEOF_)
 *
 *  Generates a call the platform dump object using the pointer
 *  @a _POINTER_ and sizeof @a _SIZEOF_ if debugging is enabled.
 */
#define DEBUG_DUMP(_POINTER_, _SIZEOF_) \
    staticcastto(void, (dump_bytes(platform_dump(), (_POINTER_), (_SIZEOF_))))


/**
 *  @def    DEBUG_DUMP_IF(_CONDITION_, _POINTER_, _SIZEOF_)
 *
 *  Generates a call the platform dump object using the pointer
 *  @a _POINTER_ and sizeof @a _SIZEOF_ if the condition @a _CONDITION_
 *  is met and debugging is enabled.
 */
#define DEBUG_DUMP_IF(_CONDITION_, _POINTER_, _SIZEOF_) \
    staticcastto(void, ((_CONDITION_) ? (dump_bytes(platform_dump(), (_POINTER_), (_SIZEOF_))) : 0)) 


#if defined(__USE_GNU)
#   define DESPERADO_FATAL_FUNCTION __PRETTY_FUNCTION__
#elif defined(__cplusplus)
#   define DESPERADO_FATAL_FUNCTION __func__
#else
#   define DESPERADO_FATAL_FUNCTION ((char *)0)
#endif


/**
 *  @def    DEBUG_FATAL(_EVENT_)
 *
 *  Generates a call the platform fatal method using the event
 *  @a _EVENT_ if debugging is enabled.
 */
#define DEBUG_FATAL(_EVENT_) \
    platform_fatal(_EVENT_, errno, __FILE__, __LINE__, DESPERADO_FATAL_FUNCTION)


/**
 *  @def    DEBUG_FATAL_IF(_CONDITION_, _EVENT_)
 *
 *  Generates a call the platform fatal method using the event
 *  @a _EVENT_ if the condition @a _CONDITION_ is met and debugging
 *  is enabled.
 */
#define DEBUG_FATAL_IF(_CONDITION_, _EVENT_) \
    ((_CONDITION_) ? (platform_fatal(_EVENT_, errno, __FILE__, __LINE__, DESPERADO_FATAL_FUNCTION)) : staticcastto(void, 0)) 


/**
 *  @def    DEBUG_TRACE(_STATEMENT_)
 *
 *  Generates a trace output of the statement @a _STATEMENT_ by
 *  printing the statement and then executing it if debugging is
 *  enabled.
 *
 *  Not suitable for all C/C++ statements or in all source code contexts.
 *  Note that this cannot be enclosed in a code block since this would defeat
 *  the scoping of the statement, particularly if it was a declaration.
 */
#define DEBUG_TRACE(_STATEMENT_) \
    platform_errorf("%s[%d]: %s\n", __FILE__, __LINE__, #_STATEMENT_); \
    _STATEMENT_


#else


#define DEBUG_PRINTF(_PARENLIST_) \
    staticcastto(void, 0)


#define DEBUG_PRINTF_IF(_CONDITION_, _PARENLIST_) \
    staticcastto(void, 0)


#if defined(__cplusplus)

#define DEBUG_SHOW(_REFERENCE_) \
    staticcastto(void, 0)


#define DEBUG_SHOW_IF(_CONDITION_, _REFERENCE_) \
    staticcastto(void, 0)

#endif


#define DEBUG_DUMP(_POINTER_, _SIZEOF_) \
    staticcastto(void, 0)


#define DEBUG_DUMP_IF(_CONDITION_, _POINTER_, _SIZEOF_) \
    staticcastto(void, 0)


#define DEBUG_FATAL(_EVENT_) \
    staticcastto(void, 0)


#define DEBUG_FATAL_IF(_CONDITION_, _EVENT_) \
    staticcastto(void, 0)


#define DEBUG_TRACE(_STATEMENT_)    \
    _STATEMENT_


#endif
