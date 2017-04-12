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
 *  Implements the Dump unit test.
 *
 *  @see    Dump
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/stdio.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Output.h"
#include "com/diag/desperado/Ascii.h"
#include "com/diag/desperado/littleendian.h"
#include "com/diag/desperado/Desperado.h"

static const char text[] =
    "SONNET NINETY-NINE:\n"
    "Love is not all.\n"
    "It is not meat nor drink nor slumber nor roof against the rain.\n"
    "Nor yet a floating spar to men that sink and rise and sink "
    "and rise and sink again.\n"
    "Love cannot fill the thickened lung with breath, nor cleanse the blood, "
    "nor set the fractured bone.\n"
    "Yet many a man is making friends with death, even as I speak, for "
    "lack of love alone.\n"
    "It may well be, in a difficult hour, pinned down by pain and "
    "moaning for release, or nagged by want past resolutions power, I may "
    "be driven to sell your love for peace.\n"
    "Or trade the memory of this night for food.\n"
    "It may well be.\n"
    "I do not think I would.\n"
    "-- Edna St. Vincent Milay\n";

static const char escapes[] = "\1\02\003\0004\x5\x06\x007\x0008\x00009";

static Dump staticDump;

CXXCAPI int unittestDump(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticDump.show();

    printf("%s", text);

    Dump dump;
    dump.show();

    Dump dump2 = dump;
    dump2.show();

    Dump dump3;
    dump3 = dump;
    dump3.show();

    Dump dumpupper(0, true);
    dumpupper.show();

    Dump dumperr(Platform::instance().error());
    dumperr.show();

    unsigned char data[256];
    for (size_t ii = 0; sizeof(data) > ii; ++ii) {
        data[ii] = static_cast<unsigned char>(ii);
    }

    printf("%s[%d]: bytes decode\n", __FILE__, __LINE__);

    dump.bytes(data, sizeof(data));

    size_t length = 8 * 3;

    for (size_t ii = 0; length >= ii; ++ii) {
        printf("%s[%d]: bytes length %u\n",
            __FILE__, __LINE__, ii);
        dump.bytes(data, ii);
        printf("%s[%d]: bytes length %u virtualized\n",
            __FILE__, __LINE__, ii);
        dump.bytes(data, ii, true);
    }

    for (size_t ii = 0; length >= ii; ++ii) {
        printf("%s[%d]: bytes alignment %u\n",
            __FILE__, __LINE__, ii);
        dump.bytes(data + ii, length - ii);
        printf("%s[%d]: bytes alignment %u virtualized\n",
            __FILE__, __LINE__, ii);
        dump.bytes(data + ii, length - ii, true, ii);
    }

    printf("%s[%d]: bytes text output\n", __FILE__, __LINE__);

    dump.bytes(text, sizeof(text));

    printf("%s[%d]: bytes text output virtualized\n", __FILE__, __LINE__);

    dump.bytes(text, sizeof(text), true);

    errorf("%s[%d]: bytes text error\n", __FILE__, __LINE__);

    dumperr.bytes(text, sizeof(text));

    printf("%s[%d]: words decode\n", __FILE__, __LINE__);

    dump.words(data, sizeof(data));

    length = 16 * 3;

    for (size_t ii = 0; length >= ii; ++ii) {
        printf("%s[%d]: words length %u\n",
            __FILE__, __LINE__, ii);
        dump.words(data, ii);
        printf("%s[%d]: words length %u virtualized\n",
            __FILE__, __LINE__, ii, true);
        dump.words(data, ii, true);
    }

    for (size_t ii = 0; length >= ii; ++ii) {
        printf("%s[%d]: words alignment %u\n",
            __FILE__, __LINE__, ii);
        dump.words(data + ii, length - ii);
        printf("%s[%d]: words alignment %u virtualized\n",
            __FILE__, __LINE__, ii);
        dump.words(data + ii, length - ii, true, ii);
    }

    printf("%s[%d]: words text output\n", __FILE__, __LINE__);

    dump.words(text, sizeof(text));

    printf("%s[%d]: words text output virtualized\n", __FILE__, __LINE__);

    dump.words(text, sizeof(text));

    errorf("%s[%d]: words text error\n", __FILE__, __LINE__);

    dumperr.words(text, sizeof(text));

    printf("%s[%d]: bytes dumping words\n", __FILE__, __LINE__);

    unsigned long words[] = {
        ('A' << 24) | ('B' << 16) | ('C' <<  8) | ('D' <<  0),
        ('E' << 24) | ('F' << 16) | ('G' <<  8) | ('H' <<  0),
        ('I' << 24) | ('J' << 16) | ('K' <<  8) | ('L' <<  0),
        ('M' << 24) | ('N' << 16) | ('O' <<  8) | ('P' <<  0)
    };

    dump.bytes(words, sizeof(words));

    printf("%s[%d]: words dumping words\n", __FILE__, __LINE__);

    dump.words(words, sizeof(words));

    printf("%s[%d]: bytes dumping bytes\n", __FILE__, __LINE__);

    char bytes[] = {
        'A', 'B', 'C', 'D',
        'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L',
        'M', 'N', 'O', 'P'
    };

    dump.bytes(bytes, sizeof(bytes));

    printf("%s[%d]: words dumping bytes\n", __FILE__, __LINE__);

    dump.words(bytes, sizeof(bytes));

    printf("%s[%d]: bytes escapes output\n",
        __FILE__, __LINE__);

    dump.bytes(escapes, sizeof(escapes));

    printf("%s[%d]: get and set\n", __FILE__, __LINE__);

    dump.show();
    Output& standard = dump.output();
    Output& output = Platform::instance().output();
    if (&standard != &output) {
        errorf("%s[%d]: (%p!=%p)!\n",
            __FILE__, __LINE__, &standard, &output);
        ++errors;
    }

    Print outputf(output);
    Print standardf(standard);

    outputf("%s[%d]: output=%p\n", __FILE__, __LINE__, &output);
    standardf("%s[%d]: standard=%p\n", __FILE__, __LINE__, &standard);

    dump.bytes(bytes, sizeof(bytes));
    dumpupper.bytes(bytes, sizeof(bytes));
    dump.words(words, sizeof(words));
    dumpupper.words(words, sizeof(words));

    printf("%s[%d]: printable\n", __FILE__, __LINE__);

    Ascii ascii;
    Ascii::Entry entry;
    unsigned char ch = 0;
    bool isnamed;
    bool isprintable;
    do {
        entry = ascii[ch];
        isnamed = (1 < std::strlen(entry.name));
        isprintable = dump.printable(ch);
        if ((isnamed == isprintable) && (ch != ' ')) {
            errorf("%s[%d]: 0x%hhx %s (%d==%d)!\n",
                __FILE__, __LINE__, ch, "", isnamed, isprintable);
            ++errors;
        }
        ++ch;
    } while (ch != 0);

    printf("%s[%d]: hexadecimals\n", __FILE__, __LINE__);

    char printbuffer[sizeof("0xc0edbabe")];
    char dumpbuffer[sizeof("0xc0edbabe")];
    char dumpbufferlittle[sizeof("0xc0edbabe")];
    char dumpbufferbig[sizeof("0xc0edbabe")];

    dumpbuffer[2] = '\0';
    ch = 0;
    do {
        ::snprintf(printbuffer, sizeof(printbuffer), "%02x", 
            static_cast<unsigned char>(ch));
        dump.hexadecimals(ch, dumpbuffer);
        if (0 != std::strcmp(printbuffer, dumpbuffer)) {
            errorf("%s[%d]: 0x%hhx (\"%s\"!=\"%s\")!\n",
                __FILE__, __LINE__, ch, printbuffer, dumpbuffer);
            ++errors;
        }
        ::snprintf(printbuffer, sizeof(printbuffer), "%02X", 
            static_cast<unsigned char>(ch));
        dumpupper.hexadecimals(ch, dumpbuffer);
        if (0 != std::strcmp(printbuffer, dumpbuffer)) {
            errorf("%s[%d]: 0x%hhx (\"%s\"!=\"%s\")!\n",
                __FILE__, __LINE__, ch, printbuffer, dumpbuffer);
            ++errors;
        }
        ++ch;
    } while (ch != 0);

    printf("%s[%d]: string\n", __FILE__, __LINE__);

    uint32_t datum = 0xc0edbabe;

    ::snprintf(printbuffer, sizeof(printbuffer), "%08x", datum);
    dump.string(&datum, sizeof(datum), dumpbuffer, littleendian());
    if (0 != std::strcmp(printbuffer, dumpbuffer)) {
        errorf("%s[%d]: 0x%x (\"%s\"!=\"%s\")!\n",
            __FILE__, __LINE__, datum, printbuffer, dumpbuffer);
        ++errors;
    }
    dump.string(&datum, sizeof(datum), dumpbufferlittle, true);
    dump.string(&datum, sizeof(datum), dumpbufferbig, false);
    printf("%s[%d]: lower print=\"%s\" dump=\"%s\" little=\"%s\" big=\"%s\"\n",
        __FILE__, __LINE__,
        printbuffer, dumpbuffer, dumpbufferlittle, dumpbufferbig);

    ::snprintf(printbuffer, sizeof(printbuffer), "%08X", datum);
    dumpupper.string(&datum, sizeof(datum), dumpbuffer, littleendian());
    if (0 != std::strcmp(printbuffer, dumpbuffer)) {
        errorf("%s[%d]: 0x%x (\"%s\"!=\"%s\")!\n",
            __FILE__, __LINE__, datum, printbuffer, dumpbuffer);
        ++errors;
    }
    dumpupper.string(&datum, sizeof(datum), dumpbufferlittle, true);
    dumpupper.string(&datum, sizeof(datum), dumpbufferbig, false);
    printf("%s[%d]: upper print=\"%s\" dump=\"%s\" little=\"%s\" big=\"%s\"\n",
        __FILE__, __LINE__,
        printbuffer, dumpbuffer, dumpbufferlittle, dumpbufferbig);

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
