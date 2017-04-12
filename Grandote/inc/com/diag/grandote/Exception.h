#ifndef _COM_DIAG_DESPERADO_EXCEPTION_H_
#define _COM_DIAG_DESPERADO_EXCEPTION_H_

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
 *  Declares the Exception class.
 *
 *  @see    Exception
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/exceptions.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/Object.h"


#include "com/diag/desperado/Begin.h"

/**
 *  This is the base class for Desperado classes which define exceptions.
 *  It is inspired by std::exception but does not extend it. This is so
 *  that it can extend Object without requiring multiple inheritance.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Exception : public Object {

public:

    /**
     *  Constructor.
     *
     *  @param  eventMessage    is a message associated with the
     *                          exception or null (0).
     *
     *  @param  errorNumber     is an error number that may be associated
     *                          with the exception. If zero, no errno
     *                          information is used. If negative, the
     *                          system errno value is used.
     *
     *  @param  fileName        may be the file name of the issuing
     *                          translation unit or null (0). Typically
     *                          it is set to __FILE__.
     *
     *  @param  lineNumber      may be the line number in the issuing
     *                          translation unit. Typically, it is set
     *                          to __LINE__.
     *
     *  @param functionName     may be the function name in the
     *                          issuing translation unit or null (0).
     *                          Typically this is set to __PRETTY_FUNCTION__
     *                          (GNU C or C++) or __func__ (ANSI C++).
     */
    explicit Exception(
        const char* eventMessage = 0,
        int errorNumber = 0,
        const char* fileName = 0,
        int lineNumber = 0,
        const char* functionName = 0
    ) THROWS(());

    /**
     *  Destructor.
     */
    virtual ~Exception() THROWS(());

    /**
     *  Returns a character string describing the cause of the exception.
     *
     *  @return a character string describing the cause of the exception.
     */
    virtual const char* what() const THROWS(());

    /**
     *  Displays internal information about this object to the specified
     *  output object. Useful for debugging and troubleshooting.
     *
     *  @param  level   sets the verbosity of the output. What this means
     *                  is object dependent. However, the level is passed
     *                  from outer to inner objects this object calls the
     *                  show methods of its inherited or composited objects.
     *
     *  @param display  points to the output object to which output is
     *                  sent. If null (zero), the default platform output
     *                  object is used as the effective output object. The
     *                  effective output object is passed from outer to
     *                  inner objects as this object calls the show methods
     *                  of its inherited and composited objects.
     *
     *  @param  indent  specifies the level of indentation. One more than
     *                  this value is passed from outer to inner objects
     *                  as this object calls the show methods of its
     *                  inherited and composited objects.
     */
    virtual void show(int level = 0, Output* display = 0, int indent = 0) const;

    /**
     *  This is a string describing the event.
     */
    const char* event;

    /**
     *  This is an errno value.
     */
    int error;

    /**
     *  This is a string naming the translation unit.
     */
    const char* file;

    /**
     *  This is a translation unit line number value.
     */
    int line;

    /**
     *  This is a string naming the function.
     */
    const char* function;
    
};

#include "com/diag/desperado/End.h"


#if defined(__USE_GNU)
#   define DESPERADO_EXCEPTION_FUNCTION    __PRETTY_FUNCTION__
#elif defined(__cplusplus)
#   define DESPERADO_EXCEPTION_FUNCTION    __func__
#else
#   define DESPERADO_EXCEPTION_FUNCTION    ((char *)0)
#endif


/**
 *  @def    DESPERADO_EXCEPTION(_EVENTMESSAGE_)
 *
 *  Calls the constructor for a Desperado Exception using the provided
 *  @a _EVENTMESSAGE_ and automatically filling in error, file, line,
 *  and function fields.
 */
#define DESPERADO_EXCEPTION(_EVENTMESSAGE_) \
    Exception(_EVENTMESSAGE_, errno, __FILE__, __LINE__, \
        DESPERADO_EXCEPTION_FUNCTION)


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Exception unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestException(void);
#endif


#endif
