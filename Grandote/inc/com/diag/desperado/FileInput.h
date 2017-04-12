#ifndef _COM_DIAG_DESPERADO_FILEINPUT_H_
#define _COM_DIAG_DESPERADO_FILEINPUT_H_

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
 *  Declares the FileInput class.
 *
 *  @see    FileInput
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an input functor that returns data from a pre-existing
 *  C standard I/O library FILE object, the default FILE object being
 *  stdin for standard input.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class FileInput : public Input {

public:

    /**
     *  Constructor.
     *
     *  @param  fp      points to a FILE object. If no pointer to
     *                  a file object is specified, a pointer to
     *                  the standard input FILE object is used.
     */
    explicit FileInput(FILE* fp = stdin);

    /**
     *  Destructor. The FILE object is not automatically closed upon
     *  destruction.
     */
    virtual ~FileInput();

    /**
     *  Returns a pointer to the associated FILE object.
     *
     *  @return a pointer to the associated FILE object.
     */
    FILE* getFile() const;

    /**
     *  Returns the next unsigned character in the file, or EOF if
     *  End Of File has been reached.
     *
     *  @return an unsigned character as an integer if successful,
     *          EOF otherwise.
     */
    virtual int operator() ();

    /**
     *  Pushes an ASCII unsigned character (but not EOF) in an integer
     *  back to the file to be returned on the next call to the
     *  input character functor.
     *
     *  @param  ch      is the character to return to the string.
     *
     *  @return the character if successful, EOF otherwise.
     */
    virtual int operator() (int ch);

    /**
     *  Inputs a newline terminated line from the file into the
     *  buffer of the specified size. If a newline is read, it is transferred
     *  into the buffer. Guarantees that the buffer is NUL terminated
     *  if it is at least one octet in size. Guarantees that no more
     *  than the specified number of octets are returned.
     *
     *  @param  buffer  points to the buffer.
     *
     *  @param  size    is the size of the buffer in octets. Size
     *                  should be no larger than the largest possible
     *                  signed integer.
     *
     *  @return the number of octets transferred (which may be zero),
     *          including the NUL, if successful, EOF otherwise.
     */
    virtual ssize_t operator() (char* buffer, size_t size);

    /**
     *  Inputs binary data into a buffer from a file. At least the
     *  minimum number of octets are input unless EOF or error occurs.
     *  Up to the maximum number may be input if it can be done without
     *  further blocking. The functor does not NUL terminate the buffer.
     *
     *  N.B. This implementation interprets "without blocking" to mean "without
     *  doing a physical read(2) in to the standard I/O buffer". It uses a
     *  non-portable and non-thread safe mechanism that directly accesses the
     *  underlying libc FILE structure. The caller can circumvent this by using
     *  the same value for minimum and maximum. In practice, specifying zero for
     *  minimum isn't useful, at least not always: since the standard I/O
     *  library performs lazy I/O (that is, not until it's really necessary),
     *  once the buffer is empty it will stay empty until a non-zero minimum is
     *  specified to force a read(2).
     *
     *  @param  buffer  points to the buffer.
     *
     *  @param  minimum is the minimum number of octets to input.
     *
     *  @param  maximum is the maximum number of octets to input.
     *
     *  @return the number of octets input (which may be any number less
     *          than maximum including zero) if successful, EOF otherwise.
     */
    virtual ssize_t operator() (
        void* buffer,
        size_t minimum,
        size_t maximum
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

private:

    /**
     *  This points to the FILE object.
     */
    FILE* file;

    /**
     * This saves the last error number.
     */
    int error;

};


//
//  Return a pointer to the associated FILE object.
//
inline FILE* FileInput::getFile() const {
    return this->file;
}

#include "com/diag/desperado/End.h"


#endif
