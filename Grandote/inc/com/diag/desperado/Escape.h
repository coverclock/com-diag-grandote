#ifndef _COM_DIAG_DESPERADO_ESCAPE_H_
#define _COM_DIAG_DESPERADO_ESCAPE_H_

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
 *  Declares the Escape class.
 *
 *  @see    Escape
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements methods to collapse and expand strings containing ANSI C-style
 *  escape sequences.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Escape : public Object {

public:

    /**
     *  Constructor.
     */
    explicit Escape();

    /**
     *  Destructor.
     */
    virtual ~Escape();

    /**
     *  Copies the NUL-terminated input buffer into the output buffer
     *  while collapsing any ANSI C escape sequences into their eight-bit
     *  binary equivalents. The output buffer is guaranteed to be
     *  NUL terminated, but it may also have embedded NULs by virtue
     *  of their having been encoded as escape sequences in the input
     *  buffer.
     *
     *  @param  from    points to the NUL-terminated input buffer.
     *
     *  @param  to      points to the output buffer.
     *
     *  @param  tsize   is the size of the output buffer in bytes.
     *                  The output is truncated to fit in this
     *                  size buffer. The output buffer is guaranteed
     *                  to be NUL-terminated.
     *
     *  @return the number of bytes in the output buffer including the
     *          terminating NUL character.
     */
    size_t collapse(
        const char* from,
        char* to,
        size_t tsize
    );

    /**
     *  Copies the input buffer into the output buffer while expanding
     *  eight-bit binary unprintable characters and any additional
     *  specified special characters into their equivalent ANSI C
     *  escape sequences. Since the input buffer may contain NUL
     *  characters, its length must be specified explicitly and need
     *  not be NUL-terminated. The output buffer is guaranteed to be
     *  NUL-terminated.
     *
     *  @param  from    points to the input buffer.
     *
     *  @param  fsize   is the size of the input buffer in bytes.
     *
     *  @param  to      points to the output buffer.
     *
     *  @param  tsize   is the size of the output buffer in bytes.
     *
     *  @param  special points to NUL-terminated ASCII string containing
     *                  additional characters to expand by escaping
     *                  them. This may be set to expand characters
     *                  which have special meaning to other components
     *                  in the application into multi-character escape
     *                  sequences. The default is the only character
     *                  special to this class, the backslash.
     *
     *  @return the number of bytes in the output buffer including the
     *          terminating NUL character.
     */
    size_t expand(
        const char* from,
        size_t fsize,
        char* to,
        size_t tsize,
        const char* special = "\\"
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
// Constructor
//
inline Escape::Escape() {
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the Escape unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestEscape(void);
#endif


#endif
