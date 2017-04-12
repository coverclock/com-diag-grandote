#ifndef _COM_DIAG_DESPERADO_ARGUMENT_H_
#define _COM_DIAG_DESPERADO_ARGUMENT_H_

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
 *  Declares the Argument class.
 *
 *  @see    Argument
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"

#include "com/diag/desperado/Begin.h"

/**
 *  Implements a simple parser using strtok_r(3) to break up a character
 *  string into an C-style argument vector.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Argument : public Object {

public:

    /**
     *  Constructor.
     */
    explicit Argument();

    /**
     *  Destructor.
     */
    virtual ~Argument();

    /**
     *  Parses a string containing words delimited by tokens into an
     *  argument vector suitable for passing to a main-type program. Returns
     *  the argument count, or argc. <I>The input character string is modified
     *  in place</I> to insert NUL characters, and the argument vector ends up
     *  containing pointers into the substrings that are now in the string.
     *  This is not smart enough to respect quotes of any kind. If there is
     *  room in the array, and if the returned value is greater than zero
     *  indicating at least one argument was extracted from the character
     *  string, a null pointer is inserted past the last entry in the array.
     *
     *  Example:
     *
     *  
     *  char argv[64];<BR>
     *  int argc;<BR>
     *  char string[64];
     *  strcpy(string, "Every good boy does fine.");
     *  argc = Argument::vector(string, argv, sizeof(argv)/sizeof(argv[0]);<BR>
     *  
     *
     *  yields
     *
     *  
     *  argc = 5;<BR>
     *  argv[0] = "Every";<BR>
     *  argv[1] = "good";<BR>
     *  argv[2] = "boy";<BR>
     *  argv[3] = "does";<BR>
     *  argv[4] = "fine.";<BR>
     *  argv[5] = 0;<BR>
     *  
     *
     *  @see    strtok_r(3)
     *
     *  @param  string      points to the non-const input character string.
     *
     *  @param  argv        points to the argument vector to fill in.
     *
     *  @param  maximum     is the maximum number of arguments to fill
     *                      into the argument vector.
     *
     *  @param  delimiter   points to a NUL-terminated array of characters
     *                      containing the set of delimiter tokens. The
     *                      default token set contains the whitespace
     *                      characters SPACE, HT, CR and NL. Using an
     *                      ampersand is a convenient way to break apart
     *                      a URL query string from a web server.
     *
     *  @return the argument count, also known as argc.
     */
    virtual int vector(
        char* string,
        char* argv[],
        int maximum,
        const char* delimiter = "\t\n\r "
    );

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

};


//
//  Constructor.
//
inline Argument::Argument() {
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Argument unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestArgument(void);
#endif


#endif
