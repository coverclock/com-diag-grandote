#ifndef _COM_DIAG_DESPERADO_PRINT_H_
#define _COM_DIAG_DESPERADO_PRINT_H_

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
 *  Declares the Print class.
 *
 *  @see    Print
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/Output.h"


#if defined(__cplusplus)


#include "com/diag/desperado/Object.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements a printf(3)-style functor that can be used to format
 *  a variadic argument list and write it to an output functor.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Print : public Object {

public:

    /**
     *  Constructor.
     *
     *  @param  po          points to an output object. If 0,
     *                      the platform output object is used.
     */
    explicit Print(Output* po = 0);

    /**
     *  Constructor.
     *
     *  @param  ro          refers to an output object.
     */
    explicit Print(Output& ro);

    /**
     *  Destructor.
     */
    virtual ~Print();

    /**
     *  Returns a reference to its output object.
     *
     *  @return a reference to its output object.
     */
    virtual Output& output() const;

    /**
     *  Formats a variadic argument list and writes the result
     *  to its output object.
     *
     *  @param  format      is the printf-style format string,
     *                      followed by zero or more arguments.
     *
     *  @return the number of characters written to its output
     *          object, or a negative number if error.
     */
    virtual ssize_t operator() (const char* format, ...);

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

private:

    /**
     *  Points to the output object to which the print is directed.
     */
    Output* ou;

};

#include "com/diag/desperado/End.h"


#endif


/**
 *  Formats a variadic argument list and writes the result
 *  to its output object.
 *
 *  @param  o           points to an output object, for example the
 *                      platform output object.
 *
 *  @param  format      is the printf-style format string,
 *                      followed by zero or more arguments.
 *
 *  @return the number of characters written to its output
 *          object, or a negative number if error.
 */
CXXCAPI size_t print_f(CXXCTYPE(::com::diag::desperado::, Output)* o, const char* format, ...);


#if defined(DESPERADO_HAS_UNITTESTS)
/**
 *  Run the Print unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestPrint(void);
#endif


#endif
