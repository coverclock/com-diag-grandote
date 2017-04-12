#ifndef _COM_DIAG_DESPERADO_DATAINPUT_H_
#define _COM_DIAG_DESPERADO_DATAINPUT_H_

/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Declares the DataInput class.
 *
 *  @see    DataInput
 */


#include "com/diag/desperado/Input.h"
#include "com/diag/desperado/Output.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Implements an input functor that returns data from a buffer in
 *  memory. An EOF is returned when a NUL or the end of the buffer is reached.
 *  The offset is the total number of characters input, and can be used as
 *  an index into the character data.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class DataInput : public Input {

public:

    /**
     *  Constructor. Since the length of the data is not specified,
     *  it is assumed to be NUL terminated. Unlike BufferInput the data
     *  can be const.
     *
     *  @param  sp      points to the input data.
     */
    explicit DataInput(const char* sp = 0);

    /**
     *  Constructor. The length of the data is specified explicitly. Unlike
     *  BufferInput the data can be const.
     *
     *  @param  sp      points to the input data.
     *
     *  @param  sz      is the length of the data in octets.
     */
    explicit DataInput(const void* sp, size_t sz);

    /**
     *  Destructor.
     */
    virtual ~DataInput();

    /**
     *  Returns the next character in the data.
     *
     *  @return a character in an integer if successful, EOF otherwise.
     */
    virtual int operator() ();

    /**
     *  Pushes an character in an integer back to the data to be
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
     *  Inputs a newline terminated line from the data into
     *  the buffer of the specified size. If a newline is read, it is
     *  transferred into the buffer. Guarantees that the buffer is NUL
     *  terminated if it is at least one octet in size. Guarantees that
     *  no more than the specified number of octets are returned.
     *
     *  @param  bp		points to the buffer.
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
     *  Inputs binary data into a buffer from a data. At least the
     *  minimum number of octets are input unless EOF or error occurs.
     *  Up to the maximum number may be input if they are available.
     *  The functor does not NUL terminate the buffer nor does a
     *  NUL character in the buffer terminate the input.
     *
     *  @param  bp  	points to the buffer.
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
     *  Returns a pointer to the data.
     *  @return a pointer to the data.
     */
    const void* getData() const;

    /**
     *  Returns the size of the data.
     *  @return the size of the data.
     */
    size_t getSize() const;

    /**
     *  Returns the current offset into the data, indicating how
     *  many characters have been consumed so far.
     *  @return an offset into the data.
     */
    size_t getOffset() const;

    /**
     * Returns the length of the available data.
     * @return the length of the available data.
     */
    size_t getLength() const;

    /**
     * Returns the number of pushed characters. For this implementation this
     * will always return zero or one.
     * @return the number of pushed characters.
     */
    size_t getPushed() const;

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
     *  This points to the beginning of the data.
     */
    const char* data;

    /**
     *  This is the size of the data.
     */
    size_t size;

    /**
     *  This is the offset into the data.
     */
    size_t offset;

    /**
     *  This is the character pushed back into the descriptor.
     */
    int saved;

};


//
//  Return a pointer to the data.
//
inline const void* DataInput::getData() const {
    return this->data;
}


//
//  Return the total unconsumed size of the data.
//
inline size_t DataInput::getSize() const {
    return this->size;
}


//
//  Return the offset into the data.
//
inline size_t DataInput::getOffset() const {
    return this->offset;
}

//
// Return the length of the data.
//
inline size_t DataInput::getLength() const {
	return (this->size - this->offset) + ((saved != EOF) ? 1 : 0);
}

//
// Return the number of pushed characters.
//
inline size_t DataInput::getPushed() const {
	return (saved != EOF) ? 1 : 0;
}

#include "com/diag/desperado/End.h"


#if defined(DESPERADO_HAS_UNITTESTS)
#include "com/diag/desperado/cxxcapi.h"
/**
 *  Run the DataInput unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestDataInput(void);
#endif


#endif
