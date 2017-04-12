#ifndef _COM_DIAG_DESPERADO_EXCEPTIONS_H_
#define _COM_DIAG_DESPERADO_EXCEPTIONS_H_

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
 *  Defines some macros useful for globally enabling or disabling the
 *  use of C++ exceptions.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include <new>


#if defined(DESPERADO_HAS_EXCEPTIONS)


#include <exception>


/**
 *  @def    THROWS(_PARENLIST_)
 *
 *  Generates a declaration of a list of exceptions that may be thrown
 *  from the parenthesized list @a _PARENLIST_ if exceptions are enabled.
 */
#define THROWS(_PARENLIST_) throw _PARENLIST_

/**
 *  @def    TRY
 *
 *  Generates the code to begin a try block in which an exception
 *  may be thrown if exceptions are enabled.
 */
#define TRY                 try

/**
 *  @def    CATCH(_EXCEPTION_)
 *
 *  Generates the code to begin a catch block in which the
 *  exception @a _EXCEPTION_ may be caught if exceptions are enabled.
 */
#define CATCH(_EXCEPTION_)  catch (_EXCEPTION_)

/**
 *  @def    THROW(_EXCEPTION_)
 *
 *  Generates the code to throw the exception @a _EXCEPTION_,
 *  which may be an empty expression, if exceptions are enabled.
 */
#define THROW(_EXCEPTION_)  throw _EXCEPTION_

/**
 *  @def    NEW
 *
 *  Generates a new call with no arguments.
 */
#define NEW                 new


#else


/**
 *  @def    THROWS(_PARENLIST_)
 *
 *  Generates no code.
 */
#define THROWS(_PARENLIST_) /**/

/**
 *  @def    TRY
 *
 *  Generates an if that causes its
 *  clause to always be executed.
 */
#define TRY                 if (true)

/**
 *  @def    CATCH(_EXCEPTION_)
 *
 *  Generates an else-if that prevents its
 *  clause from ever being executed. 
 */
#define CATCH(_EXCEPTION_)  else if (false)

/**
 *  @def    THROW(_EXCEPTION_)
 *
 *  Generates a null expression that does nothing.
 */
#define THROW(_EXCEPTION_)  static_cast<void>(0)

/**
 *  @def    NEW
 *
 *  Generates a new call in which failure will return
 *  a null pointer rather than throw an exception, if
 *  exceptions are disabled.
 */
#define NEW                 new(std::nothrow)


#endif


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Exception unit test.
 *
 *  @return the number of errors detected.
 */
CXXCAPI int unittestException(void);
#endif


#endif
