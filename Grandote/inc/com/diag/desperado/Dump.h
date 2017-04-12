#ifndef _COM_DIAG_DESPERADO_DUMP_H_
#define _COM_DIAG_DESPERADO_DUMP_H_

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
 *  Declares the Dump class.
 *
 *  @see    Dump
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/target.h"


#if defined(__cplusplus)


#include "com/diag/desperado/Object.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Input.h"


#include "com/diag/desperado/Begin.h"

/**
 *  Displays the contents of a block of memory of an arbitary location,
 *  size, and a alignment, in a hexadecimal dump format, in either word or
 *  byte form.
 *
 *  If included in a C translation unit, defines a C-callable API.
 *
 *  @author coverclock@diag.com (Chip Overclock)
 */
class Dump : public Object {

public:

    /**
     *  This array contains the characters '0' through '9' and
     *  'A' through 'F', used for binary to printable hexadecimal
     *  conversions.
     */
    static const char upper_case_hex[];


    /**
     *  This array contains the characters '0' through '9' and
     *  'a' through 'f', used for binary to printable hexadecimal
     *  conversions.
     */
    static const char lower_case_hex[];

    /**
     *  Constructor.
     *
     *  @param  po          points to an output object on to which the
     *                      dump will be directed. If 0, the platform
     *                      output object will be used.
     *
     *  @param  uppercase   if true generates dumps using upper case
     *                      characters 'A'-'F', otherwise lower case
     *                      'a'-'f' will be used. A useful convention
     *                      when writing portable code is to pass the
     *                      value of !littleendian() for
     *                      this parameter.
     *
     *  @param  unuchar     is the character printed to indicate an
     *                      octet or word that is outside the requested
     *                      range.
     *
     *  @param  unpchar     is the character printed to indicate an
     *                      octet that is unprintable.
     */
    explicit Dump(
        Output* po = 0,
        bool uppercase = false,
        int unuchar = ' ',
        int unpchar = '.'
    );

    /**
     *  Constructor.
     *
     *  @param  ro         refers to an output object on to which the
     *                      dump will be directed.
     *
     *  @param  uppercase   if true generates dumps using upper case
     *                      characters 'A'-'F', otherwise lower case
     *                      'a'-'f' will be used. A useful convention
     *                      when writing portable code is to pass the
     *                      value of !littleendian() for
     *                      this parameter.
     *
     *  @param  unuchar     is the character printed to indicate an
     *                      octet or word that is outside the requested
     *                      range.
     *
     *  @param  unpchar     is the character printed to indicate an
     *                      octet that is unprintable.
     */
    explicit Dump(
        Output& ro,
        bool uppercase = false,
        int unuchar = ' ',
        int unpchar = '.'
    );

    /**
     *  Destructor.
     */
    virtual ~Dump();

    /**
     *  Returns a reference to the current dump output object.
     *
     *  @return a reference to the current output object.
     */
    virtual Output& output() const;

    /**
     *  Bytes are dumped in units of this type.
     */
    typedef unsigned char Byte;

    /**
     *  Dumps a block of bytes in hexadecimal format to an output functor.
     *  The block is dumped line by line, eight or less bytes per line.
     *  Both the hexadecimal value and the ASCII value (if printable)
     *  of each byte is displayed. Unprintable values are displayed as an
     *  unprintable encoding. The address displayed at the beginning of
     *  each line of output points to a byte on a double word boundary, although
     *  only the bytes requested by the caller are dereferenced and displayed.
     *  This simplifies for the user the calculation of addresses of bytes
     *  displayed within the line. This method is best used when the data are
     *  predominantly characters.
     *
     *  @see Dump::words()
     *
     *  @param  data        points to the block of bytes. NULL (zero) is
     *                      a valid pointer value.
     *
     *  @param  length      is the number of bytes in the block.
     *
     *  @param  virtualize  if true causes the dump to use the following
     *                      parameter for the address to display instead
     *                      of the actual data pointer.
     *
     *  @param  address     is the value to use as the display address.
     *
     *  @param  indent      specifies the level of indentation.
     *                      
     *  @return             the number of bytes dumped.
     */
    virtual size_t bytes(
        const void* data,
        size_t length,
        bool virtualize = false,
        uintptr_t address = 0,
        int indent = 0
    );

    /**
     *  Functor equivalent to calling bytes().
     *
     *  @param  data        points to the block of bytes. NULL (zero) is
     *                      a valid pointer value.
     *
     *  @param  length      is the number of bytes in the block.
     *
     *  @param  virtualize  if true causes the dump to use the following
     *                      parameter for the address to display instead
     *                      of the actual data pointer.
     *
     *  @param  address     is the value to use as the display address.
     *
     *  @param  indent      specifies the level of indentation.
     *
     *  @return             the number of bytes dumped.
     */
    size_t operator() (
        const void* data,
        size_t length,
        bool virtualize = false,
        uintptr_t address = 0,
        int indent = 0
    );

    /**
     *  Dumps all data in byte format from an input functor until end
     *  of file is reached.
     *
     *  @param  input       refers to the input functor.
     *                      
     *  @return             the number of bytes dumped.
     */
    virtual size_t bytes(Input& input);

    /**
     *  Words are dumped in units of this type.
     */
    typedef unsigned int Word;

