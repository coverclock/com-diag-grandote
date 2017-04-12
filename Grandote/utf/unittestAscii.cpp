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
 *  Implements the Ascii unit test.
 *
 *  @see    Ascii
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/Ascii.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Escape.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Desperado.h"

static Ascii ascii;
static Escape escape;

static int test(unsigned char cc) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    const Ascii::Entry* entry = &(ascii[cc]);
    size_t length;
    char to[2];

    printf(" 0x%02hhx %*s %*s %*s %*s\n",
        cc,
        sizeof(entry->name) - 1, entry->name,
        sizeof(entry->c) - 1, entry->c,
        sizeof(entry->octal) - 1, entry->octal,
        sizeof(entry->hexadecimal) - 1, entry->hexadecimal);

    length = escape.collapse(entry->octal, to, sizeof(to));
    if (sizeof(to) != length) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, sizeof(to), length);
        ++errors;
    }
    if (static_cast<unsigned char>(to[0]) != cc) {
        errorf("%s[%d]: (0%03o!=0%03o)!\n",
            __FILE__, __LINE__, to[0], cc);
        ++errors;
    }

    length = escape.collapse(entry->hexadecimal, to, sizeof(to));
    if (sizeof(to) != length) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, sizeof(to), length);
        ++errors;
    }
    if (static_cast<unsigned char>(to[0]) != cc) {
        errorf("%s[%d]: (0x%02x!=0x%02x)!\n",
            __FILE__, __LINE__, to[0], cc);
        ++errors;
    }

    return errors;
}

static Ascii staticAscii; // Yes, this is redundant.

CXXCAPI int unittestAscii(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s(%d): begin\n", __FILE__, __LINE__);

    ::staticAscii.show();

    ascii.show();

    Ascii ascii2 = ascii;
    ascii2.show();

    Ascii ascii3;
    ascii3 = ascii;
    ascii3.show();

    printf("%5s %*s %*s %*s %*s\n",
        "VALUE",
        membersizeof(Ascii::Entry, name) - 1, "NAME",
        membersizeof(Ascii::Entry, c) - 1, "CHAR",
        membersizeof(Ascii::Entry, octal) - 1, "OCT",
        membersizeof(Ascii::Entry, hexadecimal) - 1, "HEX");

    unsigned char cc = unsignedintminof(unsigned char);
    size_t count = 0;
    errors += test(cc++);
    ++count;
    while (0 != cc) {
        errors += test(cc++);
        ++count;
    }
    if (256U != count) {
        errorf("%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__, 256U, count);
        ++errors;
    }

    printf("%5s %*s %*s %*s %*s\n",
        "VALUE",
        membersizeof(Ascii::Entry, name) - 1, "NAME",
        membersizeof(Ascii::Entry, c) - 1, "CHAR",
        membersizeof(Ascii::Entry, octal) - 1, "OCT",
        membersizeof(Ascii::Entry, hexadecimal) - 1, "HEX");

    errors += test('A');
    errors += test('\n');
    errors += test(0200);
    errors += test(0xff);

    printf("%s(%d): end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
