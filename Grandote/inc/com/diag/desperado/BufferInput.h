#ifndef _COM_DIAG_DESPERADO_BUFFERINPUT_H_
#define _COM_DIAG_DESPERADO_BUFFERINPUT_H_

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
 *  Declares the BufferInput class.
 *
 *  @see    BufferInput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an input functor that returns data from a buffer in
 *  memory. An EOF is returned when a NUL or the end of the buffer is reached.
 *  The offset is the total number of characters input, and can be used as
 *  an index into the character string.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class BufferInput : public Input {

public:

    /**
     *  Constructor. Since the length of the string is not specified,
     *  it is assumed to be NUL terminated.
     *
     *  @param  sp      points to the input string. The buffer must not be
     *                  const so that the character push back capability can
     *                  be implemented.
     */
    explicit BufferInput(char* sp = 0);

    /**
     *  Constructor. The length of the buffer is specified explicitly.
     *
     *  @param  sp      points to the input buffer. The buffer must not be
     *                  const so that the character push back capability can
     *                  be implemented.
     *
     *  @param  sz      is the length of the buffer in bytes.
     */
    explicit BufferInput(void* sp, size_t sz);

    /**
     *  Destructor.
     */
    virtual ~BufferInput();

    /**
     *  Returns the next character in the buffer.
     *
     *  @return a character in an integer if successful, EOF otherwise.
     */
    virtual int operator() ();

    /**
     *  Pushes an character in an integer back to the buffer to be
     *  returned on the next call to the input character functor.
     *  Only one pushed back character is guaranteed to succeed in
     *  between input character operations. The character pushed back
     *  does not have to be the previous character input,
     *  or even any character that was ever input, although at least
     *  one character must have been previously input.
     *
     *  @param  ch      is the character to push back into the input.
     *
     *  @return the pushed back character is successful, EOF
     *          otherwise.
     */
    virtual int operator() (int ch);

    /**
     *  Inputs a newline terminated line from the buffer into
     *  the buffer of the specified size. If a newline is read, it is
     *  transferred into the buffer. Guarantees that the buffer is NUL
     *  terminated if it is at least one octet in size. Guarantees that
     *  no more than the specified number of octets are returned.
     *
     *  @param  bp  	points to the output buffer.
     *
     *  @param  size    is the size of the buffer in octets. Size
     *                  should be no larger than the largest possible
     *                  signed integer.
     *
     *  @return the number of octets transferred (which may be zero),
     *          if successful, EOF otherwise.
     */
    virtual ssize_t operator() (char* bp, size_t size);

    /**
     *  Inputs binary data into a buffer from a buffer. At least the
     *  minimum number of octets are input unless EOF or error occurs.
     *  Up to the maximum number may be input if they are available.
     *  The functor does not NUL terminate the buffer nor does a
     *  NUL character in the buffer terminate the input.
     *
     *  @param  bp  	points to the output buffer.
     *
     *  @param  minimum is the minimum number of octets to input.
     *
     *  @param  maximum is the maximum number of octets to input.
     *
     *  @return the number of octets input (which may be any number less
     *          than maximum including zero) if successful, EOF otherwise.
     */
    virtual ssize_t operator() (
        void* bp,
        size_t minimum,
        size_t maximum
    );

    /**
     *  Returns a pointer to the buffer.
     *  @return a pointer to the buffer.
     */
    void* getBuffer() const;

    /**
     *  Returns the size of the buffer.
     *  @return the size of the buffer.
     */
    size_t getSize() const;

    /**
     *  Returns the current offset into the buffer, indicating how
     *  many characters have been consumed so far.
     *  @return the current offset into the buffer.
     */
    size_t getOffset() const;

    /**
     * Returns the length of the available data.
     * @return the length of the available data.
     */
    size_t getLength() const;

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
     *  This points to the beginning of the character buffer.
     */
    char* buffer;

    /**
     *  This is the size of the buffer.
     */
    size_t size;

    /**
     *  This is the offset into the character buffer.
     */
    size_t offset;

};


//
//  Return a pointer to the buffer.
//
inline void* BufferInput::getBuffer() const {
    return this->buffer;
}


//
//  Return a pointer to the buffer.
//
inline size_t BufferInput::getSize() const {
    return this->size;
}


//
//  Return the offset into the buffer.
//
inline size_t BufferInput::getOffset() const {
    return this->offset;
}

//
// Return the length of the buffer.
//
inline size_t BufferInput::getLength() const {
	return (this->size - this->offset);
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the BufferInput unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestBufferInput(void);
#endif


#endif
