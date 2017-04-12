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
 *  Implements the Escape class.
 *
 *  @see    Escape
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Escape.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Platform.h"


#include "com/diag/desperado/Begin.h"


//
//  Destructor.
//
Escape::~Escape() {
}


//
//  Collapse a string containing escape sequences.
//
size_t Escape::collapse(const char *from, char *to, size_t tsize) {
    const char* ff = from;
    char* tt = to;
    char byte;
    int ii;

    while (('\0' != *ff) && (sizeof("") < tsize)) {
        if ('\\' == *ff) {
            switch (*(ff + 1)) {
            case '\0':                  // Terminating NUL.
                ++ff;
                break;
            case '\n':                  // Escaped newline; skip over.
                ff += 2;
                break;
            case '\\':                  // Escaped backslash.
                ff += 2;
                *(tt++) = '\\';
                --tsize;
                break;
            case 'a':                   // Alarm a.k.a. BEL.
                ff += 2;
                *(tt++) = '\a';
                --tsize;
                break;
            case 'b':                   // Backspace a.k.a. BS.
                ff += 2;
                *(tt++) = '\b';
                --tsize;
                break;
            case 'f':                   // Formfeed a.k.a. FF.
                ff += 2;
                *(tt++) = '\f';
                --tsize;
                break;
            case 'n':                   // Newline a.k.a. NL or LF.
                ff += 2;
                *(tt++) = '\n';
                --tsize;
                break;
            case 'r':                   // Return a.k.a. CR.
                ff += 2;
                *(tt++) = '\r';
                --tsize;
                break;
            case 't':                   // Horizontal tab a.k.a. HT.
                ff += 2;
                *(tt++) = '\t';
                --tsize;
                break;
            case 'v':                   // Vertical tab a.k.a. VT.
                ff += 2;
                *(tt++) = '\v';
                --tsize;
                break;
            case 'x':                   // Possible hexadecimal sequence.
                byte = 0;
                // ANSI hexadecimal sequences must have at least 1 digit.
                for (ii = 2; ; ++ii) {
                    if (('0' <= *(ff + ii)) && (*(ff + ii) <= '9')) {
                        byte <<= 4;
                        byte |= '\x0' + *(ff + ii) - '0';
                    } else if (('a' <= *(ff + ii)) && (*(ff + ii) <= 'f')) {
                        byte <<= 4;
                        byte |= '\xa' + *(ff + ii) - 'a';
                    } else if (('A' <= *(ff + ii)) && (*(ff + ii) <= 'F')) {
                        byte <<= 4;
                        byte |= '\xA' + *(ff + ii) - 'A';
                    } else {
                        break;
                    }
                }
                if (2 < ii) {
                    ff += ii;           // Hexadecimal sequence.
                    *(tt++) = byte;
                } else {
                    *(tt++) = *(ff++);  // Invalid hexadecimal sequence.
                }
                --tsize;
                break;
            case '0':                   // Octal sequence.
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                byte = 0;
                // ANSI octal sequences may be 1, 2 or 3 octal digits long.
                for (ii = 1; 4 > ii; ++ii) {
                    if (('0' <= *(ff + ii)) && (*(ff + ii) <= '7')) {
                        byte <<= 3;
                        byte |= '\0' + *(ff + ii) - '0';
                    } else {
                        break;
                    }
                }
                ff += ii;           // Octal sequence.
                *(tt++) = byte;
                --tsize;
                break;
            default:
                *(tt++) = *(ff++);      // Unknown escape sequence.
                --tsize;
                break;
            }
        } else {
            *(tt++) = *(ff++);          // Non-escape sequence.
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
size_t Escape::expand(
    const char *from,
    size_t fsize,
    char *to,
    size_t tsize,
    const char* special
) {
    Dump dump;
    const char* ff = from;
    char* tt = to;
    char buffer[sizeof("\\xff")];
    char* bb;
    size_t bsize;
    size_t ii;

    while ((0 < fsize) && (1 < tsize)) {
        bb = buffer;
        if ('\0' == *ff) {
            *(bb++) = '\\';             // Escaped zero a.k.a. NUL.
            *(bb++) = '0';
        } else if (0 != std::strchr(special, *ff)) {
            *(bb++) = '\\';             // Escaped character.
            *(bb++) = *ff;
        } else if ((' ' <= *ff) && (*ff <= '~')) {
            *(bb++) = *ff;              // Non-escaped character.
        } else if ('\a' == *ff) {
            *(bb++) = '\\';             // Alarm a.k.a. BEL.
            *(bb++) = 'a';
        } else if ('\b' == *ff) {
            *(bb++) = '\\';             // Backspace a.k.a. BS.
            *(bb++) = 'b';
        } else if ('\f' == *ff) {
            *(bb++) = '\\';             // Formfeed a.k.a. FF.
            *(bb++) = 'f';
        } else if ('\n' == *ff) {
            *(bb++) = '\\';             // Newline a.k.a. NL or LF.
            *(bb++) = 'n';
        } else if ('\r' == *ff) {
            *(bb++) = '\\';             // Return a.k.a. CR.
            *(bb++) = 'r';
        } else if ('\t' == *ff) {
            *(bb++) = '\\';             // Horizontal tab a.k.a. HT.
            *(bb++) = 't';
        } else if ('\v' == *ff) {
            *(bb++) = '\\';             // Vertical tab a.k.a. VT.
            *(bb++) = 'v';
        } else {
            *(bb++) = '\\';             // Hexadecimal sequence.
            *(bb++) = 'x';
            bb = dump.hexadecimals(*ff, bb);
        }
        ++ff;
        --fsize;
        bsize = bb - buffer;
        if (bsize >= tsize) {
            break;
        }
        for (ii = 0; ii < bsize; ++ii) {
            *(tt++) = *(buffer + ii);
        }
        tsize -= bsize;
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
void Escape::show(int /* level */, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
}


#include "com/diag/desperado/End.h"
