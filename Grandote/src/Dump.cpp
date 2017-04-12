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
 *  Implements the Dump class.
 *
 *  @see    Dump
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/target.h"
#include "com/diag/desperado/littleendian.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/generics.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Constant.h"


#include "com/diag/desperado/Begin.h"


const char Dump::upper_case_hex[] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
};


const char Dump::lower_case_hex[] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
};


//
//  Constructor.
//
Dump::Dump(Output* po, bool uppercase, int unuchar, int unpchar) :
    ou(po),
    table(uppercase ? this->upper_case_hex : this->lower_case_hex),
    unused(unuchar),
    unprintable(unpchar)
{
}


//
//  Constructor.
//
Dump::Dump(Output& ro, bool uppercase, int unuchar, int unpchar) :
    ou(&ro),
    table(uppercase ? this->upper_case_hex : this->lower_case_hex),
    unused(unuchar),
    unprintable(unpchar)
{
}


//
//  Destructor.
//
Dump::~Dump() {
}


//
//  Returns current output object.
//
Output& Dump::output() const {
    return this->ou ? *this->ou : Platform::instance().output();
}


//
//  Dump bytes in hex format to output functor.
//
size_t Dump::bytes(
    const void* data,
    size_t length,
    bool virtualize,
    uintptr_t address,
    int indent
) {
    if (0 == length) {
        return 0;
    }

    static const size_t bytespergroup = 8;

    const Byte* first = static_cast<const Byte*>(data);
    const Byte* last = first + length - 1;
    const Byte* pointer =
        static_cast<const Byte*>(aligndownby(data, bytespergroup));
    const Byte* here;
    int ch;


    if (virtualize) {
        address = static_cast<uintptr_t>(sizedownby(address, bytespergroup));
    } else {
        address = reinterpret_cast<uintptr_t>(pointer);
    }

    size_t count = (length + bytespergroup - 1) / bytespergroup;

    Output& out = this->output();
    Print printf(out);
    const char* sp = out.indentation(indent);
    char buffer[(sizeof(Byte) * 2) + 1];
    buffer[sizeof(buffer) - 1] = '\0';

    while (0 < count) {

        printf("%s0x%08x: ", sp, address);

        here = pointer;
        for (size_t ii = 0; bytespergroup > ii; ++ii) {
            if ((first <= here) && (here <= last)) {
                out("0x");
                this->hexadecimals(*here, buffer);
            } else {
                out(this->unused);
                out(this->unused);
                std::memset(buffer, this->unused, sizeof(buffer) - 1);
            }
            out(buffer);
            out(' ');
            ++here;
        }

        here = pointer;
        out('|');
        for (size_t ii = 0; bytespergroup > ii; ++ii) {
            if ((first <= here) && (here <= last)) {
                ch = *here;
                if (!this->printable(ch)) {
                    ch = this->unprintable;
                }
                out(ch);
            } else {
                out(this->unused);
            }
            ++here;
        }
        out("|\n");

        address += bytespergroup;
        pointer += bytespergroup;
        --count;
    }

    return length;
}


