/* vim: set ts=4 expandtab shiftwidth=4: */

/******************************************************************************

    Copyright 2005-2011 Digital Aggregates Corporation, Colorado, USA.
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
 *  Implements the BufferOutput class.
 *
 *  @see    BufferOutput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/BufferOutput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"

//
//  Constructor. The buffer has a specified size.
//
BufferOutput::BufferOutput(void* sp, size_t sz) :
    Output(),
    buffer(static_cast<char*>(sp)),
    size((0 != sp) ? sz : 0),
    offset(0)
{
}


//
//  Destructor.
//
BufferOutput::~BufferOutput() {
}


//
//  Output a character.
//
int BufferOutput::operator() (int c) {
    int rc = EOF;
    if (0 == this->buffer) {
        errno = EINVAL;
    } else if (this->offset >= this->size) {
        errno = 0;
    } else {
        this->buffer[this->offset++] = rc = c;
    }
    return rc;
}


//
//  Format and output a variable length argument list.
//
ssize_t BufferOutput::operator() (const char* format, va_list ap) {
    ssize_t rc = EOF;
    if (0 == this->buffer) {
        errno = EINVAL;
    } else if (this->offset >= this->size) {
        errno = 0;
    } else {
        char buffer[this->minimum_buffer_size + 1];
        ssize_t fc = ::vsnprintf(buffer, sizeof(buffer), format, ap);
        if (0 < fc) {
            rc = (*this)(buffer, fc);
        } else if (0 == fc) {
            rc = fc;
        } else if (0 == errno) {
            errno = EIO;
        }
    }
    return rc;
}


//
//  Output a buffer of no more than the specified size.
//
ssize_t BufferOutput::operator() (const char* s, size_t size) {
    ssize_t rc = EOF;
    if (0 == this->buffer) {
        errno = EINVAL;
    } else if (this->offset >= this->size) {
        errno = 0;
    } else {
        size_t want = ::strnlen(s, size);
        size_t have = this->size - this->offset;
        if (have < want) {
            want = have;
        }
        if (0 < want) {
            std::strncpy(&(this->buffer[this->offset]), s, want);
            this->offset += want;
        }
        rc = want;
    }
    return rc;
}


//
//  Write binary data to the buffer from a buffer.
//
ssize_t BufferOutput::operator() (
    const void* bp,
    size_t /* minimum */,
    size_t maximum
) {
    ssize_t rc = EOF;
    if (0 == this->buffer) {
        errno = EINVAL;
    } else if (this->offset >= this->size) {
        errno = 0;
    } else if (0 == maximum) {
        rc = 0;
    } else {
        rc = this->size - this->offset;
        if (rc > static_cast<ssize_t>(maximum)) {
            rc = maximum;
        }
        memcpy(&(this->buffer[this->offset]), bp, static_cast<size_t>(rc));
        this->offset += rc;
    }
    return rc;
}


//
//  Flush buffered data.
//
int BufferOutput::operator() () {
    return 0;
}


//
//  Show this object on the output object.
//
void BufferOutput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Output::show(level, display, indent + 1);
    printf("%s buffer=%p\n", sp, this->buffer);
    printf("%s size=%lu\n", sp, this->size);
    printf("%s offset=%lu\n", sp, this->offset);
}


#include "com/diag/desperado/End.h"
