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
 *  Implements the Encode class.
 *
 *  @see    Encode
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Encode.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Destructor.
//
Encode::~Encode() {
}


//
//  Collapse a string containing escape sequences.
//
size_t Encode::collapse(
    const char *from,
    char *to,
    size_t tsize
) const {
    const char* ff = from;
    char* tt = to;
    char byte;
    int ii;

    while (('\0' != *ff) && (sizeof("") < tsize)) {
        if ('+' == *ff) {
            ++ff;                       // Encoded space.
            *(tt++) = ' ';
            --tsize;
        } else if ('%' == *ff) {
            byte = 0;                   // Possible hexadecimal encoding.
            for (ii = 1; 3 > ii; ++ii) {
                if (('0' <= *(ff + ii)) && (*(ff + ii) <= '9')) {
                    byte <<= 4;
                    byte |= '\x00' + *(ff + ii) - '0';
                } else if (('a' <= *(ff + ii)) && (*(ff + ii) <= 'f')) {
                    byte <<= 4;
                    byte |= '\x0a' + *(ff + ii) - 'a';
                } else if (('A' <= *(ff + ii)) && (*(ff + ii) <= 'F')) {
                    byte <<= 4;
                    byte |= '\x0A' + *(ff + ii) - 'A';
                } else {
                    break;
                }
            }
            if (3 == ii) {
                ff += 3;                // Hexadecimal encoding.
                *(tt++) = byte;
            } else {
                *(tt++) = *(ff++);      // Invalid hexadecimal encoding.
            }
            --tsize;
        } else {
            *(tt++) = *(ff++);          // Unencoded character.
            --tsize;
        }
    }

    if (0 < tsize) {
        *(tt++) = '\0';
        --tsize;
    }

    return tt - to;
}

//
//  Expand binary data in a buffer into a string of escape sequences.
//
size_t Encode::expand(
    const char *from,
    size_t fsize,
    char *to,
    size_t tsize,
    const char* special
) const {
    Dump dump(0, true);
    const char* ff = from;
    char* tt = to;

    while ((0 < fsize) && (sizeof("") < tsize)) {
        if (
            ('\x1f' >= *ff) ||
            ('\x7f' <= *ff) ||
            (0 != std::strchr(special, *ff))
        ) {
            if (sizeof("%ff") > tsize) {
                break;
            }
            *(tt++) = '%';              // Encoded character.
            tt = dump.hexadecimals(*ff, tt);
            tsize -= 3;
        } else {
            *(tt++) = *ff;              // Non-escaped character.
            --tsize;
        }
        ++ff;
        --fsize;
    }

    if (0 < tsize) {
        *(tt++) = '\0';
        --tsize;
    }

    return tt - to;
}


//
//  Show this object on the output object.
//
void Encode::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


#include "com/diag/desperado/End.h"
