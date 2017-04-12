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
 *  Implements the DataInput class.
 *
 *  @see    DataInput
 *
 *  @author Chip Overclock (coverclock@diag.com)
 */


#include "com/diag/desperado/errno.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/DataInput.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Constructor. The data is assumed to be NUL terminated.
//
DataInput::DataInput(const char* sp) :
    Input(),
    data(sp),
    size((0 != sp) ? std::strlen(sp) : 0),
    offset(0),
    saved(EOF)
{
}


//
//  Constructor. The data has a specified size.
//
DataInput::DataInput(const void* sp, size_t sz) :
    Input(),
    data(static_cast<const char*>(sp)),
    size((0 != sp) ? sz : 0),
    offset(0),
    saved(EOF)
{
}


//
//  Destructor.
//
DataInput::~DataInput() {
}


//
//  Return the next character in the data.
//
int DataInput::operator() () {
    int rc = EOF;
    if (0 == this->data) {
        errno = EINVAL;
    } else if (EOF != this->saved) {
        rc = this->saved;
        rc = intmaxof(uint8_t) & rc;
        this->saved = EOF;
    } else if (this->offset >= this->size) {
        errno = 0;
    } else {
        rc = this->data[this->offset++];
        rc = rc & unsignedintmaxof(char);
    }
    return rc;
}


//
//  Push a character back into the data up to the limit of the
//  beginning of the data.
//
int DataInput::operator() (int ch) {
    int rc = EOF;
    if (0 == this->data) {
        errno = EINVAL;
    } else if (0 == this->offset) {
        errno = 0;
    } else {
        rc = this->saved = ch;
    }
    return rc;
}


//
//  Read a line from the data.
//
ssize_t DataInput::operator() (char* bp, size_t length) {
    ssize_t rc = EOF;
    if (0 == this->data) {
        errno = EINVAL;
    } else if (0 == length) {
        rc = 0;
    } else if ((this->offset >= this->size) && (EOF == this->saved)) {
        errno = 0;
    } else if (1 == length) {
    	*bp = '\0';
    	rc = 1;
    } else {
        char ch = ' ';
        rc = 0;
    	if (EOF != this->saved) {
    		ch = this->saved;
            this->saved = EOF;
            bp[rc++] = ch;
            --length;
    	}
    	if (('\n' != ch) && (this->offset < this->size)) {
			while (1 < length) {
				ch = this->data[this->offset];
				++this->offset;
				bp[rc++] = ch;
				--length;
				if ('\n' == ch) {
					break;
				}
			}
    	}
        bp[rc++] = '\0';
    }
    return rc;
}


//
//  Read binary data from the data.
//
ssize_t DataInput::operator() (
    void* bp,
    size_t /* minimum */,
    size_t maximum
) {
    ssize_t rc = EOF;
    if (0 == this->data) {
        errno = EINVAL;
    } else if (this->offset >= this->size) {
        errno = 0;
    } else if (0 == maximum) {
        rc = 0;
    } else {
    	rc = 0;
    	char* sp = static_cast<char*>(bp);
    	if (EOF != this->saved) {
    		*(sp++) = this->saved;
    		this->saved = EOF;
    		--maximum;
    		++rc;
    	}
    	ssize_t effective = this->size - this->offset;
        if (effective > static_cast<ssize_t>(maximum)) {
            effective = maximum;
        }
        if (effective > 0) {
        	memcpy(sp, &(this->data[this->offset]), effective);
        	this->offset += effective;
        	rc += effective;
        }
    }
    return rc;
}


//
//  Show this object on the output object.
//
void DataInput::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    this->Input::show(level, display, indent + 1);
    printf("%s data=%p\n", sp, this->data);
    printf("%s size=%lu\n", sp, this->size);
    printf("%s offset=%lu\n", sp, this->offset);
    printf("%s saved=0x%08x%s\n",
        sp, this->saved,
        (EOF == this->saved) ? "=EOF" : "");
}


#include "com/diag/desperado/End.h"
