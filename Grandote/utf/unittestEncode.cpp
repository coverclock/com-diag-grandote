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
 *  Implements the Encode unit test.
 *
 *  @see    Encode
 *
 *  @author Chip Overclock (coverclock@diag.com)
 *
 *
 */


#include <ctype.h>
#include "com/diag/desperado/UnitTest.h"
#include "com/diag/desperado/target.h"
#include "com/diag/desperado/string.h"
#include "com/diag/desperado/Encode.h"
#include "com/diag/desperado/Encode.h"
#include "com/diag/desperado/Dump.h"
#include "com/diag/desperado/Print.h"
#include "com/diag/desperado/Platform.h"
#include "com/diag/desperado/Desperado.h"

static Encode staticEncode;

CXXCAPI int unittestEncode(void) {
    Print printf(Platform::instance().output());
    Print errorf(Platform::instance().error());
    int errors = 0;

    char raw[256];
    for (size_t ii = 0; sizeof(raw) > ii; ++ii) {
        raw[ii] = ii;
    }

    char expanded[(sizeof(raw)*3) + 1];
    char collapsed[sizeof(expanded)];

    printf("%s[%d]: begin\n", __FILE__, __LINE__);

    ::staticEncode.show();

    Encode encode;
    encode.show();

    Encode encode2 = encode;
    encode2.show();

    Encode encode3;
    encode3 = encode;
    encode3.show();

    Dump dump;
    dump.bytes(raw, sizeof(raw));

    printf("%s[%d]: expand\n", __FILE__, __LINE__);

    size_t ne = encode.expand(raw, sizeof(raw), expanded, sizeof(expanded));
    dump.bytes(expanded, ne);
    size_t me = ne;

    if (sizeof(expanded) < ne) {
        errorf("%s[%d]: (%u<%u))!\n",
            __FILE__, __LINE__, sizeof(expanded), ne);
        ++errors;
    }

    for (int ii = 0; '\0' != expanded[ii]; ++ii) {
        if (!isprint(expanded[ii])) {
            errorf("%s[%d]: (0x%hhx[%d])!\n",
                __FILE__, __LINE__, expanded[ii], ii);
            ++errors;
        }
    }

    printf("%s[%d]: collapse\n", __FILE__, __LINE__);

    size_t nc = encode.collapse(expanded, collapsed, sizeof(collapsed));
    dump.bytes(collapsed, nc);
    size_t mc = nc;

    if (sizeof(collapsed) < nc) {
        errorf("%s[%d]: (%lu<%lu)!\n",
            __FILE__, __LINE__, sizeof(collapsed), nc);
        ++errors;
    }

    if ((sizeof(raw) + 1) != nc) {
        errorf("%s[%d]: (%lu!=%lu)!\n",
            __FILE__, __LINE__, sizeof(raw) + 1, nc);
        ++errors;
    }

    size_t limit = (nc < sizeof(raw)) ? nc : sizeof(raw);
    for (size_t ii = 0; limit > ii; ++ii) {
        if (raw[ii] != collapsed[ii]) {
            errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                __FILE__, __LINE__, raw[ii], expanded[ii], ii);
            ++errors;
        }
    }

    printf("%s[%d]: expand lengths\n", __FILE__, __LINE__);

    bool bad;
    size_t maximum;

    for (size_t limit = 0; sizeof(expanded) > limit; ++limit) {
        bad = false;
        maximum = me < limit ? me : limit;
        expanded[limit] = '!';
        expanded[0] = '?';
        ne = encode.expand(raw, sizeof(raw), expanded, limit);
        if (!((maximum <= (ne + 4)) && ((ne + 4) <= (maximum + 4)))) {
            errorf("%s[%d]: (%lu?%lu)!\n",
                __FILE__, __LINE__, ne, limit);
            bad = true;
            ++errors;
        }
        if (0 == limit) {
            if ('?' != expanded[0]) {
                errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                    __FILE__, __LINE__, '?', expanded[0], limit);
                bad = true;
                ++errors;
            }
        } else {
            if ('!' != expanded[limit]) {
                errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                    __FILE__, __LINE__, '!', expanded[limit], limit);
                bad = true;
                ++errors;
            }
            if ('\0' != expanded[ne - 1]) {
                errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                    __FILE__, __LINE__, '\0', expanded[limit - 1], limit);
                bad = true;
                ++errors;
            }
            nc = encode.collapse(expanded, collapsed, nc);
            if (0 != std::memcmp(raw, collapsed, nc)) {
                errorf("%s[%d]: (@%p!=@%p[%lu])!\n",
                    __FILE__, __LINE__,
                    raw, collapsed, limit);
                bad = true;
                ++errors;
            }
        }
        if (bad) {
            dump.bytes(raw, sizeof(raw));
            dump.bytes(collapsed, nc);
            dump.bytes(expanded, ne);
        }
    }

    printf("%s[%d]: collapse lengths\n", __FILE__, __LINE__);

    for (size_t limit = 0; sizeof(collapsed) > limit; ++limit) {
        bad = false;
        maximum = mc < limit ? mc : limit;
        collapsed[limit] = '!';
        collapsed[0] = '?';
        nc = encode.collapse(expanded, collapsed, limit);
        if (!((maximum <= (nc + 4)) && ((nc + 4) <= (maximum + 4)))) {
            errorf("%s[%d]: (%lu?%lu)!\n",
                __FILE__, __LINE__, nc, limit);
            bad = true;
            ++errors;
        }
        if (0 == limit) {
            if ('?' != collapsed[0]) {
                errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                    __FILE__, __LINE__, '?', collapsed[0], limit);
                bad = true;
                ++errors;
            }
        } else {
            if ('!' != collapsed[limit]) {
                errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                    __FILE__, __LINE__, '!', collapsed[limit], limit);
                bad = true;
                ++errors;
            }
            if ('\0' != collapsed[nc - 1]) {
                errorf("%s[%d]: (0x%hhx!=0x%hhx[%lu])!\n",
                    __FILE__, __LINE__, '\0', collapsed[limit - 1], limit);
                bad = true;
                ++errors;
            }
            if (0 != std::memcmp(raw, collapsed, nc - 1)) {
                errorf("%s[%d]: (@%p!=@%p[%lu])!\n",
                    __FILE__, __LINE__, raw, collapsed, limit);
                bad = true;
                ++errors;
            }
        }
        if (bad) {
            dump.bytes(raw, sizeof(raw));
            dump.bytes(expanded, ne);
            dump.bytes(collapsed, nc);
        }
    }

    printf("%s[%d]: invalid\n", __FILE__, __LINE__);

    const char invalid[] = "+%0h++%g0+++%gh";
    const char result[] = {
        ' ',
        '%', '0', 'h',
        ' ', ' ',
        '%', 'g', '0',
        ' ', ' ', ' ',
        '%', 'g', 'h',
        '\0'
    };

    bad = false;

    nc = encode.collapse(invalid, collapsed, sizeof(collapsed));
    if (sizeof(result) != nc) {
        errorf("%s[%d]: (%lu?%lu)!\n",
            __FILE__, __LINE__, sizeof(result), nc);
        bad = true;
        ++errors;
    }
    if (0 != std::memcmp(result, collapsed, nc)) {
        errorf("%s[%d]: (@%p!=@%p[%lu])!\n",
            __FILE__, __LINE__, result, collapsed, nc);
        bad = true;
        ++errors;
    }
    if (bad) {
        dump.bytes(invalid, sizeof(invalid));
        dump.bytes(result, sizeof(result));
        dump.bytes(collapsed, nc);
    }


    printf("%s[%d]: end errors=%d\n",
        __FILE__, __LINE__, errors);

    return errors;
}
