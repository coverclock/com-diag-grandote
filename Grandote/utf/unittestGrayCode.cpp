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
 *  Implements the GrayCode unit test.
 *
 *  @see    GrayCode
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/GrayCode.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

typedef unsigned short Code;

template class GrayCode<int>;
template class GrayCode<Code>;

static GrayCode<int> staticGrayCode;

CXXCAPI int unittestGrayCode(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticGrayCode.show();

    GrayCode<Code> gray;
    gray.show();

    printf("%s[%d]: generation\n", __FILE__, __LINE__);

    Code to[(static_cast<size_t>(1)) << widthof(Code)];
    Code from[countof(to)];
    Code bb;
    Code gg;

    bb = 0;
    do {
        to[bb] = gray.encode(bb);
        from[to[bb]] = gray.decode(to[bb]);
    } while (0 != (++bb));

    printf("%s[%d]: transitivity\n", __FILE__, __LINE__);

    bb = 0;
    do {
        if (from[to[bb]] != bb) {
            errorf(
                "%s[%d]: (from[to[0x%02x]]==0x%02x)!\n",
                __FILE__, __LINE__,
                bb, from[to[bb]]);
            ++errors;
        }
        Code gg = bb;
        if (to[from[gg]] != gg) {
            errorf(
                "%s[%d]: (to[from[0x%02x]]==0x%02x)!\n",
                __FILE__, __LINE__, gg, to[from[gg]]);
            ++errors;
        }
    } while (0 != (++bb));

    printf("%s[%d]: uniqueness\n", __FILE__, __LINE__);

    bb = 0;
    do {
        gg = 0;
        do {
            if (bb != gg) {
                if (to[bb] == to[gg]) {
                    errorf(
                        "%s[%d]: (to[0x%02x]==to[0x%02x]==0x%02x)!\n",
                        __FILE__, __LINE__,
                        bb, gg, to[bb]);
                    ++errors;
                }
                if (from[bb] == from[gg]) {
                    errorf(
                        "%s[%d]: (from[0x%02x]==from[0x%02x]==0x%02x)!\n",
                        __FILE__, __LINE__,
                        bb, gg, from[bb]);
                    ++errors;
                }
            }
        } while (0 != (++gg));
    } while (0 != (++bb));

    printf("%s[%d]: validity\n", __FILE__, __LINE__);

    Code b2;
    Code gx;
    int n;
    size_t ii;

    bb = 0;
    do {
        b2 = (bb + 1) % countof(to);
        gx = to[bb] ^ to[b2];
        n = 0;
        for (ii = 0; widthof(Code) > ii; ++ii) {
            if (gx & 0x1) {
                n++;
            }
            gx >>= 1;
        }
        if (1 != n) {
            errorf(
                "%s[%d]: (to[0x%02x]==0x%02x to[0x%02x]==0x%02x %d)!\n",
                __FILE__, __LINE__,
                bb, to[bb], b2, to[b2], n);
            ++errors;
        }
        if (unsignedintmaxof(Code) == bb) {
            break;
        }
    } while (0 != (++bb));

    printf("%s[%d]: range\n", __FILE__, __LINE__);

    Code cc;
    unsigned long long sumb = 0;
    unsigned long long sumg = 0;

    bb = 0;
    do {
        gg = gray.encode(bb);
        cc = gray.decode(gg);
        if (bb != cc) {
            errorf(
                "%s[%d]: %u (%u!=%u)!\n",
                __FILE__, __LINE__,
                gg, bb, cc);
                ++errors;
        }
        sumb += bb;
        sumg += gg;
    } while (0 != (++bb));

    if (sumb != sumg) {
        errorf(
            "%s[%d]: (%u!=%u)!\n",
            __FILE__, __LINE__,
            sumb, sumg);
        ++errors;
    }

    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