    /**
     *  Dumps a block of words in hexadecimal format to an output functor.
     *  The block is dumped line by line, four or less words per line.
     *  Both the hexadecimal value and the ASCII values (if printable) of
     *  each word is displayed. Unprintable values are displayed as an
     *  unprintable encoding. The address displayed at the beginning of each
     *  line of output points to a word on a quadruple word boundary, although
     *  only the whole words requested by the caller are dereferenced and
     *  displayed. This simplifies for the user the calculation of addresses
     *  of words displayed within the line. This method is best used when
     *  the data are predominantly words. On little-endian systems, the
     *  ASCII decode will exhibit byte reversal, so that the ASCII
     *  characters are read high-order to low-order left-to-right in each
     *  word.
     *
     *  @see Dump::bytes()
     *
     *  @param  data        points to the block of words. NULL (zero) is
     *                      a valid pointer value.
     *
     *  @param  length      is the number of bytes (not words) in the block.
     *
     *  @param  virtualize  if true causes the dump to use the following
     *                      parameter for the address to display instead
     *                      of the actual data pointer.
     *
     *  @param  address     is the value to use as the display address.
     *
     *  @param  indent      specifies the level of indentation.
     *
     *  @return             the number of words dumped.
     */
    virtual size_t words(
        const void* data,
        size_t length,
        bool virtualize = false,
        uintptr_t address = 0,
        int indent = 0
    );

    /**
     *  Dumps all data in word format from an input functor until end
     *  of file is reached.
     *
     *  @param  input       refers to the input functor.
     *                      
     *  @return             the number of bytes dumped.
     */
    virtual size_t words(Input& input);

    /**
     *  Represents a sequence of bytes if a given length in printable
     *  hexadecimal, without the leading "0x" prefix, in a buffer.
     *
     *  @see    littleendian()
     *
     *  @param  data    points to the sequence of bytes to dump.
     *
     *  @param  length  is the number of bytes in the sequence.
     *
     *  @param  buffer  points to the buffer that is at least length
     *                  plus one (to accomodate the trailing NUL) bytes
     *                  long.
     *
     *  @param  reverse if true causes the output sequence to be
     *                  displayed in reverse order. By passing the
     *                  value of the littleendian()
     *                  function as this parameter when the input
     *                  data is an integral type, causes the resulting
     *                  hexadecimal string to be printed in the correct
     *                  order.
     *
     *  @return         a pointer to the buffer.
     */
    virtual char* string(
        const void* data,
        size_t length,
        char* buffer,
        bool reverse = false
    );

    /**
     *  Returns true if the input character is printable, false otherwise.
     *
     *  @param  ch      conains the input character.
     *
     *  @return true if the input character is printable, false otherwise.
     */
    bool printable(int ch) const;

    /**
     *  Represent the eight-bit value as two printable hexadecimal
     *  characters in the caller-provided buffer. Only the two
     *  hexadecimal characters are placed in the buffer with no
     *  terminating NUL character.
     *
     *  @param  value   is the eight-bit value.
     *
     *  @param  to      points to the caller provided buffer into
     *                  which the two printable characters are
     *                  placed.
     *
     *  @return         the updated value of the buffer pointer.
     */
    char* hexadecimals(const Byte value, char* to) const;

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
     *  Points to the output object to which the dump is directed.
     */
    Output* ou;

    /**
     *  Points to the hexadecimal table to use.
     */
    const char* table;

    /**
     *  This is the character to use to indicate unused data.
     */
    int unused;

    /**
     *  This is the character to use to indicate unprintable data.
     */
    int unprintable;

};

//
//	Implements a functor version of bytes().
//
inline size_t Dump::operator() (const void* data, size_t length, bool virtualize, uintptr_t address, int indent) {
	return this->bytes(data, length, virtualize, address, indent);
}

//
//  Returns true of the character ch is printable, false otherwise.
//
inline bool Dump::printable(int ch) const {
    return ((' ' <= ch) && (ch <= '~'));
}


//
//  Represent a single character into two printable hex characters.
//
inline char* Dump::hexadecimals(const Byte value, char* to) const {
    *(to++) = this->table[(value >> 4) & 0x0f];
    *(to++) = this->table[(value >> 0) & 0x0f];
    return to;
}

#include "com/diag/desperado/End.h"


#else


/**
 *  This is an opaque type used to refer to a C++ dump object
 *  in a C program.
 */
typedef struct Dump Dump;


#endif


/**
 *  Dumps a block of bytes in hexadecimal format.
 *
 *  @see Dump::bytes
 *
 *  @param  d       points to a dump object, for example the platform
 *                  dump object.
 *
 *  @param  data    points to the block of bytes. NULL (zero) is a valid
 *                  pointer value.
 *
 *  @param  length  is the number of bytes in the block.
 *
 *  @return         the number of bytes dumped.
 */
CXXCAPI size_t dump_bytes(CXXCTYPE(::com::diag::desperado::, Dump)* d, const void* data, size_t length);


/**
 *  Dumps a block of words in hexadecimal format.
 *
 *  @see Dump::words
 *
 *  @param  d       points to a dump object, for example the platform
 *                  dump object.
 *
 *  @param  data    points to the block of words. NULL (zero) is a valid
 *                  pointer value.
 *
 *  @param  length  is the number of bytes (not words) in the block.
 *
 *  @return         the number of words dumped.
 */
CXXCAPI size_t dump_words(CXXCTYPE(::com::diag::desperado::, Dump)* d, const void* data, size_t length);


#if defined(DESPERADO_HAS_UNITTESTS)
/**
 *  Run the Dump unit test.
 *  
 *  @return the number of errors detected by the unit test.
 */
CXXCAPI int unittestDump(void);
#endif

#endif