//
//  Dump words in hex format to output functor.
//
size_t Dump::words(
    const void* data,
    size_t length,
    bool virtualize,
    uintptr_t address,
    int indent
) {
    size_t wordcount = length / sizeof(Word);

    if (0 == wordcount) {
        return 0;
    }

    static const size_t bytespergroup = 16;
    static const size_t wordspergroup = bytespergroup / sizeof(Word);

    const Word* first =
        static_cast<const Word*>(alignupby(data, sizeof(Word)));
    const Word* last = first + wordcount - 1;
    const Word* pointer =
        static_cast<const Word*>(aligndownby(data, bytespergroup));
    const Word* here;
    int ch;

    if (virtualize) {
        address = static_cast<uintptr_t>(sizedownby(address, bytespergroup));
    } else {
        address = reinterpret_cast<uintptr_t>(pointer);
    }

    size_t count = (length + bytespergroup - 1) / bytespergroup;

    Output& out = this->output();
    Print printf(out);
    const char* sp = out.indentation(indent);
    char buffer[(sizeof(Word) * 2) + 1];
    buffer[sizeof(buffer) - 1] = '\0';
    bool endian = littleendian();

    while (0 < count) {

        printf("%s0x%08x: ", sp, address);

        here = pointer;
        for (size_t ii = 0; wordspergroup > ii; ++ii) {
            if ((first <= here) && (here <= last)) {
                out("0x");
                this->string(here, sizeof(*here), buffer, endian);
            } else {
                out(this->unused);
                out(this->unused);
                std::memset(buffer, this->unused, sizeof(buffer) - 1);
            }
            out(buffer);
            out(' ');
            ++here;
        }

        here = pointer;
        out('|');
        for (size_t ii = 0; wordspergroup > ii; ++ii) {
            if ((first <= here) && (here <= last)) {
                for (size_t jj = 0; sizeof(Word) > jj; ++jj) {
                    ch = ((*here) >> (((sizeof(Word) - 1) - jj) *
                        Constant::bits_per_octet)) & 0xff;
                    if (!this->printable(ch)) {
                        ch = this->unprintable;
                    }
                    out(ch);
                }
            } else {
                for (size_t jj = 0; sizeof(Word) > jj; ++jj) {
                    out(this->unused);
                }
            }
            ++here;
        }
        out("|\n");

        address += bytespergroup;
        pointer += wordspergroup;
        --count;
    }

    return wordcount;
}


//
//  Dump bytes from an input functor.
//
size_t Dump::bytes(Input& input) {
    uintptr_t total = 0;
    char buffer[256];
    ssize_t octets;
    while (true) {
        octets = input(buffer, sizeof(buffer), sizeof(buffer));
        if (EOF == octets) {
            break;
        }
        this->bytes(buffer, octets, true, total);
        total += octets;
    }

    return total;
}


//
//  Dump words from an input functor.
//
size_t Dump::words(Input& input) {
    uintptr_t total = 0;
    char buffer[256];
    ssize_t octets;
    while (true) {
        octets = input(buffer, sizeof(buffer), sizeof(buffer));
        if (EOF == octets) {
            break;
        }
        this->words(buffer, octets, true, total);
        total += octets;
    }

    return total;
}


//
//  Convert an input sequence to printable hexadecimal.
//
char* Dump::string(
    const void* data,
    size_t length,
    char* buffer,
    bool reverse
) {
    const char* dd = static_cast<const char*>(data);
    if (reverse) {
        char* bb = buffer + (length * 2);
        *bb = '\0';
        bb -= 2;
        while (0 < (length--)) {
            hexadecimals(*(dd++), bb);
            bb -= 2;
        }
    } else {
        char* bb = buffer;
        while (0 < (length--)) {
            hexadecimals(*(dd++), bb);
            bb += 2;
        }
        *bb = '\0';
    }
    return buffer;
}


//
//  Show this object on the output object.
//
void Dump::show(int level, Output* display, int indent) const {
    Platform& pl = Platform::instance();
    Print printf(display);
    const char* sp = printf.output().indentation(indent);
    char component[sizeof(__FILE__)];
    printf("%s%s(%p)[%lu]:\n",
        sp, pl.component(__FILE__, component, sizeof(component)),
        this, sizeof(*this));
    printf("%s sizeof(Byte)=%lu\n", sp, sizeof(Byte));
    printf("%s sizeof(Word)=%lu\n", sp, sizeof(Word));
    printf("%s littleendian()=%d\n", sp, littleendian());
    printf("%s ou=%p\n", sp, this->ou);
    if (this->ou) {
        this->ou->show(level, display, indent + 2);
    }
    printf("%s table=%p=%s\n",
        sp, this->table,
        (this->table == this->upper_case_hex) ? "UPPER" :
            (this->table == this->lower_case_hex) ? "lower" :
                "Other");
    printf("%s unused=0x%02hhx\n", sp, this->unused);
    printf("%s unprintable=0x%02hhx\n", sp, this->unprintable);
}


#include "com/diag/desperado/End.h"
