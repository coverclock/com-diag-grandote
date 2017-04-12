#ifndef _COM_DIAG_DESPERADO_STRINGOUTPUT_H_
#define _COM_DIAG_DESPERADO_STRINGOUTPUT_H_

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
 *  Declares the BufferOutput class.
 *
 *  @see    BufferOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an output functor that writes data into a buffer in
 *  memory. The buffer is not automatically NUL terminated.
 *  An EOF is returned if the end of the buffer is reached.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class BufferOutput : public Output {

public:

    /**
     *  Constructor.
     *
     *  @param  sp      points to the output buffer.
     *
     *  @param  sz      is the length of the buffer in bytes.
     */
    explicit BufferOutput(void* sp = 0, size_t sz = 0);

    /**
     *  Destructor.
     */
    virtual ~BufferOutput();

    /**
     *  Outputs a character in integer form.
     *
     *  @param  c           is a character in integer form.
     *
     *  @return the output character if successful, EOF otherwise.
     */
    virtual int operator() (int c);

    /**
     *  Format a variable length argument list and output the result.
     *
     *  @param  format      is a NUL-terminated string containing a
     *                      printf-style format statement.
     *
     *  @param  ap          is a variable length argument object.
     *
     *  @return a non-negative number if successful, EOF otherwise.
     */
    virtual ssize_t operator() (const char* format, va_list ap);

    /**
     *  Outputs a string of no more than the specified length not
     *  including its terminating NUL.
     *
     *  @param  s           points to a constant NUL-terminated string.
     *
     *  @param  size        is the size of the string in octets.
     *
     *  @return the number of octets output if successful (which
     *          may be zero), EOF otherwise.
     */
    virtual ssize_t operator() (
        const char* s,
        size_t size = maximum_string_length
    );

    /**
     *  Outputs binary data from a buffer. The intent is that the
     *  operation blocks until the minimum number of requested octets
     *  are output or EOF or an error occurs. If more space is available
     *  without blocking, up to the maximum number of requested octets
     *  may be output. Specific implementations may differ. The interface
     *  only guarantees that at least the minimum number of octets are
     *  output from the buffer.
     *
     *  @param  bp		points to the buffer.
     *
     *  @param  minimum is the minimum number of octets to output.
     *
     *  @param  maximum is the maximum number of octets to output.
     *
     *  @return the number of octets output (which may be any number less
     *          than maximum including zero) if successful, EOF otherwise.
     */
    virtual ssize_t operator() (
        const void* bp,
        size_t minimum,
        size_t maximum
    );

    /**
     *  Flush any buffered data to the file. Specific implementations
     *  may do nothing if they do not buffer.
     *
     *  @return a non-negative number if successful, EOF otherwise.
     */
    virtual int operator() ();


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
     *  many characters have been produced so far.
     *  @return the current offset into the buffer.
     */
    size_t getOffset() const;

    /**
     * Returns the length of the available space.
     * @return the length of the available space.
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
     *  This points to the beginning of the buffer.
     */
    char* buffer;

    /**
     *  This is the size of the buffer.
     */
    size_t size;

    /**
     *  This is the offset into the buffer.
     */
    size_t offset;

};


//
//  Return a pointer to the buffer.
//
inline void* BufferOutput::getBuffer() const {
    return this->buffer;
}


//
//  Return a pointer to the buffer.
//
inline size_t BufferOutput::getSize() const {
    return this->size;
}


//
//  Return the offset into the buffer.
//
inline size_t BufferOutput::getOffset() const {
    return this->offset;
}

//
// Return the length of the buffer.
//
inline size_t BufferOutput::getLength() const {
	return (this->size - this->offset);
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the BufferOutput unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestBufferOutput(void);
#endif


#endif
