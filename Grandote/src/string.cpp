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
 *  Implements the Desperado string functions.
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/string.h"
#include "com/diag/desperado/cxxcapi.h"
#include "com/diag/desperado/target.h"


//
//  This emulates the non-POSIX Linux strnlen() function.
//
CXXCAPI size_t desperado_strnlen(const char* s, size_t maxlen) {
    size_t length = 0;
    while (('\0' != *(s++)) && (0 < (maxlen--))) {
        ++length;
    }
    return length;
}


//
//  This emulates the non-POSIX OpenBSD strlcpy() function.
//
CXXCAPI size_t desperado_strlcpy(char *dst, const char *src, size_t count) {
    char* dstptr = dst;
    const char* srcptr = src;
    size_t tocopy = count;
    if (tocopy && (--tocopy)) {
        do {
            if (!(*(dstptr++) = *(srcptr++))) {
                break;
            }
        } while (--tocopy);
    }
    if (0 == tocopy) {
        if (0 < count) {
            *dstptr = '\0';
        }
        while (*(srcptr++)) {
            ;
        }
    }
    return (srcptr - src - 1);
}


//
//  This emulates the non-POSIX OpenBSD strlcat() function.
//
CXXCAPI size_t desperado_strlcat(char *dst, const char *src, size_t count) {
    char* dstptr = dst;
    const char* srcptr = src;
    size_t tocopy = count;
    size_t dstlen;
    while (tocopy-- && *dstptr) {
        dstptr++;
    }
    dstlen = dstptr - dst;
    tocopy = count - dstlen;
    if (0 == tocopy) {
        return (dstlen + std::strlen(src));
    }
    while ('\0' != *srcptr) {
        if (1 != tocopy) {
            *dstptr++ = *srcptr;
            tocopy--;
        }
        ++srcptr;
    }
    *dstptr = '\0';
    return (dstlen + (srcptr - src));
}


//
//  Search from the left.
//
CXXCAPI char* desperado_strnchr(const char* s, size_t count, int c) {
    while (('\0' != *s) && (0 < (count--))) {
        if (c == *s) {
            return const_cast<char*>(s);
        }
        ++s;
    }
    return 0;
}


//
//  Search from the left.
//
CXXCAPI char* desperado_strnrchr(const char* s, size_t count, int c) {
    size_t maxlen = ::desperado_strnlen(s, count);
    if (0 < maxlen) {
        const char* e = s + maxlen - 1;
        while (e >= s) {
            if (c == *e) {
                return const_cast<char*>(e);
            }
            --e;
        }
    }
    return 0;
}
